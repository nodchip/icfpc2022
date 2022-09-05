# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\4.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\4.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\4.txt
# Solver     : MergeAll
# Inst. Cost : 0 (0.00 %)
#  Sim. Cost : 25308 (100.00 %)
# Total Cost : 25308
# Elapsed    : 9.999999999999999e-06 s
# cost = 18247
# num_intervals = 80
cut [0] [y] [128]
cut [0.1] [x] [323]
cut [0.1.0] [70, 160]
merge [0.1.0.2] [0.1.0.3]
cut [1] [x] [268]
color [1.0] [0, 0, 0, 255]
cut [1.0] [230, 180]
merge [1.0.3] [1.0.0]
color [2] [255, 255, 255, 255]
cut [2] [x] [227]
cut [2.0] [180, 195]
merge [2.0.2] [2.0.3]
color [3] [1, 1, 1, 255]
cut [3] [215, 235]
merge [3.2] [3.3]
color [4] [255, 255, 255, 255]
cut [4] [x] [218]
color [4.0] [0, 0, 0, 255]
cut [4.0] [190, 260]
merge [4.0.2] [4.0.3]
color [5] [255, 255, 255, 255]
cut [5] [x] [210]
color [5.0] [5, 5, 5, 255]
cut [5.0] [200, 310]
merge [5.0.2] [5.0.3]
color [6] [255, 255, 255, 255]
cut [6] [x] [199]
color [6.0] [0, 0, 0, 255]
cut [6.0] [160, 330]
merge [6.0.2] [6.0.3]
color [7] [255, 255, 255, 255]
color [6.0.0] [255, 255, 255, 255]
color [5.0.0] [255, 255, 255, 255]
color [4.0.0] [255, 255, 255, 255]
color [3.0] [255, 255, 255, 255]
cut [3.0] [x] [199]
color [3.0.0] [0, 0, 0, 255]
cut [3.0.0] [x] [174]
color [3.0.0.0] [255, 255, 255, 255]
color [2.0.0] [0, 0, 0, 255]
cut [2.0.0] [x] [157]
color [2.0.0.0] [255, 255, 255, 255]
color [1.0.2] [255, 255, 255, 255]
color [0.1.0.1] [0, 0, 0, 255]
cut [0.1.0.1] [y] [137]
color [0.1.0.1.1] [255, 255, 255, 255]
cut [0.0] [75, 20]
color [0.0.2] [0, 0, 0, 255]
cut [0.0.2] [85, 80]
merge [0.0.2.0] [0.0.2.1]
color [8] [255, 255, 255, 255]
color [0.0.2.2] [255, 255, 255, 255]
cut [0.0.2.2] [x] [99]
color [0.0.2.2.1] [0, 0, 0, 255]
cut [0.0.2.2.1] [x] [109]
color [0.0.2.2.1.1] [255, 255, 255, 255]
cut [0.0.2.2.1.1] [x] [162]
color [0.0.2.2.1.1.1] [0, 0, 0, 255]
cut [0.0.2.2.1.1.1] [x] [172]
color [0.0.2.2.1.1.1.1] [255, 255, 255, 255]
cut [0.0.2.2.1.1.1.1] [x] [215]
color [0.0.2.2.1.1.1.1.1] [0, 0, 0, 255]
cut [0.0.2.2.1.1.1.1.1] [x] [225]
color [0.0.2.2.1.1.1.1.1.1] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 3984 (21.83 %)
#  Sim. Cost : 14263 (78.17 %)
# Total Cost : 18247
# Elapsed    : 3955.9192839999996 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 4030 (23.98 %)
#  Sim. Cost : 12775 (76.02 %)
# Total Cost : 16805
# Elapsed    : 33.156554 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 3996 (23.83 %)
#  Sim. Cost : 12775 (76.17 %)
# Total Cost : 16771
# Elapsed    : 0.020534999999999998 s
# Solver     : ReorderColor
# Inst. Cost : 3996 (23.83 %)
#  Sim. Cost : 12775 (76.17 %)
# Total Cost : 16771
# Elapsed    : 4.4999999999999996e-05 s
