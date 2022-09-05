# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\15.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\15.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\15.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 75113 (100.00 %)
# Total Cost : 75113
# Elapsed    : 9e-06 s
# cost = 25832
# num_intervals = 80
cut [0] [385, 395]
color [0.0] [241, 188, 25, 255]
cut [0.0] [350, 360]
merge [0.0.3] [0.0.0]
color [1] [255, 255, 255, 255]
cut [1] [y] [385]
cut [1.0] [295, 345]
merge [1.0.0] [1.0.1]
cut [2] [55, 305]
merge [2.0] [2.1]
color [3] [206, 225, 244, 255]
cut [3] [295, 225]
merge [3.0] [3.1]
color [4] [255, 255, 255, 255]
cut [4] [120, 15]
merge [4.3] [4.0]
cut [5] [10, 150]
merge [5.0] [5.1]
color [6] [251, 205, 89, 255]
cut [6] [40, 115]
merge [6.1] [6.2]
color [7] [136, 137, 185, 255]
cut [7] [95, 70]
merge [7.0] [7.1]
color [8] [255, 255, 255, 255]
color [7.3] [206, 225, 244, 255]
color [6.0] [255, 255, 255, 255]
color [5.2] [206, 225, 244, 255]
cut [4.2] [290, 45]
merge [4.2.2] [4.2.3]
color [9] [206, 225, 244, 255]
cut [9] [285, 195]
merge [9.0] [9.1]
color [10] [136, 137, 184, 255]
cut [10] [x] [125]
color [10.1] [206, 225, 244, 255]
cut [10.1] [290, 95]
merge [10.1.2] [10.1.3]
color [11] [72, 44, 42, 255]
cut [11] [x] [130]
color [11.1] [251, 205, 89, 255]
cut [11.1] [x] [159]
color [11.1.1] [136, 137, 185, 255]
cut [11.1.1] [x] [183]
color [11.1.1.1] [246, 204, 164, 255]
cut [11.1.1.1] [230, 135]
merge [11.1.1.1.1] [11.1.1.1.2]
color [12] [135, 136, 183, 255]
cut [12] [x] [252]
color [12.1] [251, 205, 89, 255]
cut [12.1] [x] [276]
color [12.1.1] [206, 225, 244, 255]
color [11.1.1.1.3] [251, 205, 89, 255]
color [10.1.0] [136, 137, 185, 255]
color [9.3] [251, 205, 89, 255]
color [4.2.0] [206, 225, 244, 255]
color [3.3] [73, 46, 43, 255]
cut [3.3] [x] [291]
color [3.3.0] [251, 205, 89, 255]
cut [3.3.0] [x] [277]
color [3.3.0.0] [71, 43, 41, 255]
cut [3.3.0.0] [x] [267]
color [3.3.0.0.0] [255, 255, 255, 255]
cut [3.3.0.0.0] [x] [22]
color [3.3.0.0.0.1] [206, 225, 244, 255]
cut [3.3.0.0.0.1] [x] [105]
color [3.3.0.0.0.1.1] [73, 46, 44, 255]
cut [3.3.0.0.0.1.1] [x] [110]
color [3.3.0.0.0.1.1.1] [251, 205, 89, 255]
cut [3.3.0.0.0.1.1.1] [x] [127]
color [3.3.0.0.0.1.1.1.1] [71, 43, 41, 255]
cut [3.3.0.0.0.1.1.1.1] [x] [140]
color [3.3.0.0.0.1.1.1.1.1] [251, 205, 89, 255]
color [2.2] [206, 225, 244, 255]
cut [2.2] [x] [150]
cut [2.2.1] [x] [257]
color [2.2.1.0] [251, 205, 89, 255]
cut [1.0.3] [x] [105]
color [1.0.3.1] [206, 225, 244, 255]
color [0.0.1] [255, 255, 255, 255]
cut [0.0.1] [y] [295]
cut [0.0.1.0] [y] [108]
color [0.0.1.0.1] [206, 225, 244, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 5425 (21.00 %)
#  Sim. Cost : 20407 (79.00 %)
# Total Cost : 25832
# Elapsed    : 3811.855913 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 5394 (21.13 %)
#  Sim. Cost : 20131 (78.87 %)
# Total Cost : 25525
# Elapsed    : 38.432945 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 5089 (20.18 %)
#  Sim. Cost : 20131 (79.82 %)
# Total Cost : 25220
# Elapsed    : 0.098712 s
# Solver     : ReorderColor
# Inst. Cost : 5089 (20.18 %)
#  Sim. Cost : 20131 (79.82 %)
# Total Cost : 25220
# Elapsed    : 4.6e-05 s
