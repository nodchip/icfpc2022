# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\19.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\19.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\19.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 253304 (100.00 %)
# Total Cost : 253304
# Elapsed    : 9.999999999999999e-06 s
# cost = 29112
# num_intervals = 80
color [0] [9, 22, 50, 255]
cut [0] [355, 40]
merge [0.3] [0.0]
color [1] [8, 16, 28, 255]
cut [1] [y] [385]
color [1.0] [11, 20, 35, 255]
cut [1.0] [y] [377]
cut [1.0.0] [330, 290]
merge [1.0.0.0] [1.0.0.1]
color [2] [12, 26, 52, 255]
cut [2] [325, 20]
merge [2.3] [2.0]
color [3] [48, 63, 74, 255]
cut [3] [260, 125]
merge [3.2] [3.3]
color [4] [176, 164, 149, 255]
cut [4] [x] [315]
color [4.0] [108, 112, 124, 255]
cut [4.0] [215, 145]
merge [4.0.3] [4.0.0]
cut [5] [x] [204]
color [5.0] [142, 133, 139, 255]
cut [5.0] [x] [186]
color [5.0.0] [17, 27, 40, 255]
cut [5.0.0] [y] [130]
color [5.0.0.1] [218, 203, 208, 255]
cut [5.0.0.1] [135, 255]
merge [5.0.0.1.3] [5.0.0.1.0]
color [6] [16, 26, 37, 255]
cut [6] [y] [165]
color [6.1] [215, 194, 201, 255]
cut [6.1] [x] [118]
color [6.1.0] [16, 26, 40, 255]
color [5.0.0.1.1] [169, 154, 159, 255]
color [4.0.2] [86, 78, 87, 255]
cut [4.0.2] [260, 220]
merge [4.0.2.2] [4.0.2.3]
color [7] [30, 68, 116, 255]
color [4.0.2.1] [25, 32, 51, 255]
color [3.0] [74, 87, 95, 255]
cut [3.0] [x] [246]
color [3.0.0] [138, 137, 127, 255]
cut [3.0.0] [x] [232]
color [3.0.0.0] [228, 236, 238, 255]
cut [3.0.0.0] [190, 100]
merge [3.0.0.0.0] [3.0.0.0.1]
color [8] [170, 154, 131, 255]
cut [8] [x] [176]
color [8.0] [139, 140, 136, 255]
cut [8.0] [155, 50]
merge [8.0.2] [8.0.3]
color [9] [10, 18, 28, 255]
color [8.0.0] [9, 14, 21, 255]
color [3.0.0.0.3] [14, 24, 37, 255]
color [2.2] [72, 74, 76, 255]
cut [2.2] [y] [185]
color [2.2.0] [174, 173, 166, 255]
color [1.0.0.3] [47, 52, 57, 255]
cut [1.0.0.3] [x] [315]
color [1.0.0.3.0] [86, 87, 87, 255]
cut [1.0.0.3.0] [x] [294]
color [1.0.0.3.0.0] [181, 175, 164, 255]
cut [1.0.0.3.0.0] [210, 345]
merge [1.0.0.3.0.0.0] [1.0.0.3.0.0.1]
color [10] [65, 96, 136, 255]
cut [10] [x] [255]
color [10.0] [117, 162, 208, 255]
cut [10.0] [x] [195]
color [10.0.0] [157, 187, 219, 255]
cut [10.0.0] [x] [138]
color [10.0.0.0] [16, 29, 49, 255]
color [1.0.0.3.0.0.3] [12, 24, 44, 255]
color [0.2] [111, 141, 166, 255]
cut [0.2] [y] [90]
color [0.2.1] [10, 15, 26, 255]
cut [0.2.1] [x] [385]
color [0.2.1.0] [64, 84, 116, 255]
cut [0.2.1.0] [y] [175]
color [0.2.1.0.1] [11, 17, 29, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 4082 (14.02 %)
#  Sim. Cost : 25030 (85.98 %)
# Total Cost : 29112
# Elapsed    : 3979.358428 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 4061 (13.99 %)
#  Sim. Cost : 24969 (86.01 %)
# Total Cost : 29030
# Elapsed    : 26.800552 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 4032 (13.89 %)
#  Sim. Cost : 24991 (86.11 %)
# Total Cost : 29023
# Elapsed    : 0.028631 s