# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\6.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\6.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\6.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 169166 (100.00 %)
# Total Cost : 169166
# Elapsed    : 9.999999999999999e-06 s
# cost = 10540
# num_intervals = 80
color [0] [0, 147, 185, 255]
cut [0] [y] [390]
color [0.0] [255, 255, 255, 255]
cut [0.0] [285, 65]
merge [0.0.2] [0.0.3]
color [1] [227, 228, 226, 255]
cut [1] [y] [75]
color [1.1] [227, 228, 226, 255]
cut [1.1] [y] [125]
color [1.1.1] [78, 78, 79, 255]
cut [1.1.1] [y] [155]
color [1.1.1.1] [0, 147, 185, 255]
cut [1.1.1.1] [y] [340]
color [1.1.1.1.0] [78, 78, 79, 255]
cut [1.1.1.1.0] [x] [60]
color [1.1.1.1.0.1] [78, 78, 79, 255]
cut [1.1.1.1.0.1] [x] [305]
color [1.1.1.1.0.1.0] [133, 187, 64, 255]
cut [1.1.1.1.0.1.0] [y] [165]
color [1.1.1.1.0.1.0.1] [78, 78, 79, 255]
cut [1.1.1.1.0.1.0.1] [y] [180]
color [1.1.1.1.0.1.0.1.1] [78, 78, 79, 255]
cut [1.1.1.1.0.1.0.1.1] [195, 195]
merge [1.1.1.1.0.1.0.1.1.2] [1.1.1.1.0.1.0.1.1.3]
color [2] [78, 78, 79, 255]
cut [2] [y] [210]
color [2.1] [0, 147, 185, 255]
cut [2.1] [260, 225]
merge [2.1.2] [2.1.3]
color [3] [78, 78, 79, 255]
cut [3] [y] [240]
color [3.1] [0, 147, 185, 255]
cut [3.1] [165, 255]
merge [3.1.1] [3.1.2]
color [4] [78, 78, 79, 255]
color [3.1.3] [84, 84, 84, 255]
color [2.1.0] [235, 88, 73, 255]
color [1.1.1.1.0.1.0.1.1.0] [227, 228, 226, 255]
color [0.0.0] [255, 255, 255, 255]
cut [0.0.0] [125, 15]
merge [0.0.0.2] [0.0.0.3]
color [5] [255, 255, 255, 255]
cut [5] [x] [250]
color [5.0] [178, 177, 180, 255]
cut [5.0] [155, 50]
merge [5.0.3] [5.0.0]
color [6] [255, 255, 255, 255]
color [5.0.1] [227, 228, 226, 255]
color [0.0.0.1] [178, 177, 180, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 2620 (24.86 %)
#  Sim. Cost : 7920 (75.14 %)
# Total Cost : 10540
# Elapsed    : 3974.925041 s
