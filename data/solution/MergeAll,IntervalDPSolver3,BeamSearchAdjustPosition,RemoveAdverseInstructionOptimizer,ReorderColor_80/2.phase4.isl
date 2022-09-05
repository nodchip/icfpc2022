# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\2.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\2.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\2.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 66638 (100.00 %)
# Total Cost : 66638
# Elapsed    : 9e-06 s
# cost = 8907
# num_intervals = 80
cut [0] [y] [360]
cut [0.0] [285, 310]
merge [0.0.3] [0.0.0]
cut [1] [y] [50]
cut [1.1] [270, 105]
merge [1.1.2] [1.1.3]
cut [2] [260, 265]
merge [2.0] [2.1]
cut [3] [105, 240]
merge [3.1] [3.2]
color [4] [56, 182, 255, 255]
cut [4] [y] [121]
color [4.1] [115, 94, 88, 255]
cut [4.1] [y] [138]
color [4.1.1] [56, 182, 255, 255]
cut [4.1.1] [x] [259]
cut [4.1.1.0] [140, 240]
merge [4.1.1.0.0] [4.1.1.0.1]
color [5] [255, 22, 22, 255]
cut [5] [x] [117]
color [5.1] [56, 182, 255, 255]
color [4.1.1.0.2] [0, 74, 173, 255]
cut [3.0] [y] [145]
color [3.0.1] [255, 22, 22, 255]
cut [3.0.1] [x] [77]
color [3.0.1.0] [255, 255, 255, 255]
color [2.3] [0, 74, 173, 255]
cut [2.3] [x] [242]
color [2.3.0] [92, 225, 230, 255]
cut [2.3.0] [155, 280]
merge [2.3.0.3] [2.3.0.0]
color [6] [0, 74, 173, 255]
cut [6] [x] [139]
color [6.0] [255, 255, 255, 255]
color [2.3.0.2] [0, 74, 173, 255]
color [1.1.0] [255, 222, 89, 255]
cut [1.1.0] [x] [229]
color [1.1.0.0] [255, 255, 255, 255]
cut [1.1.0.0] [x] [179]
color [1.1.0.0.0] [255, 222, 89, 255]
cut [1.1.0.0.0] [x] [139]
color [1.1.0.0.0.0] [255, 255, 255, 255]
color [0.0.1] [0, 128, 55, 255]
cut [0.0.1] [325, 220]
merge [0.0.1.0] [0.0.1.1]
color [7] [56, 182, 255, 255]
cut [7] [305, 105]
merge [7.0] [7.1]
color [8] [255, 255, 255, 255]
color [7.2] [255, 255, 255, 255]
color [0.0.1.2] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2948 (33.10 %)
#  Sim. Cost : 5959 (66.90 %)
# Total Cost : 8907
# Elapsed    : 3794.50684 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 2943 (37.26 %)
#  Sim. Cost : 4956 (62.74 %)
# Total Cost : 7899
# Elapsed    : 23.601301 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2794 (36.05 %)
#  Sim. Cost : 4956 (63.95 %)
# Total Cost : 7750
# Elapsed    : 0.021192 s
