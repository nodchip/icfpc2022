# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\40.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\40.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\40.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 42755 (100.00 %)
# Total Cost : 42755
# Elapsed    : 7e-06 s
# cost = 19013
# num_intervals = 80
cut [0] [50, 250]
cut [0.2] [x] [60]
color [0.2.1] [0, 0, 0, 255]
cut [0.2.1] [145, 265]
merge [0.2.1.1] [0.2.1.2]
cut [1] [225, 280]
merge [1.2] [1.3]
color [2] [255, 255, 255, 255]
cut [2] [x] [171]
color [2.1] [0, 0, 0, 255]
cut [2.1] [250, 290]
merge [2.1.1] [2.1.2]
color [3] [255, 255, 255, 255]
color [2.1.3] [255, 255, 255, 255]
color [1.1] [255, 255, 255, 255]
color [0.2.1.3] [255, 255, 255, 255]
cut [0.2.1.3] [x] [70]
cut [0.2.1.3.1] [x] [121]
color [0.2.1.3.1.0] [0, 0, 0, 255]
cut [0.2.1.3.1.0] [y] [279]
color [0.2.1.3.1.0.1] [255, 255, 255, 255]
color [0.1] [0, 0, 0, 255]
cut [0.1] [85, 240]
merge [0.1.1] [0.1.2]
color [4] [255, 255, 255, 255]
cut [4] [100, 165]
merge [4.0] [4.1]
color [5] [0, 0, 0, 255]
cut [5] [115, 135]
merge [5.1] [5.2]
color [6] [255, 255, 255, 255]
cut [6] [x] [138]
color [6.1] [0, 0, 0, 255]
cut [6.1] [165, 150]
merge [6.1.0] [6.1.1]
color [7] [255, 255, 255, 255]
cut [7] [x] [144]
color [7.1] [0, 0, 0, 255]
cut [7.1] [195, 135]
merge [7.1.1] [7.1.2]
color [8] [255, 255, 255, 255]
cut [8] [x] [349]
cut [8.0] [x] [250]
color [8.0.1] [127, 127, 127, 255]
cut [8.0.1] [y] [135]
color [8.0.1.0] [255, 255, 255, 255]
color [7.1.0] [255, 255, 255, 255]
color [6.1.2] [255, 255, 255, 255]
color [5.0] [255, 255, 255, 255]
color [4.2] [0, 0, 0, 255]
cut [4.2] [160, 190]
merge [4.2.2] [4.2.3]
color [9] [255, 255, 255, 255]
cut [9] [x] [111]
color [9.1] [0, 0, 0, 255]
cut [9.1] [x] [185]
color [9.1.1] [255, 255, 255, 255]
cut [9.1.1] [y] [205]
color [9.1.1.1] [0, 0, 0, 255]
cut [9.1.1.1] [x] [202]
color [9.1.1.1.1] [255, 255, 255, 255]
cut [9.1.1.1.1] [x] [345]
color [9.1.1.1.1.0] [2, 2, 2, 255]
cut [9.1.1.1.1.0] [y] [243]
color [9.1.1.1.1.0.0] [255, 255, 255, 255]
color [4.2.1] [255, 255, 255, 255]
color [0.1.0] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2993 (15.74 %)
#  Sim. Cost : 16020 (84.26 %)
# Total Cost : 19013
# Elapsed    : 3780.480009 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2979 (15.96 %)
#  Sim. Cost : 15692 (84.04 %)
# Total Cost : 18671
# Elapsed    : 21.795702 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2844 (15.34 %)
#  Sim. Cost : 15692 (84.66 %)
# Total Cost : 18536
# Elapsed    : 0.029862999999999997 s
# Solver     : ReorderColor
# Inst. Cost : 2844 (15.34 %)
#  Sim. Cost : 15692 (84.66 %)
# Total Cost : 18536
# Elapsed    : 3.9e-05 s
