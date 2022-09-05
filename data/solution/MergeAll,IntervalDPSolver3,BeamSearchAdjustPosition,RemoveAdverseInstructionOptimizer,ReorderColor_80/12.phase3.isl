# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\12.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\12.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\12.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 24770 (100.00 %)
# Total Cost : 24770
# Elapsed    : 9e-06 s
# cost = 10928
# num_intervals = 80
color [0] [255, 255, 255, 255]
cut [0] [65, 280]
merge [0.0] [0.1]
color [1] [255, 255, 255, 255]
cut [1] [290, 230]
merge [1.3] [1.0]
color [2] [114, 58, 145, 255]
cut [2] [250, 205]
merge [2.0] [2.1]
color [3] [255, 255, 255, 255]
cut [3] [x] [255]
color [3.0] [114, 58, 145, 255]
cut [3.0] [225, 155]
merge [3.0.3] [3.0.0]
color [4] [254, 254, 254, 255]
cut [4] [180, 170]
merge [4.3] [4.0]
color [5] [188, 166, 204, 255]
cut [5] [155, 135]
merge [5.3] [5.0]
color [6] [255, 255, 255, 255]
cut [6] [x] [125]
color [6.0] [189, 167, 205, 255]
cut [6.0] [95, 155]
merge [6.0.3] [6.0.0]
color [7] [255, 255, 255, 255]
cut [7] [y] [170]
color [7.0] [189, 167, 204, 255]
cut [7.0] [50, 115]
merge [7.0.3] [7.0.0]
color [8] [255, 255, 255, 255]
color [7.0.1] [255, 255, 255, 255]
color [6.0.1] [255, 255, 255, 255]
color [5.1] [255, 255, 255, 255]
color [4.1] [114, 58, 145, 255]
cut [4.1] [y] [115]
color [4.1.0] [255, 255, 255, 255]
color [3.0.1] [255, 255, 255, 255]
color [2.3] [255, 255, 255, 255]
cut [2.3] [x] [163]
color [2.3.0] [188, 166, 204, 255]
cut [2.3.0] [x] [118]
color [2.3.0.0] [255, 255, 255, 255]
color [1.1] [114, 58, 144, 255]
cut [1.1] [310, 155]
merge [1.1.0] [1.1.1]
color [9] [255, 255, 255, 255]
cut [9] [x] [296]
color [9.1] [114, 58, 145, 255]
cut [9.1] [345, 115]
merge [9.1.1] [9.1.2]
color [10] [255, 255, 255, 255]
color [9.1.0] [255, 255, 255, 255]
color [1.1.2] [255, 255, 255, 255]
color [0.2] [189, 167, 204, 255]
cut [0.2] [140, 315]
merge [0.2.1] [0.2.2]
color [11] [255, 255, 255, 255]
cut [11] [x] [196]
color [11.1] [114, 58, 145, 255]
cut [11.1] [270, 315]
merge [11.1.1] [11.1.2]
color [12] [255, 255, 255, 255]
color [11.1.3] [255, 255, 255, 255]
color [0.2.3] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 3595 (32.90 %)
#  Sim. Cost : 7333 (67.10 %)
# Total Cost : 10928
# Elapsed    : 3980.836618 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 3604 (33.02 %)
#  Sim. Cost : 7311 (66.98 %)
# Total Cost : 10915
# Elapsed    : 7.5054 s
