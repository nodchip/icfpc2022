# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\25.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\25.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\25.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 287941 (100.00 %)
# Total Cost : 287941
# Elapsed    : 9.999999999999999e-06 s
# cost = 25401
# num_intervals = 80
color [0] [117, 41, 24, 255]
cut [0] [35, 55]
merge [0.2] [0.3]
color [1] [18, 39, 26, 255]
cut [1] [365, 150]
merge [1.2] [1.3]
color [2] [50, 5, 3, 255]
cut [2] [15, 275]
merge [2.2] [2.3]
color [3] [43, 91, 65, 255]
cut [3] [340, 360]
merge [3.3] [3.0]
color [4] [34, 73, 51, 255]
cut [4] [x] [320]
color [4.0] [26, 50, 35, 255]
cut [4.0] [x] [263]
color [4.0.0] [33, 69, 46, 255]
cut [4.0.0] [x] [250]
color [4.0.0.0] [37, 79, 52, 255]
cut [4.0.0.0] [190, 375]
merge [4.0.0.0.0] [4.0.0.0.1]
color [5] [48, 105, 70, 255]
cut [5] [x] [233]
color [5.0] [18, 64, 36, 255]
cut [5.0] [x] [48]
color [5.0.1] [48, 96, 64, 255]
cut [5.0.1] [195, 340]
merge [5.0.1.0] [5.0.1.1]
color [6] [154, 82, 47, 255]
cut [6] [x] [205]
color [6.0] [13, 4, 3, 255]
color [5.0.1.3] [99, 22, 7, 255]
color [4.0.0.0.3] [12, 29, 17, 255]
color [3.1] [56, 118, 80, 255]
color [2.1] [7, 1, 1, 255]
cut [2.1] [x] [75]
color [2.1.1] [63, 17, 5, 255]
cut [2.1.1] [x] [83]
color [2.1.1.1] [97, 41, 20, 255]
cut [2.1.1.1] [x] [95]
color [2.1.1.1.1] [100, 53, 34, 255]
cut [2.1.1.1.1] [140, 220]
merge [2.1.1.1.1.1] [2.1.1.1.1.2]
color [7] [157, 105, 79, 255]
cut [7] [x] [165]
color [7.1] [210, 154, 122, 255]
cut [7.1] [x] [183]
color [7.1.1] [27, 52, 36, 255]
cut [7.1.1] [y] [261]
color [7.1.1.0] [195, 142, 113, 255]
cut [7.1.1.0] [x] [205]
color [7.1.1.0.1] [84, 46, 34, 255]
cut [7.1.1.0.1] [x] [214]
color [7.1.1.0.1.1] [15, 2, 2, 255]
cut [7.1.1.0.1.1] [x] [230]
color [7.1.1.0.1.1.1] [86, 9, 5, 255]
cut [7.1.1.0.1.1.1] [x] [244]
color [7.1.1.0.1.1.1.1] [152, 80, 50, 255]
cut [7.1.1.0.1.1.1.1] [x] [254]
color [7.1.1.0.1.1.1.1.1] [22, 46, 30, 255]
color [2.1.1.1.1.0] [166, 112, 88, 255]
color [1.0] [31, 85, 50, 255]
cut [1.0] [255, 75]
merge [1.0.3] [1.0.0]
color [8] [114, 59, 37, 255]
cut [8] [x] [239]
color [8.0] [77, 2, 1, 255]
cut [8.0] [x] [180]
color [8.0.0] [163, 111, 90, 255]
cut [8.0.0] [130, 105]
merge [8.0.0.0] [8.0.0.1]
color [9] [94, 16, 9, 255]
color [8.0.0.3] [122, 38, 23, 255]
color [1.0.2] [21, 48, 31, 255]
color [0.1] [29, 74, 42, 255]
cut [0.1] [x] [350]
color [0.1.0] [54, 113, 69, 255]
cut [0.1.0] [x] [218]
color [0.1.0.0] [91, 9, 5, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 3585 (14.11 %)
#  Sim. Cost : 21816 (85.89 %)
# Total Cost : 25401
# Elapsed    : 3817.7494709999996 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 3569 (14.08 %)
#  Sim. Cost : 21783 (85.92 %)
# Total Cost : 25352
# Elapsed    : 35.109378 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 3569 (14.08 %)
#  Sim. Cost : 21783 (85.92 %)
# Total Cost : 25352
# Elapsed    : 0.021348 s
