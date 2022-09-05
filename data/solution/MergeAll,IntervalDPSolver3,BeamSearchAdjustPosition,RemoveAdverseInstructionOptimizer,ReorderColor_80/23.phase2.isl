# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\23.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\23.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\23.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 93953 (100.00 %)
# Total Cost : 93953
# Elapsed    : 1.2e-05 s
# cost = 27055
# num_intervals = 80
cut [0] [25, 375]
cut [0.1] [375, 180]
color [0.1.3] [239, 251, 228, 255]
cut [0.1.3] [y] [190]
color [0.1.3.1] [239, 251, 228, 255]
cut [0.1.3.1] [x] [40]
color [0.1.3.1.1] [238, 250, 228, 255]
cut [0.1.3.1.1] [y] [370]
color [0.1.3.1.1.0] [126, 136, 127, 255]
cut [0.1.3.1.1.0] [x] [45]
color [0.1.3.1.1.0.1] [239, 251, 228, 255]
cut [0.1.3.1.1.0.1] [x] [360]
color [0.1.3.1.1.0.1.0] [205, 205, 184, 255]
cut [0.1.3.1.1.0.1.0] [100, 210]
merge [0.1.3.1.1.0.1.0.1] [0.1.3.1.1.0.1.0.2]
color [1] [10, 14, 21, 255]
cut [1] [140, 325]
merge [1.1] [1.2]
color [2] [211, 208, 180, 255]
cut [2] [180, 215]
merge [2.1] [2.2]
color [3] [164, 218, 246, 255]
cut [3] [210, 355]
merge [3.1] [3.2]
color [4] [51, 56, 52, 255]
cut [4] [x] [225]
color [4.1] [219, 216, 185, 255]
cut [4.1] [275, 225]
merge [4.1.1] [4.1.2]
color [5] [55, 55, 57, 255]
cut [5] [300, 225]
merge [5.2] [5.3]
color [6] [222, 219, 192, 255]
cut [6] [305, 300]
merge [6.2] [6.3]
color [7] [15, 17, 23, 255]
color [6.1] [114, 121, 104, 255]
color [5.1] [223, 219, 186, 255]
color [4.1.3] [62, 69, 66, 255]
color [3.0] [77, 86, 78, 255]
color [2.3] [50, 62, 64, 255]
color [1.0] [70, 84, 77, 255]
color [0.1.3.1.1.0.1.0.3] [11, 14, 22, 255]
cut [0.1.3.1.1.0.1.0.3] [y] [295]
color [0.1.3.1.1.0.1.0.3.0] [56, 73, 79, 255]
cut [0.1.0] [x] [65]
cut [0.1.0.1] [335, 20]
color [0.1.0.1.3] [189, 143, 81, 255]
cut [0.1.0.1.3] [y] [170]
color [0.1.0.1.3.0] [255, 255, 255, 255]
cut [0.1.0.1.3.0] [x] [195]
color [0.1.0.1.3.0.1] [208, 144, 60, 255]
cut [0.1.0.1.3.0.1] [x] [205]
color [0.1.0.1.3.0.1.1] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2935 (10.85 %)
#  Sim. Cost : 24120 (89.15 %)
# Total Cost : 27055
# Elapsed    : 3987.0399319999997 s
