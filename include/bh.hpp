// barnes_hut.hpp
// Simple Barnes-Hut octree gravity solver (single-file).
// Usage:
//   Particles P = ...; // fill x,y,z,m and N
//   BarnesHutSolver bh(P, /*theta=*/0.6, /*eps=*/1e-3);
//   bh.build();
//   bh.compute_accelerations(P); // writes to P.ax,P.ay,P.az
//
// Note: This implementation uses heap allocation for nodes and deletes them
// at the end of build(). It's simple and clear; replace with a pool allocator
// for performance if desired.

#ifndef BARNES_HUT_HPP
#define BARNES_HUT_HPP

#include <vector>
#include <array>
#include <memory>
#include <limits>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include "particles.hpp"
#include "vec3.hpp"

// Barnes-Hut solver
class BarnesHutSolver {
public:
    BarnesHutSolver(const Particles& P,
                    float theta = 0.6,
                    float eps = 1e-3)
        : theta_(theta), eps2_(eps*eps), root_(nullptr)
    {
        build_bbox(P);
    }

    ~BarnesHutSolver() { clear_tree(); }

    // Build tree from particle set P (makes a fresh tree).
    void build(const Particles& P) {
        clear_tree();
        build_bbox(P);
        root_ = new Node(bbox_center_, bbox_half_);
        // insert particles
        for (size_t i = 0; i < P.N; ++i) {
            if (P.alive.empty() || P.alive[i]) {
                insert(root_, P, i);
            }
        }
        // compute multipoles (mass and center-of-mass)
        // P_ptr_ = &P;
        // compute_multipoles(root_);
        // P_ptr_ = nullptr; // clear after multipoles computed

        // after inserting particles
        compute_multipoles(root_, P);   // call the overload that accepts Particles directly
    }

    // Compute accelerations and write into P.ax,ay,az (overwrites).
    // G is gravitational constant (default 1.0 in code units).
    void compute_accelerations(Particles& P, float G = 1.0) const {
        if (!root_) return;
        for (size_t i = 0; i < P.N; ++i) {
            if (!P.alive.empty() && !P.alive[i]) continue;
            Vec3 pos{P.x[i], P.y[i], P.z[i]};
            Vec3 acc = compute_acc_on_particle(root_, P, i, pos, G);
            P.ax[i] = acc.x;
            P.ay[i] = acc.y;
            P.az[i] = acc.z;
        }
    }

    // Accessors
    float theta() const { return theta_; }
    float softening2() const { return eps2_; }

private:
    struct Node {
        Vec3 center;     // geometric center of this node's cube
        float half;     // half side length of the cube
        float mass;     // total mass in node
        Vec3 com;        // center of mass
        int particle_idx; // if leaf and contains single particle, index>=0, else -1
        std::array<Node*,8> children;
        Node(const Vec3& c, float h)
            : center(c), half(h), mass(0.0), com(0.0,0.0,0.0), particle_idx(-1)
        {
            children.fill(nullptr);
        }
    };

    float theta_;
    float eps2_; // softening squared
    Node* root_;

    // bounding box
    Vec3 bbox_center_;
    float bbox_half_;

    // helpers
    void clear_tree() {
        if (root_) delete_subtree(root_);
        root_ = nullptr;
    }

    void delete_subtree(Node* n) {
        if (!n) return;
        for (int i=0;i<8;++i) if (n->children[i]) delete_subtree(n->children[i]);
        delete n;
    }

    void build_bbox(const Particles& P) {
        // find an axis-aligned cubic bounding box that contains particles (with padding)
        float xmin = std::numeric_limits<float>::infinity();
        float ymin = xmin; float zmin = xmin;
        float xmax = -xmin, ymax = -xmin, zmax = -xmin;
        bool any = false;
        for (size_t i = 0; i < P.N; ++i) {
            if (!P.alive.empty() && !P.alive[i]) continue;
            any = true;
            xmin = std::min(xmin, P.x[i]);
            ymin = std::min(ymin, P.y[i]);
            zmin = std::min(zmin, P.z[i]);
            xmax = std::max(xmax, P.x[i]);
            ymax = std::max(ymax, P.y[i]);
            zmax = std::max(zmax, P.z[i]);
        }
        if (!any) {
            bbox_center_ = Vec3(0.0,0.0,0.0);
            bbox_half_ = 1.0;
            return;
        }
        float cx = 0.5*(xmin+xmax);
        float cy = 0.5*(ymin+ymax);
        float cz = 0.5*(zmin+zmax);
        float dx = xmax - xmin;
        float dy = ymax - ymin;
        float dz = zmax - zmin;
        float half = 0.5 * std::max({dx,dy,dz});
        if (half <= 0) half = 1e-6; // degenerate case
        // pad box slightly
        half *= 1.0001;
        bbox_center_ = Vec3(cx,cy,cz);
        bbox_half_ = half;
    }

    // Octant index for position p relative to node
    inline int octant(const Node* n, const Vec3& p) const {
        int idx = 0;
        if (p.x >= n->center.x) idx |= 1;
        if (p.y >= n->center.y) idx |= 2;
        if (p.z >= n->center.z) idx |= 4;
        return idx;
    }

