# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\5.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\5.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\5.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 42755 (100.00 %)
# Total Cost : 42755
# Elapsed    : 9e-06 s
# cost = 19748
# num_intervals = 80
cut [0] [x] [47]
cut [0.1] [65, 250]
merge [0.1.0] [0.1.1]
color [1] [0, 0, 0, 255]
cut [1] [85, 240]
merge [1.1] [1.2]
color [2] [255, 255, 255, 255]
cut [2] [100, 165]
merge [2.0] [2.1]
color [3] [0, 0, 0, 255]
cut [3] [115, 135]
merge [3.1] [3.2]
color [4] [255, 255, 255, 255]
cut [4] [x] [138]
color [4.1] [0, 0, 0, 255]
cut [4.1] [165, 150]
merge [4.1.0] [4.1.1]
color [5] [255, 255, 255, 255]
cut [5] [x] [144]
color [5.1] [0, 0, 0, 255]
cut [5.1] [195, 135]
merge [5.1.1] [5.1.2]
color [6] [255, 255, 255, 255]
cut [6] [x] [349]
cut [6.0] [x] [250]
color [6.0.1] [127, 127, 127, 255]
cut [6.0.1] [y] [135]
color [6.0.1.0] [255, 255, 255, 255]
color [5.1.0] [255, 255, 255, 255]
color [4.1.2] [255, 255, 255, 255]
color [3.0] [255, 255, 255, 255]
color [2.2] [0, 0, 0, 255]
cut [2.2] [160, 190]
merge [2.2.2] [2.2.3]
color [7] [255, 255, 255, 255]
cut [7] [x] [110]
cut [7.1] [x] [345]
color [7.1.0] [0, 0, 0, 255]
cut [7.1.0] [y] [243]
cut [7.1.0.0] [x] [201]
color [7.1.0.0.1] [255, 255, 255, 255]
color [2.2.1] [255, 255, 255, 255]
color [1.0] [255, 255, 255, 255]
color [0.1.2] [0, 0, 0, 255]
cut [0.1.2] [125, 280]
merge [0.1.2.1] [0.1.2.2]
color [8] [255, 255, 255, 255]
cut [8] [x] [135]
color [8.1] [0, 0, 0, 255]
cut [8.1] [225, 275]
merge [8.1.2] [8.1.3]
color [9] [255, 255, 255, 255]
cut [9] [x] [165]
color [9.1] [0, 0, 0, 255]
cut [9.1] [245, 290]
merge [9.1.1] [9.1.2]
color [10] [255, 255, 255, 255]
color [9.1.3] [255, 255, 255, 255]
color [8.1.1] [255, 255, 255, 255]
color [0.1.2.3] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2932 (14.85 %)
#  Sim. Cost : 16816 (85.15 %)
# Total Cost : 19748
# Elapsed    : 3790.556933 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2909 (14.96 %)
#  Sim. Cost : 16539 (85.04 %)
# Total Cost : 19448
# Elapsed    : 12.578496 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2758 (14.29 %)
#  Sim. Cost : 16539 (85.71 %)
# Total Cost : 19297
# Elapsed    : 0.035906 s
