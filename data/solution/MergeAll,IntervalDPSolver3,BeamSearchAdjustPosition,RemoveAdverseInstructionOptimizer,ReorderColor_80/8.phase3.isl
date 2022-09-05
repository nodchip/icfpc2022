# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\8.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\8.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\8.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 21906 (100.00 %)
# Total Cost : 21906
# Elapsed    : 9.999999999999999e-06 s
# cost = 20642
# num_intervals = 80
cut [0] [30, 10]
cut [0.2] [370, 360]
color [0.2.0] [120, 230, 234, 255]
cut [0.2.0] [145, 350]
merge [0.2.0.1] [0.2.0.2]
color [1] [255, 255, 255, 255]
cut [1] [y] [354]
color [1.0] [202, 217, 238, 255]
cut [1.0] [y] [353]
color [1.0.0] [255, 255, 255, 255]
cut [1.0.0] [175, 250]
merge [1.0.0.0] [1.0.0.1]
color [2] [3, 129, 57, 255]
cut [2] [170, 230]
merge [2.0] [2.1]
color [3] [255, 255, 255, 255]
cut [3] [y] [188]
color [3.0] [34, 55, 120, 255]
cut [3.0] [270, 170]
merge [3.0.1] [3.0.2]
color [4] [255, 255, 255, 255]
color [3.0.0] [255, 255, 255, 255]
color [2.2] [254, 254, 254, 255]
color [1.0.0.2] [15, 83, 177, 255]
cut [1.0.0.2] [y] [333]
color [1.0.0.2.0] [255, 255, 255, 255]
color [0.2.0.0] [255, 255, 255, 255]
cut [0.2.0.0] [y] [62]
color [0.2.0.0.1] [18, 140, 228, 255]
cut [0.2.0.0.1] [90, 75]
merge [0.2.0.0.1.3] [0.2.0.0.1.0]
color [5] [255, 255, 255, 255]
color [0.2.0.0.1.2] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 946 (4.58 %)
#  Sim. Cost : 19696 (95.42 %)
# Total Cost : 20642
# Elapsed    : 3791.498385 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 943 (4.73 %)
#  Sim. Cost : 18978 (95.27 %)
# Total Cost : 19921
# Elapsed    : 6.570659999999999 s
