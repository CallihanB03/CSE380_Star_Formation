#!/bin/bash

#SBATCH --time=00:05:00
#SBATCH --output=Outputs/starform.txt
#SBATCH --error=Errors/starform.txt
#SBATCH --job-name=StarForm
#SBATCH --partition=skx-dev
#SBATCH -N 1
#SBATCH -n 48 
#SBATCH --mail-type=all
#SBATCH --mail-user=cjb5564@my.utexas.edu

echo "Running: make clean"
make clean

echo "Running: make"
make

