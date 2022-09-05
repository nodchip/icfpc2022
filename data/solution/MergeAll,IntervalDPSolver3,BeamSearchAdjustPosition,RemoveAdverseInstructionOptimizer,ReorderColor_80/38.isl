# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\38.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\38.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\38.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 277170 (100.00 %)
# Total Cost : 277170
# Elapsed    : 8e-06 s
# cost = 27071
# num_intervals = 80
color [0] [42, 42, 39, 255]
cut [0] [365, 50]
merge [0.3] [0.0]
color [1] [19, 21, 28, 255]
cut [1] [x] [30]
color [1.1] [80, 73, 49, 255]
cut [1.1] [y] [10]
color [1.1.1] [36, 36, 33, 255]
cut [1.1.1] [x] [355]
color [1.1.1.0] [78, 71, 38, 255]
cut [1.1.1.0] [60, 70]
merge [1.1.1.0.1] [1.1.1.0.2]
color [2] [68, 62, 47, 255]
cut [2] [245, 20]
merge [2.3] [2.0]
color [3] [22, 25, 32, 255]
cut [3] [125, 90]
merge [3.2] [3.3]
color [4] [184, 145, 60, 255]
cut [4] [230, 180]
merge [4.2] [4.3]
color [5] [19, 20, 26, 255]
cut [5] [x] [70]
color [5.1] [112, 82, 45, 255]
cut [5.1] [y] [196]
color [5.1.1] [60, 52, 41, 255]
cut [5.1.1] [x] [205]
color [5.1.1.0] [21, 22, 29, 255]
cut [5.1.1.0] [100, 250]
merge [5.1.1.0.2] [5.1.1.0.3]
color [6] [87, 69, 48, 255]
cut [6] [x] [177]
color [6.0] [46, 41, 35, 255]
cut [6.0] [y] [370]
color [6.0.0] [163, 114, 60, 255]
cut [6.0.0] [y] [296]
color [6.0.0.1] [21, 21, 25, 255]
cut [6.0.0.1] [x] [94]
color [6.0.0.1.1] [131, 91, 45, 255]
color [5.1.1.0.1] [111, 81, 44, 255]
color [4.0] [81, 62, 39, 255]
cut [4.0] [y] [155]
color [4.0.0] [180, 149, 71, 255]
cut [4.0.0] [x] [170]
color [4.0.0.0] [21, 24, 31, 255]
color [3.1] [109, 98, 57, 255]
color [2.2] [92, 63, 31, 255]
cut [2.2] [y] [360]
color [2.2.0] [37, 30, 24, 255]
cut [2.2.0] [y] [332]
color [2.2.0.0] [178, 177, 164, 255]
cut [2.2.0.0] [260, 270]
merge [2.2.0.0.0] [2.2.0.0.1]
color [7] [28, 27, 24, 255]
cut [7] [y] [247]
color [7.0] [123, 107, 61, 255]
cut [7.0] [x] [273]
color [7.0.1] [37, 40, 44, 255]
cut [7.0.1] [y] [188]
color [7.0.1.0] [164, 161, 134, 255]
cut [7.0.1.0] [305, 150]
merge [7.0.1.0.1] [7.0.1.0.2]
color [8] [56, 55, 47, 255]
color [7.0.1.0.0] [102, 90, 54, 255]
color [2.2.0.0.2] [31, 27, 25, 255]
color [1.1.1.0.3] [19, 20, 27, 255]
color [0.2] [23, 26, 33, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2201 (8.13 %)
#  Sim. Cost : 24870 (91.87 %)
# Total Cost : 27071
# Elapsed    : 3936.1060639999996 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2177 (8.06 %)
#  Sim. Cost : 24844 (91.94 %)
# Total Cost : 27021
# Elapsed    : 20.587274 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2177 (8.06 %)
#  Sim. Cost : 24844 (91.94 %)
# Total Cost : 27021
# Elapsed    : 0.017032 s
# Solver     : ReorderColor
# Inst. Cost : 2177 (8.06 %)
#  Sim. Cost : 24844 (91.94 %)
# Total Cost : 27021
# Elapsed    : 4.1e-05 s
