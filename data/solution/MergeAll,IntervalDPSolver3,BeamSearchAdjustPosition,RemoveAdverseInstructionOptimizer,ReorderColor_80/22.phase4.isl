# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\22.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\22.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\22.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 297676 (100.00 %)
# Total Cost : 297676
# Elapsed    : 9.999999999999999e-06 s
# cost = 22922
# num_intervals = 80
color [0] [1, 1, 1, 255]
cut [0] [x] [15]
cut [0.1] [y] [395]
cut [0.1.0] [x] [67]
color [0.1.0.1] [251, 251, 251, 255]
cut [0.1.0.1] [85, 300]
merge [0.1.0.1.1] [0.1.0.1.2]
color [1] [1, 1, 1, 255]
cut [1] [y] [1]
color [1.1] [252, 252, 252, 255]
cut [1.1] [160, 380]
merge [1.1.1] [1.1.2]
cut [2] [175, 300]
merge [2.0] [2.1]
color [3] [1, 1, 2, 255]
cut [3] [x] [175]
cut [3.1] [y] [289]
color [3.1.0] [1, 1, 1, 255]
cut [3.1.0] [y] [69]
color [3.1.0.1] [50, 38, 26, 255]
cut [3.1.0.1] [275, 105]
merge [3.1.0.1.2] [3.1.0.1.3]
color [4] [15, 15, 19, 255]
cut [4] [x] [186]
color [4.1] [251, 251, 251, 255]
cut [4.1] [220, 170]
merge [4.1.2] [4.1.3]
color [5] [1, 1, 1, 255]
cut [5] [y] [212]
color [5.1] [185, 157, 137, 255]
cut [5.1] [x] [227]
color [5.1.1] [1, 1, 1, 255]
color [4.1.1] [1, 1, 1, 255]
color [3.1.0.1.1] [1, 1, 1, 255]
color [2.2] [104, 104, 104, 255]
cut [2.2] [x] [180]
color [2.2.1] [250, 250, 250, 255]
cut [2.2.1] [x] [209]
color [2.2.1.1] [1, 1, 1, 255]
cut [2.2.1.1] [y] [390]
cut [2.2.1.1.0] [230, 360]
merge [2.2.1.1.0.0] [2.2.1.1.0.1]
cut [6] [x] [333]
color [6.0] [250, 250, 250, 255]
color [2.2.1.1.0.2] [247, 247, 247, 255]
cut [2.2.1.1.0.2] [x] [264]
color [2.2.1.1.0.2.1] [1, 1, 1, 255]
color [1.1.0] [1, 1, 1, 255]
cut [1.1.0] [y] [359]
cut [1.1.0.0] [x] [89]
color [1.1.0.0.1] [251, 251, 251, 255]
cut [1.1.0.0.1] [y] [298]
color [1.1.0.0.1.0] [1, 1, 1, 255]
color [0.1.0.1.0] [1, 1, 1, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2547 (11.11 %)
#  Sim. Cost : 20375 (88.89 %)
# Total Cost : 22922
# Elapsed    : 3807.911506 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2563 (11.51 %)
#  Sim. Cost : 19701 (88.49 %)
# Total Cost : 22264
# Elapsed    : 35.585583 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2386 (10.80 %)
#  Sim. Cost : 19712 (89.20 %)
# Total Cost : 22098
# Elapsed    : 0.050166999999999996 s