    // Insert particle i into subtree rooted at n
    void insert(Node* n, const Particles& P, size_t i) {
        Vec3 pos{P.x[i], P.y[i], P.z[i]};
        // if node is empty leaf
        if (n->particle_idx == -1 && all_children_null(n)) {
            // if node has no particle and no children -> store particle
            n->particle_idx = static_cast<int>(i);
            return;
        }

        // If node is a leaf but already contains a particle, subdivide
        if (n->particle_idx >= 0 && all_children_null(n)) {
            int old_idx = n->particle_idx;
            n->particle_idx = -1;
            // create children and re-insert old particle
            subdivide(n);
            int oct_old = octant(n, Vec3{P.x[old_idx], P.y[old_idx], P.z[old_idx]});
            insert(n->children[oct_old], P, old_idx);
        }

        // now insert new particle into appropriate child, creating child if necessary
        int oct = octant(n, pos);
        if (!n->children[oct]) {
            // make child centered appropriately
            Vec3 c = child_center(n->center, n->half, oct);
            n->children[oct] = new Node(c, n->half * 0.5);
        }
        insert(n->children[oct], P, i);
    }

    inline bool all_children_null(const Node* n) const {
        for (int i=0;i<8;++i) if (n->children[i]) return false;
        return true;
    }

    inline void subdivide(Node* n) {
        // allocate 8 children (lazy allocation also OK; here we create only as needed)
        for (int oct=0; oct<8; ++oct) {
            if (!n->children[oct]) {
                Vec3 c = child_center(n->center, n->half, oct);
                n->children[oct] = new Node(c, n->half*0.5);
            }
        }
    }

    inline Vec3 child_center(const Vec3& c, float half, int oct) const {
        float q = half * 0.5;
        // bit 0 -> +x, bit1 -> +y, bit2 -> +z
        return Vec3(
            c.x + ((oct & 1) ? q : -q),
            c.y + ((oct & 2) ? q : -q),
            c.z + ((oct & 4) ? q : -q)
        );
    }
    // Overloaded compute_multipoles that accepts Particles
    void compute_multipoles(Node* n, const Particles& P) {
        if (!n) return;
        // if leaf with single particle
        if (n->particle_idx >= 0) {
            int idx = n->particle_idx;
            n->mass = P.mass[idx];
            n->com = Vec3{P.x[idx], P.y[idx], P.z[idx]};
            return;
        }
        // otherwise sum children
        float mass_sum = 0.0;
        Vec3 com_sum{0.0,0.0,0.0};
        for (int c=0;c<8;++c) {
            if (!n->children[c]) continue;
            compute_multipoles(n->children[c], P);
            mass_sum += n->children[c]->mass;
            com_sum.x += n->children[c]->mass * n->children[c]->com.x;
            com_sum.y += n->children[c]->mass * n->children[c]->com.y;
            com_sum.z += n->children[c]->mass * n->children[c]->com.z;
        }
        n->mass = mass_sum;
        if (mass_sum > 0.0) {
            n->com = Vec3{ com_sum.x / mass_sum, com_sum.y / mass_sum, com_sum.z / mass_sum };
        } else {
            n->com = n->center; // fallback
        }
    }

    // top-level compute multipoles wrapper (public build uses this)
    void compute_multipoles(Node* root) {
        // we need access to Particles used to fill tree; easiest is to store a pointer to P
        // in the class during build(). To keep the implementation self-contained, we'll instead
        // require the build() call to store P_ptr_ temporarily. To keep this file small and clear,
        // we'll use a private member P_ptr_.
        // (See full implementation below where P_ptr_ is set during build.)
        if (!P_ptr_) return;
        compute_multipoles(root, *P_ptr_);
    }

    // compute acceleration on particle i by traversing the tree
    Vec3 compute_acc_on_particle(const Node* n, const Particles& P, size_t i, const Vec3& pos, float G) const {
        Vec3 acc{0.0,0.0,0.0};
        if (!n) return acc;
        // stack for iterative traversal (avoid recursion depth issues)
        struct StackItem { const Node* node; };
        std::vector<const Node*> stack;
        stack.reserve(64);
        stack.push_back(n);

        while (!stack.empty()) {
            const Node* node = stack.back();
            stack.pop_back();
            if (node->mass == 0.0) continue;

            // If node is a leaf with one particle, and it is the same particle, skip
            if (node->particle_idx >= 0) {
                size_t j = static_cast<size_t>(node->particle_idx);
                if (j == i) continue; // skip self
                Vec3 r{ P.x[j] - pos.x, P.y[j] - pos.y, P.z[j] - pos.z };
                float r2 = r.x*r.x + r.y*r.y + r.z*r.z + eps2_;
                float inv_r3 = 1.0 / (std::sqrt(r2) * r2);
                float s = G * P.mass[j] * inv_r3;
                acc.x += s * r.x;
                acc.y += s * r.y;
                acc.z += s * r.z;
                continue;
            }

            // distance from particle to node COM
            Vec3 d{ node->com.x - pos.x, node->com.y - pos.y, node->com.z - pos.z };
            float dist2 = d.x*d.x + d.y*d.y + d.z*d.z + eps2_;
            float dist = std::sqrt(dist2);

            // size = node->half * 2 (side length)
            float size = node->half * 2.0;

            // opening criterion: size / dist < theta  (or dist > size/theta)
            if (size / dist < theta_) {
                // approximate by multipole (monopole only)
                float inv_r3 = 1.0 / (dist2 * dist);
                float s = G * node->mass * inv_r3;
                acc.x += s * d.x;
                acc.y += s * d.y;
                acc.z += s * d.z;
            } else {
                // open node: traverse children
                for (int c = 0; c < 8; ++c) {
                    if (node->children[c]) stack.push_back(node->children[c]);
                }
            }
        }
        return acc;
    }

    // we store pointer to particles used to build the tree (needed when computing multipoles)
    const Particles* P_ptr_ = nullptr;

public:
    // Modified build that saves pointer to P for multipole computation
    void build_with_particles(const Particles& P) {
        P_ptr_ = &P;
        build(P);
        P_ptr_ = nullptr; // clear after multipoles computed
    }
};

#endif // BARNES_HUT_HPP
