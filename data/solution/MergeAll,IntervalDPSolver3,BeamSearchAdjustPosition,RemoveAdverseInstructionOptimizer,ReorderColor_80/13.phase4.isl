# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\13.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\13.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\13.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 179453 (100.00 %)
# Total Cost : 179453
# Elapsed    : 9e-06 s
# cost = 15192
# num_intervals = 80
color [0] [127, 93, 70, 255]
cut [0] [80, 15]
merge [0.1] [0.2]
color [1] [201, 187, 142, 255]
cut [1] [105, 295]
merge [1.1] [1.2]
cut [2] [125, 335]
merge [2.1] [2.2]
cut [3] [y] [361]
color [3.0] [101, 95, 104, 255]
cut [3.0] [225, 325]
merge [3.0.1] [3.0.2]
color [4] [201, 187, 142, 255]
cut [4] [y] [347]
color [4.0] [93, 85, 96, 255]
cut [4.0] [x] [242]
color [4.0.1] [201, 187, 142, 255]
cut [4.0.1] [y] [335]
color [4.0.1.0] [101, 95, 104, 255]
cut [4.0.1.0] [x] [255]
color [4.0.1.0.1] [201, 187, 142, 255]
cut [4.0.1.0.1] [y] [317]
color [4.0.1.0.1.0] [101, 95, 104, 255]
cut [4.0.1.0.1.0] [x] [273]
color [4.0.1.0.1.0.1] [149, 117, 87, 255]
cut [4.0.1.0.1.0.1] [315, 270]
merge [4.0.1.0.1.0.1.0] [4.0.1.0.1.0.1.1]
color [5] [178, 132, 83, 255]
cut [5] [y] [257]
color [5.0] [101, 95, 104, 255]
cut [5.0] [x] [285]
color [5.0.1] [89, 74, 60, 255]
cut [5.0.1] [y] [47]
color [5.0.1.1] [149, 117, 87, 255]
cut [5.0.1.1] [y] [61]
color [5.0.1.1.1] [93, 85, 96, 255]
cut [5.0.1.1.1] [y] [226]
color [5.0.1.1.1.0] [178, 132, 83, 255]
cut [5.0.1.1.1.0] [335, 175]
merge [5.0.1.1.1.0.1] [5.0.1.1.1.0.2]
color [6] [93, 85, 96, 255]
cut [6] [y] [155]
color [6.0] [178, 132, 83, 255]
color [5.0.1.1.1.0.0] [173, 106, 106, 255]
color [4.0.1.0.1.0.1.2] [201, 187, 142, 255]
color [3.0.0] [82, 73, 86, 255]
cut [3.0.0] [y] [70]
color [3.0.0.1] [173, 106, 106, 255]
cut [3.0.0.1] [y] [80]
color [3.0.0.1.1] [93, 85, 96, 255]
cut [3.0.0.1.1] [140, 140]
merge [3.0.0.1.1.2] [3.0.0.1.1.3]
cut [7] [165, 180]
merge [7.1] [7.2]
color [8] [255, 219, 172, 255]
color [7.3] [255, 219, 172, 255]
color [3.0.0.1.1.1] [255, 219, 172, 255]
color [2.0] [93, 85, 96, 255]
color [1.0] [101, 95, 104, 255]
color [0.3] [89, 74, 60, 255]
cut [0.3] [y] [33]
color [0.3.1] [149, 117, 87, 255]
cut [0.3.1] [y] [51]
color [0.3.1.1] [178, 132, 83, 255]
cut [0.3.1.1] [y] [65]
color [0.3.1.1.1] [201, 187, 142, 255]
cut [0.3.1.1.1] [y] [320]
color [0.3.1.1.1.0] [149, 117, 87, 255]
cut [0.3.1.1.1.0] [y] [285]
color [0.3.1.1.1.0.0] [93, 85, 96, 255]
cut [0.3.1.1.1.0.0] [y] [230]
color [0.3.1.1.1.0.0.0] [96, 69, 51, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 3052 (20.09 %)
#  Sim. Cost : 12140 (79.91 %)
# Total Cost : 15192
# Elapsed    : 3954.6538729999997 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 3041 (20.22 %)
#  Sim. Cost : 11995 (79.78 %)
# Total Cost : 15036
# Elapsed    : 33.409085999999995 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 2984 (19.92 %)
#  Sim. Cost : 11996 (80.08 %)
# Total Cost : 14980
# Elapsed    : 0.032299 s
