#!/bin/bash
rm -f gmon.out

echo "Running non-cached..."
./bin/simulation
mv gmon.out gmon_normal.out
gprof ./bin/simulation gmon_normal.out > report_normal.txt

echo "Running cached..."
./bin/simulation --cached
mv gmon.out gmon_cached.out
gprof ./bin/simulation gmon_cached.out > report_cached.txt

echo "Done! Reports: report_normal.txt and report_cached.txt"
