# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\39.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\39.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\39.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 274518 (100.00 %)
# Total Cost : 274518
# Elapsed    : 6e-06 s
# cost = 22942
# num_intervals = 80
color [0] [38, 31, 29, 255]
cut [0] [x] [3]
color [0.1] [24, 19, 17, 255]
cut [0.1] [x] [8]
color [0.1.1] [30, 22, 20, 255]
cut [0.1.1] [x] [397]
color [0.1.1.0] [20, 13, 11, 255]
cut [0.1.1.0] [x] [366]
color [0.1.1.0.0] [57, 41, 13, 255]
cut [0.1.1.0.0] [330, 25]
merge [0.1.1.0.0.3] [0.1.1.0.0.0]
color [1] [157, 108, 26, 255]
cut [1] [25, 55]
merge [1.2] [1.3]
color [2] [16, 15, 13, 255]
cut [2] [x] [24]
color [2.1] [102, 73, 32, 255]
cut [2.1] [190, 85]
merge [2.1.2] [2.1.3]
color [3] [21, 13, 11, 255]
cut [3] [x] [326]
color [3.0] [15, 13, 12, 255]
cut [3.0] [x] [35]
color [3.0.1] [174, 127, 51, 255]
cut [3.0.1] [100, 110]
merge [3.0.1.1] [3.0.1.2]
color [4] [64, 48, 20, 255]
cut [4] [125, 275]
merge [4.0] [4.1]
color [5] [21, 14, 11, 255]
cut [5] [x] [300]
color [5.0] [39, 27, 16, 255]
cut [5.0] [140, 205]
merge [5.0.0] [5.0.1]
color [6] [92, 62, 25, 255]
cut [6] [240, 180]
merge [6.3] [6.0]
color [7] [50, 31, 20, 255]
cut [7] [x] [222]
color [7.0] [87, 65, 38, 255]
cut [7.0] [x] [199]
color [7.0.0] [123, 98, 60, 255]
cut [7.0.0] [y] [175]
color [7.0.0.0] [149, 121, 80, 255]
color [6.1] [21, 14, 12, 255]
color [5.0.2] [112, 82, 43, 255]
color [4.2] [118, 87, 51, 255]
cut [4.2] [y] [308]
color [4.2.1] [20, 13, 10, 255]
cut [4.2.1] [x] [280]
color [4.2.1.0] [88, 61, 26, 255]
color [3.0.1.3] [16, 13, 12, 255]
cut [3.0.1.3] [x] [46]
color [3.0.1.3.1] [154, 110, 42, 255]
cut [3.0.1.3.1] [y] [140]
color [3.0.1.3.1.1] [80, 50, 23, 255]
cut [3.0.1.3.1.1] [y] [182]
color [3.0.1.3.1.1.1] [17, 14, 11, 255]
cut [3.0.1.3.1.1.1] [y] [248]
color [3.0.1.3.1.1.1.1] [72, 56, 22, 255]
color [2.1.0] [182, 136, 63, 255]
color [1.1] [168, 120, 45, 255]
cut [1.1] [x] [311]
color [1.1.0] [204, 159, 85, 255]
cut [1.1.0] [x] [273]
color [1.1.0.0] [111, 70, 22, 255]
cut [1.1.0.0] [x] [251]
color [1.1.0.0.0] [143, 95, 28, 255]
cut [1.1.0.0.0] [x] [190]
color [1.1.0.0.0.0] [184, 139, 65, 255]
color [0.1.1.0.0.2] [120, 86, 26, 255]
cut [0.1.1.0.0.2] [y] [101]
color [0.1.1.0.0.2.1] [20, 12, 10, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2092 (9.12 %)
#  Sim. Cost : 20850 (90.88 %)
# Total Cost : 22942
# Elapsed    : 3792.7217219999998 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2087 (9.14 %)
#  Sim. Cost : 20753 (90.86 %)
# Total Cost : 22840
# Elapsed    : 59.110296999999996 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2087 (9.14 %)
#  Sim. Cost : 20753 (90.86 %)
# Total Cost : 22840
# Elapsed    : 0.018362 s
