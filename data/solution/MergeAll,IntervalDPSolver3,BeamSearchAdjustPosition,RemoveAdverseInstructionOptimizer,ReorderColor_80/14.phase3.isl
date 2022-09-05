# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\14.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\14.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\14.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 45199 (100.00 %)
# Total Cost : 45199
# Elapsed    : 1.6e-05 s
# cost = 29751
# num_intervals = 80
cut [0] [20, 255]
cut [0.2] [35, 305]
cut [0.2.2] [320, 385]
color [0.2.2.0] [74, 162, 253, 255]
cut [0.2.2.0] [245, 365]
merge [0.2.2.0.0] [0.2.2.0.1]
color [1] [132, 67, 242, 255]
cut [1] [x] [63]
color [1.1] [228, 62, 251, 255]
cut [1.1] [x] [85]
color [1.1.1] [78, 218, 250, 255]
cut [1.1.1] [x] [264]
color [1.1.1.0] [28, 215, 158, 255]
cut [1.1.1.0] [x] [234]
color [1.1.1.0.0] [255, 255, 255, 255]
color [0.2.2.0.3] [255, 255, 255, 255]
cut [0.2.1] [130, 295]
cut [0.2.1.1] [240, 275]
color [0.2.1.1.3] [52, 126, 151, 255]
cut [0.1] [375, 20]
color [0.1.3] [255, 148, 3, 255]
cut [0.1.3] [345, 235]
merge [0.1.3.0] [0.1.3.1]
color [2] [255, 255, 255, 255]
cut [2] [x] [367]
color [2.0] [248, 131, 18, 255]
cut [2.0] [325, 40]
merge [2.0.3] [2.0.0]
color [3] [255, 255, 255, 255]
cut [3] [y] [31]
color [3.1] [252, 119, 12, 255]
cut [3.1] [295, 60]
merge [3.1.3] [3.1.0]
color [4] [3, 170, 254, 255]
cut [4] [75, 90]
merge [4.1] [4.2]
color [5] [9, 201, 254, 255]
cut [5] [85, 75]
merge [5.2] [5.3]
color [6] [255, 255, 255, 255]
cut [6] [240, 135]
merge [6.2] [6.3]
color [7] [49, 114, 136, 255]
cut [7] [x] [95]
color [7.1] [46, 119, 143, 255]
cut [7.1] [x] [276]
color [7.1.0] [255, 255, 255, 255]
cut [7.1.0] [x] [161]
color [7.1.0.1] [100, 243, 254, 255]
cut [7.1.0.1] [200, 205]
merge [7.1.0.1.1] [7.1.0.1.2]
color [8] [255, 255, 255, 255]
color [7.1.0.1.0] [207, 145, 250, 255]
color [6.0] [255, 255, 255, 255]
cut [6.0] [x] [133]
color [6.0.1] [52, 118, 140, 255]
cut [6.0.1] [y] [95]
color [6.0.1.1] [255, 255, 255, 255]
color [5.1] [255, 255, 255, 255]
color [4.3] [255, 255, 255, 255]
color [3.1.2] [255, 255, 255, 255]
color [2.0.2] [254, 163, 37, 255]
cut [2.0.2] [y] [190]
color [2.0.2.0] [255, 255, 255, 255]
color [0.1.3.3] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 4446 (14.94 %)
#  Sim. Cost : 25305 (85.06 %)
# Total Cost : 29751
# Elapsed    : 3785.170519 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 4427 (14.90 %)
#  Sim. Cost : 25286 (85.10 %)
# Total Cost : 29713
# Elapsed    : 16.23714 s
