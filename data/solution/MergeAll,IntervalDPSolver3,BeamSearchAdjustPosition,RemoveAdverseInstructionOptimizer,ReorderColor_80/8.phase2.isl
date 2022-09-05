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
color [0.2.0] [132, 232, 236, 255]
cut [0.2.0] [145, 350]
merge [0.2.0.1] [0.2.0.2]
color [1] [255, 255, 255, 255]
cut [1] [y] [355]
color [1.0] [170, 194, 227, 255]
cut [1.0] [y] [350]
color [1.0.0] [255, 255, 255, 255]
cut [1.0.0] [175, 250]
merge [1.0.0.0] [1.0.0.1]
color [2] [7, 130, 59, 255]
cut [2] [170, 230]
merge [2.0] [2.1]
color [3] [255, 255, 255, 255]
cut [3] [y] [185]
color [3.0] [64, 83, 138, 255]
cut [3.0] [270, 170]
merge [3.0.1] [3.0.2]
color [4] [255, 255, 255, 255]
color [3.0.0] [255, 255, 255, 255]
color [2.2] [252, 251, 251, 255]
color [1.0.0.2] [53, 110, 189, 255]
cut [1.0.0.2] [y] [335]
color [1.0.0.2.0] [255, 255, 255, 255]
color [0.2.0.0] [255, 255, 255, 255]
cut [0.2.0.0] [y] [60]
color [0.2.0.0.1] [39, 150, 230, 255]
cut [0.2.0.0.1] [90, 75]
merge [0.2.0.0.1.3] [0.2.0.0.1.0]
color [5] [255, 255, 255, 255]
color [0.2.0.0.1.2] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 946 (4.58 %)
#  Sim. Cost : 19696 (95.42 %)
# Total Cost : 20642
# Elapsed    : 3791.498385 s
