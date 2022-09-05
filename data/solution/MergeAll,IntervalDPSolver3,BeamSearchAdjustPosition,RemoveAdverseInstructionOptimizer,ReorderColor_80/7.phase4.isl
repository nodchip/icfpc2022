# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\7.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\7.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\7.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 277945 (100.00 %)
# Total Cost : 277945
# Elapsed    : 8e-06 s
# cost = 18575
# num_intervals = 80
color [0] [0, 0, 0, 255]
cut [0] [x] [19]
cut [0.1] [x] [385]
cut [0.1.0] [255, 255]
merge [0.1.0.0] [0.1.0.1]
cut [1] [x] [25]
color [1.1] [33, 81, 216, 255]
cut [1.1] [85, 235]
merge [1.1.0] [1.1.1]
color [2] [255, 255, 255, 255]
cut [2] [210, 220]
merge [2.0] [2.1]
color [3] [0, 0, 0, 255]
cut [3] [x] [375]
color [3.0] [255, 255, 255, 255]
cut [3.0] [265, 205]
merge [3.0.0] [3.0.1]
color [4] [0, 0, 0, 255]
cut [4] [x] [33]
color [4.1] [33, 81, 216, 255]
cut [4.1] [y] [165]
color [4.1.0] [0, 0, 0, 255]
cut [4.1.0] [x] [40]
cut [4.1.0.1] [x] [365]
color [4.1.0.1.0] [234, 0, 32, 255]
cut [4.1.0.1.0] [170, 145]
merge [4.1.0.1.0.0] [4.1.0.1.0.1]
color [5] [0, 0, 0, 255]
cut [5] [x] [356]
color [5.0] [33, 81, 216, 255]
cut [5.0] [245, 130]
merge [5.0.0] [5.0.1]
color [6] [0, 0, 0, 255]
cut [6] [x] [119]
color [6.1] [234, 0, 32, 255]
cut [6.1] [y] [110]
color [6.1.0] [0, 0, 0, 255]
cut [6.1.0] [x] [332]
cut [6.1.0.0] [x] [195]
color [6.1.0.0.1] [234, 0, 32, 255]
cut [6.1.0.0.1] [y] [83]
color [6.1.0.0.1.0] [0, 0, 0, 255]
color [5.0.3] [234, 0, 32, 255]
color [4.1.0.1.0.2] [33, 81, 216, 255]
color [3.0.3] [33, 81, 216, 255]
color [2.3] [33, 81, 216, 255]
color [1.1.2] [255, 255, 255, 255]
color [0.1.0.3] [255, 255, 255, 255]
cut [0.1.0.3] [210, 280]
merge [0.1.0.3.3] [0.1.0.3.0]
cut [7] [75, 295]
merge [7.2] [7.3]
color [8] [0, 0, 0, 255]
cut [8] [x] [188]
cut [8.0] [x] [50]
color [8.0.1] [255, 255, 255, 255]
cut [8.0.1] [y] [306]
color [8.0.1.1] [0, 0, 0, 255]
cut [8.0.1.1] [x] [155]
color [8.0.1.1.0] [255, 255, 255, 255]
cut [8.0.1.1.0] [y] [327]
color [8.0.1.1.0.1] [0, 0, 0, 255]
color [7.1] [33, 81, 216, 255]
cut [7.1] [x] [137]
color [7.1.1] [255, 255, 255, 255]
color [0.1.0.3.2] [0, 0, 0, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 3733 (20.10 %)
#  Sim. Cost : 14842 (79.90 %)
# Total Cost : 18575
# Elapsed    : 3960.158833 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 3757 (20.47 %)
#  Sim. Cost : 14594 (79.53 %)
# Total Cost : 18351
# Elapsed    : 31.18632 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 3616 (19.86 %)
#  Sim. Cost : 14594 (80.14 %)
# Total Cost : 18210
# Elapsed    : 0.061389 s
