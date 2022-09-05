# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\17.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\17.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\17.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 218613 (100.00 %)
# Total Cost : 218613
# Elapsed    : 9.999999999999999e-06 s
# cost = 37369
# num_intervals = 80
color [0] [134, 155, 149, 255]
cut [0] [50, 375]
merge [0.1] [0.2]
color [1] [58, 80, 141, 255]
cut [1] [70, 250]
merge [1.1] [1.2]
color [2] [74, 96, 144, 255]
cut [2] [y] [389]
color [2.0] [32, 37, 34, 255]
cut [2.0] [x] [79]
color [2.0.1] [66, 87, 148, 255]
cut [2.0.1] [325, 350]
merge [2.0.1.0] [2.0.1.1]
color [3] [147, 152, 96, 255]
cut [3] [115, 325]
merge [3.0] [3.1]
color [4] [98, 127, 165, 255]
cut [4] [y] [292]
color [4.0] [29, 36, 33, 255]
cut [4.0] [x] [89]
color [4.0.1] [77, 112, 167, 255]
cut [4.0.1] [300, 255]
merge [4.0.1.0] [4.0.1.1]
color [5] [62, 95, 144, 255]
cut [5] [160, 220]
merge [5.0] [5.1]
color [6] [31, 35, 32, 255]
cut [6] [x] [100]
color [6.1] [81, 118, 156, 255]
cut [6.1] [130, 160]
merge [6.1.1] [6.1.2]
color [7] [101, 135, 172, 255]
cut [7] [y] [210]
color [7.0] [182, 192, 176, 255]
cut [7.0] [190, 165]
merge [7.0.0] [7.0.1]
color [8] [135, 161, 163, 255]
cut [8] [y] [144]
color [8.0] [49, 65, 108, 255]
cut [8.0] [275, 120]
merge [8.0.0] [8.0.1]
color [9] [30, 34, 30, 255]
cut [9] [x] [163]
color [9.1] [57, 79, 129, 255]
cut [9.1] [y] [97]
color [9.1.0] [30, 37, 37, 255]
cut [9.1.0] [x] [204]
color [9.1.0.1] [55, 76, 100, 255]
cut [9.1.0.1] [y] [60]
color [9.1.0.1.0] [44, 51, 59, 255]
color [8.0.3] [111, 140, 155, 255]
color [7.0.2] [96, 130, 167, 255]
color [6.1.0] [31, 35, 32, 255]
color [5.2] [98, 129, 168, 255]
color [4.0.1.3] [107, 137, 170, 255]
color [3.2] [80, 107, 157, 255]
color [2.0.1.3] [124, 144, 142, 255]
cut [2.0.1.3] [x] [276]
color [2.0.1.3.0] [53, 72, 141, 255]
color [1.0] [34, 40, 36, 255]
color [0.0] [61, 85, 157, 255]
cut [0.0] [y] [301]
color [0.0.0] [48, 57, 64, 255]
cut [0.0.0] [y] [97]
color [0.0.0.1] [108, 140, 168, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2413 (6.46 %)
#  Sim. Cost : 34956 (93.54 %)
# Total Cost : 37369
# Elapsed    : 4011.571301 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2415 (6.47 %)
#  Sim. Cost : 34906 (93.53 %)
# Total Cost : 37321
# Elapsed    : 25.646411999999998 s
