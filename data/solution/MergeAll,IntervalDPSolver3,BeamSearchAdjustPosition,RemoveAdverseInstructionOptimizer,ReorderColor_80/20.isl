# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\20.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\20.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\20.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 135112 (100.00 %)
# Total Cost : 135112
# Elapsed    : 9e-06 s
# cost = 18553
# num_intervals = 80
color [0] [95, 82, 86, 255]
cut [0] [130, 25]
merge [0.1] [0.2]
cut [1] [270, 25]
merge [1.3] [1.0]
color [2] [67, 43, 49, 255]
cut [2] [145, 70]
merge [2.2] [2.3]
color [3] [52, 42, 48, 255]
cut [3] [185, 165]
merge [3.2] [3.3]
color [4] [208, 206, 201, 255]
cut [4] [x] [264]
color [4.0] [41, 32, 38, 255]
cut [4.0] [y] [210]
color [4.0.1] [206, 203, 202, 255]
cut [4.0.1] [y] [223]
cut [4.0.1.1] [x] [219]
color [4.0.1.1.0] [64, 60, 44, 255]
cut [4.0.1.1.0] [180, 300]
merge [4.0.1.1.0.3] [4.0.1.1.0.0]
color [5] [202, 200, 194, 255]
color [4.0.1.1.0.2] [193, 193, 186, 255]
color [3.1] [31, 25, 27, 255]
color [2.1] [18, 16, 17, 255]
color [1.2] [112, 100, 97, 255]
cut [1.2] [y] [59]
color [1.2.1] [191, 173, 149, 255]
cut [1.2.1] [y] [68]
color [1.2.1.1] [148, 163, 172, 255]
cut [1.2.1.1] [y] [80]
color [1.2.1.1.1] [165, 181, 196, 255]
cut [1.2.1.1.1] [y] [116]
color [1.2.1.1.1.1] [189, 199, 211, 255]
cut [1.2.1.1.1.1] [y] [138]
color [1.2.1.1.1.1.1] [231, 227, 223, 255]
cut [1.2.1.1.1.1.1] [y] [181]
color [1.2.1.1.1.1.1.1] [202, 201, 195, 255]
cut [1.2.1.1.1.1.1.1] [x] [296]
color [1.2.1.1.1.1.1.1.1] [193, 194, 187, 255]
cut [1.2.1.1.1.1.1.1.1] [330, 270]
merge [1.2.1.1.1.1.1.1.1.0] [1.2.1.1.1.1.1.1.1.1]
color [6] [220, 216, 211, 255]
color [1.2.1.1.1.1.1.1.1.2] [208, 206, 200, 255]
color [0.3] [111, 98, 98, 255]
cut [0.3] [y] [48]
color [0.3.1] [126, 113, 106, 255]
cut [0.3.1] [y] [59]
color [0.3.1.1] [192, 175, 151, 255]
cut [0.3.1.1] [y] [68]
color [0.3.1.1.1] [147, 163, 175, 255]
cut [0.3.1.1.1] [y] [89]
color [0.3.1.1.1.1] [160, 177, 190, 255]
cut [0.3.1.1.1.1] [y] [116]
color [0.3.1.1.1.1.1] [183, 194, 208, 255]
cut [0.3.1.1.1.1.1] [y] [130]
color [0.3.1.1.1.1.1.1] [199, 206, 216, 255]
cut [0.3.1.1.1.1.1.1] [y] [138]
color [0.3.1.1.1.1.1.1.1] [208, 206, 200, 255]
cut [0.3.1.1.1.1.1.1.1] [y] [351]
color [0.3.1.1.1.1.1.1.1.0] [232, 228, 223, 255]
cut [0.3.1.1.1.1.1.1.1.0] [y] [202]
color [0.3.1.1.1.1.1.1.1.0.1] [204, 203, 197, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2233 (12.04 %)
#  Sim. Cost : 16320 (87.96 %)
# Total Cost : 18553
# Elapsed    : 3797.647599 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2236 (12.20 %)
#  Sim. Cost : 16095 (87.80 %)
# Total Cost : 18331
# Elapsed    : 50.0824 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2195 (11.98 %)
#  Sim. Cost : 16129 (88.02 %)
# Total Cost : 18324
# Elapsed    : 0.019823999999999998 s
# Solver     : ReorderColor
# Inst. Cost : 2195 (11.98 %)
#  Sim. Cost : 16129 (88.02 %)
# Total Cost : 18324
# Elapsed    : 4.1e-05 s
