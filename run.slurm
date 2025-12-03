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

echo "Running non-cached..."
./bin/simulation
mv gmon.out gmon_grav_acc.out
gprof ./bin/simulation gmon_grav_acc.out > report_grav_acc.txt
# ./bin/simulation --use_bh


echo "Running cached..."
./bin/simulation --use_bh
mv gmon.out gmon_bh.out
gprof ./bin/simulation gmon_bh.out > report_bh.txt

echo "Done! Reports: report_grav_acc.txt and report_bh.txt"



# echo "Running: make coverage"
# make coverage


