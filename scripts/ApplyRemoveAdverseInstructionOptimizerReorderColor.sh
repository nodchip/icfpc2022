#!/bin/sh
for problem_number in `seq 40`
do
    ../vs/x64/Release/solver.exe solve RemoveAdverseInstructionOptimizer,ReorderColor --interval-dp-3-inherit-ticks --interval-dp-3-prune-threshold=0.0 --interval-dp-3-allow-point-cut-merge /c/home/nodchip/icfpc2022/data/problems/${problem_number}.txt /c/home/nodchip/icfpc2022/data/solution/RemoveAdverseInstructionOptimizer,ReorderColor_post_processing/${problem_number}.isl /c/home/nodchip/icfpc2022/data/solution/IntervalDPSolver3_post_processing/${problem_number}.isl
done
