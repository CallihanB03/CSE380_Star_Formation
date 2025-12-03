import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import os



def visualize(data: np.ndarray, save_path: str, frame: int)-> None:
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d') # '111' means 1x1 grid, first subplot

    x, y, z = np.hsplit(data, indices_or_sections=3)

    # Plot the 3D line
    ax.scatter(x, y, z, label='Particles', color="black")

    # Set labels
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')
    ax.set_title(f"Frame {frame}")

    # Add a legend
    ax.legend()

    # Display the plot
    plt.savefig(save_path)

    # Save path
    print(f"Saved plot to: {save_path}")

    return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--path_to_frames')
    parser.add_argument('--path_to_save')
    args = parser.parse_args()

    num_frames = len(list(os.listdir(args.path_to_frames)))

    for i in range(num_frames):
        path_to_frame = "/".join([args.path_to_frames, f'frame_{i}.csv'])
        data = pd.read_csv(path_to_frame)

        pos_cols = ['x', 'y', 'z']
        pos_data = np.array(data[pos_cols], dtype=np.float32)

        save_path = "/".join([args.path_to_save, f"frame_{i}.png"])

        visualize(pos_data, save_path=save_path, frame=i)

if __name__ == "__main__":
    main()