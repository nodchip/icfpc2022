# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\3.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\3.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\3.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 303416 (100.00 %)
# Total Cost : 303416
# Elapsed    : 8e-06 s
# cost = 20083
# num_intervals = 80
color [0] [255, 222, 89, 255]
cut [0] [375, 25]
merge [0.3] [0.0]
cut [1] [345, 50]
merge [1.3] [1.0]
color [2] [0, 0, 0, 255]
cut [2] [340, 210]
merge [2.0] [2.1]
color [3] [0, 0, 0, 255]
cut [3] [x] [15]
cut [3.1] [170, 205]
merge [3.1.1] [3.1.2]
color [4] [126, 217, 87, 255]
cut [4] [220, 25]
merge [4.2] [4.3]
color [5] [0, 0, 0, 255]
cut [5] [x] [340]
color [5.0] [0, 74, 173, 255]
cut [5.0] [290, 180]
merge [5.0.0] [5.0.1]
color [6] [0, 0, 0, 255]
cut [6] [x] [323]
color [6.0] [0, 74, 173, 255]
cut [6.0] [195, 100]
merge [6.0.1] [6.0.2]
color [7] [126, 217, 87, 255]
cut [7] [220, 50]
merge [7.1] [7.2]
color [8] [255, 22, 22, 255]
cut [8] [295, 100]
merge [8.2] [8.3]
color [9] [0, 0, 0, 255]
color [8.0] [0, 0, 0, 255]
color [7.3] [0, 0, 0, 255]
color [6.0.3] [255, 22, 22, 255]
color [5.0.3] [0, 0, 0, 255]
color [4.1] [201, 226, 101, 255]
cut [3.1.0] [y] [25]
color [3.1.0.1] [0, 74, 173, 255]
cut [3.1.0.1] [145, 50]
merge [3.1.0.1.2] [3.1.0.1.3]
color [10] [0, 0, 0, 255]
cut [10] [145, 150]
merge [10.3] [10.0]
cut [11] [y] [76]
color [11.1] [217, 217, 217, 255]
cut [11.1] [120, 105]
merge [11.1.2] [11.1.3]
color [12] [94, 23, 235, 255]
cut [12] [x] [118]
color [12.0] [0, 0, 0, 255]
color [11.1.0] [201, 226, 101, 255]
color [10.1] [217, 217, 217, 255]
color [3.1.0.1.0] [0, 0, 0, 255]
color [2.3] [0, 74, 173, 255]
cut [2.3] [315, 235]
merge [2.3.3] [2.3.0]
color [13] [217, 217, 217, 255]
cut [13] [290, 285]
merge [13.3] [13.0]
color [14] [0, 0, 0, 255]
cut [14] [y] [226]
cut [14.1] [200, 235]
merge [14.1.3] [14.1.0]
color [15] [0, 74, 173, 255]
cut [15] [150, 305]
merge [15.3] [15.0]
color [16] [0, 0, 0, 255]
cut [16] [x] [102]
color [16.0] [201, 226, 101, 255]
cut [16.0] [y] [252]
color [16.0.1] [0, 0, 0, 255]
cut [16.0.1] [x] [93]
cut [16.0.1.0] [y] [284]
cut [16.0.1.0.1] [y] [362]
color [16.0.1.0.1.0] [255, 255, 255, 255]
cut [16.0.1.0.1.0] [x] [67]
color [16.0.1.0.1.0.0] [0, 0, 0, 255]
color [15.2] [0, 0, 0, 255]
cut [14.1.2] [y] [336]
color [14.1.2.0] [217, 217, 217, 255]
cut [14.1.2.0] [260, 260]
merge [14.1.2.0.3] [14.1.2.0.0]
color [17] [0, 0, 0, 255]
color [14.1.2.0.2] [255, 22, 22, 255]
color [13.2] [0, 0, 0, 255]
color [2.3.2] [0, 0, 0, 255]
color [1.2] [126, 217, 87, 255]
cut [1.2] [y] [102]
color [1.2.1] [0, 0, 0, 255]
color [0.2] [94, 23, 235, 255]
cut [0.2] [y] [128]
color [0.2.1] [0, 0, 0, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 7217 (35.94 %)
#  Sim. Cost : 12866 (64.06 %)
# Total Cost : 20083
# Elapsed    : 3784.460925 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 7215 (38.77 %)
#  Sim. Cost : 11396 (61.23 %)
# Total Cost : 18611
# Elapsed    : 40.538222999999995 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 6931 (37.82 %)
#  Sim. Cost : 11396 (62.18 %)
# Total Cost : 18327
# Elapsed    : 0.12336399999999999 s
