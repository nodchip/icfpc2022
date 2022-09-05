# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\10.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\10.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\10.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 210784 (100.00 %)
# Total Cost : 210784
# Elapsed    : 9.999999999999999e-06 s
# cost = 21912
# num_intervals = 80
cut [0] [10, 30]
merge [0.2] [0.3]
color [1] [26, 35, 126, 255]
cut [1] [60, 85]
merge [1.2] [1.3]
color [2] [255, 255, 255, 255]
cut [2] [15, 375]
merge [2.0] [2.1]
color [3] [40, 53, 147, 255]
cut [3] [75, 115]
merge [3.1] [3.2]
cut [4] [285, 125]
merge [4.3] [4.0]
color [5] [255, 224, 130, 255]
cut [5] [260, 345]
merge [5.0] [5.1]
color [6] [57, 73, 171, 255]
cut [6] [y] [341]
color [6.0] [33, 33, 33, 255]
cut [6.0] [x] [95]
color [6.0.1] [255, 213, 79, 255]
cut [6.0.1] [130, 300]
merge [6.0.1.0] [6.0.1.1]
color [7] [33, 33, 33, 255]
cut [7] [x] [103]
color [7.1] [92, 107, 192, 255]
cut [7.1] [y] [250]
color [7.1.0] [33, 33, 33, 255]
cut [7.1.0] [x] [122]
cut [7.1.0.1] [160, 155]
merge [7.1.0.1.2] [7.1.0.1.3]
color [8] [57, 73, 171, 255]
cut [8] [x] [145]
color [8.1] [255, 236, 179, 255]
cut [8.1] [210, 190]
merge [8.1.2] [8.1.3]
color [9] [57, 73, 171, 255]
cut [9] [x] [167]
color [9.1] [121, 134, 203, 255]
color [8.1.1] [57, 73, 171, 255]
color [7.1.0.1.1] [26, 35, 126, 255]
cut [7.1.0.1.1] [y] [113]
color [7.1.0.1.1.1] [255, 236, 179, 255]
color [6.0.1.2] [92, 107, 192, 255]
color [5.3] [57, 73, 171, 255]
color [4.2] [26, 35, 126, 255]
cut [4.2] [y] [145]
color [4.2.1] [255, 236, 179, 255]
cut [4.2.1] [y] [169]
cut [4.2.1.1] [370, 200]
merge [4.2.1.1.3] [4.2.1.1.0]
color [10] [57, 73, 171, 255]
cut [10] [315, 315]
merge [10.0] [10.1]
cut [11] [y] [255]
color [11.0] [121, 134, 203, 255]
color [10.2] [254, 235, 178, 255]
color [4.2.1.1.2] [57, 73, 171, 255]
color [3.3] [255, 236, 179, 255]
cut [3.3] [y] [153]
color [3.3.1] [57, 73, 171, 255]
cut [3.3.1] [y] [178]
color [3.3.1.1] [255, 213, 79, 255]
cut [3.3.1.1] [50, 220]
merge [3.3.1.1.2] [3.3.1.1.3]
color [12] [57, 73, 171, 255]
cut [12] [y] [300]
color [12.0] [92, 107, 192, 255]
color [3.3.1.1.0] [57, 73, 171, 255]
cut [2.2] [x] [386]
color [2.2.0] [57, 73, 171, 255]
color [1.1] [33, 33, 33, 255]
cut [1.1] [x] [195]
color [1.1.1] [26, 35, 126, 255]
cut [0.1] [x] [390]
color [0.1.0] [40, 53, 147, 255]
cut [0.1.0] [x] [77]
color [0.1.0.1] [33, 33, 33, 255]
cut [0.1.0.1] [x] [220]
color [0.1.0.1.1] [26, 35, 126, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 5474 (24.98 %)
#  Sim. Cost : 16438 (75.02 %)
# Total Cost : 21912
# Elapsed    : 3792.3851929999996 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 5454 (25.16 %)
#  Sim. Cost : 16226 (74.84 %)
# Total Cost : 21680
# Elapsed    : 30.819606 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 5162 (24.14 %)
#  Sim. Cost : 16226 (75.86 %)
# Total Cost : 21388
# Elapsed    : 0.088824 s
# Solver     : ReorderColor
# Inst. Cost : 5162 (24.14 %)
#  Sim. Cost : 16226 (75.86 %)
# Total Cost : 21388
# Elapsed    : 4.2e-05 s
