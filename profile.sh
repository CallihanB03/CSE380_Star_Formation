#!/bin/bash

echo "Running non-cached..."
./bin/simulation
mv gmon.out gmon_grav_acc.out
gprof ./bin/simulation gmon_grav_acc.out > report_grav_acc.txt

echo "Running cached..."
./bin/simulation --use_cached
mv gmon.out gmon_cached.out
gprof ./bin/simulation gmon_cached.out > report_cached.txt

echo "Done! Reports: report_grav_acc.txt and report_bh.txt"
