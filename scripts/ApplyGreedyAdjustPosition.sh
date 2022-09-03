#!/bin/sh
for problem_number in `seq 25`
do
    ../vs/x64/Release/solver.exe solve GreedyAdjustPosition ../data/problems/${problem_number}.txt ../data/solution/IntervalDPSolver3_GreedyAdjustPosition/${problem_number}.isl ../data/solution/IntervalDPSolver3/${problem_number}.isl --loop 100
done
