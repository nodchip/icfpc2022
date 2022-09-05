# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\9.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\9.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\9.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 198787 (100.00 %)
# Total Cost : 198787
# Elapsed    : 9e-06 s
# cost = 17030
# num_intervals = 80
color [0] [254, 254, 254, 255]
cut [0] [x] [3]
color [0.1] [246, 247, 248, 255]
cut [0.1] [y] [383]
color [0.1.0] [242, 243, 244, 255]
cut [0.1.0] [y] [31]
color [0.1.0.1] [245, 246, 247, 255]
cut [0.1.0.1] [x] [378]
color [0.1.0.1.0] [243, 244, 245, 255]
cut [0.1.0.1.0] [x] [30]
color [0.1.0.1.0.1] [125, 125, 127, 255]
cut [0.1.0.1.0.1] [x] [377]
color [0.1.0.1.0.1.0] [0, 0, 0, 255]
cut [0.1.0.1.0.1.0] [x] [54]
color [0.1.0.1.0.1.0.1] [0, 0, 0, 255]
cut [0.1.0.1.0.1.0.1] [90, 290]
merge [0.1.0.1.0.1.0.1.1] [0.1.0.1.0.1.0.1.2]
color [1] [0, 0, 0, 255]
cut [1] [140, 95]
merge [1.1] [1.2]
color [2] [0, 0, 0, 255]
cut [2] [y] [357]
color [2.0] [0, 0, 0, 255]
cut [2.0] [y] [72]
color [2.0.1] [0, 0, 0, 255]
cut [2.0.1] [260, 85]
merge [2.0.1.3] [2.0.1.0]
color [3] [95, 84, 81, 255]
cut [3] [y] [102]
color [3.1] [141, 144, 111, 255]
cut [3.1] [y] [295]
color [3.1.0] [96, 73, 76, 255]
cut [3.1.0] [y] [281]
color [3.1.0.0] [94, 71, 75, 255]
cut [3.1.0.0] [x] [223]
color [3.1.0.0.0] [226, 197, 123, 255]
cut [3.1.0.0.0] [y] [140]
color [3.1.0.0.0.1] [95, 73, 77, 255]
cut [3.1.0.0.0.1] [x] [154]
color [3.1.0.0.0.1.1] [223, 190, 120, 255]
color [2.0.1.2] [0, 0, 0, 255]
cut [2.0.1.2] [y] [341]
color [2.0.1.2.0] [94, 82, 78, 255]
cut [2.0.1.2.0] [290, 145]
merge [2.0.1.2.0.1] [2.0.1.2.0.2]
color [4] [0, 0, 0, 255]
cut [4] [y] [322]
color [4.0] [0, 0, 0, 255]
cut [4.0] [y] [109]
color [4.0.1] [141, 143, 110, 255]
cut [4.0.1] [x] [316]
color [4.0.1.1] [0, 0, 0, 255]
cut [4.0.1.1] [y] [140]
color [4.0.1.1.1] [0, 0, 0, 255]
cut [4.0.1.1.1] [y] [290]
color [4.0.1.1.1.0] [140, 143, 110, 255]
cut [4.0.1.1.1.0] [x] [345]
color [4.0.1.1.1.0.1] [0, 0, 0, 255]
color [2.0.1.2.0.3] [141, 143, 110, 255]
color [1.3] [96, 85, 81, 255]
cut [1.3] [y] [120]
color [1.3.1] [0, 0, 0, 255]
cut [1.3.1] [y] [335]
color [1.3.1.0] [142, 145, 112, 255]
color [0.1.0.1.0.1.0.1.0] [0, 0, 0, 255]
cut [0.1.0.1.0.1.0.1.0] [y] [137]
color [0.1.0.1.0.1.0.1.0.1] [142, 145, 111, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2905 (17.06 %)
#  Sim. Cost : 14125 (82.94 %)
# Total Cost : 17030
# Elapsed    : 3803.6022679999996 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2868 (20.13 %)
#  Sim. Cost : 11377 (79.87 %)
# Total Cost : 14245
# Elapsed    : 58.949377 s
