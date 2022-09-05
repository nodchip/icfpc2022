# command line  : vs\x64\Release\solver.exe solve MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor data\problems\26.txt D:\hnoda\icfpc2022\data\solution\MergeAll,IntervalDPSolver3,BeamSearchAdjustPosition,RemoveAdverseInstructionOptimizer,ReorderColor_80\26.isl --interval-dp-num-intervals=80 --interval-dp-2-num-intervals=80 --interval-dp-3-num-intervals=80 --interval-dp-3-num-threads=2 --interval-dp-3-allow-point-cut-merge=true --interval-dp-border-num-intervals=80 --interval-dp-border-num-threads=2 --greedy-adjust-position-loop=100 --greedy-adjust-position-color --beam-search-adjust-position-loop=100 --beam-search-adjust-position-beam-width=10 --beam-search-adjust-position-color --erase-background
# git commit id : 35c3490fb8362dc08cd0abbb1fd6e5b2c2a783f4
# resolved problem : data\problems\26.txt
merge [0] [1]
merge [100] [2]
merge [101] [3]
merge [102] [4]
merge [103] [5]
merge [104] [6]
merge [105] [7]
merge [106] [8]
merge [107] [9]
merge [10] [11]
merge [109] [12]
merge [110] [13]
merge [111] [14]
merge [112] [15]
merge [113] [16]
merge [114] [17]
merge [115] [18]
merge [116] [19]
merge [20] [21]
merge [118] [22]
merge [119] [23]
merge [120] [24]
merge [121] [25]
merge [122] [26]
merge [123] [27]
merge [124] [28]
merge [125] [29]
merge [30] [31]
merge [127] [32]
merge [128] [33]
merge [129] [34]
merge [130] [35]
merge [131] [36]
merge [132] [37]
merge [133] [38]
merge [134] [39]
merge [40] [41]
merge [136] [42]
merge [137] [43]
merge [138] [44]
merge [139] [45]
merge [140] [46]
merge [141] [47]
merge [142] [48]
merge [143] [49]
merge [50] [51]
merge [145] [52]
merge [146] [53]
merge [147] [54]
merge [148] [55]
merge [149] [56]
merge [150] [57]
merge [151] [58]
merge [152] [59]
merge [60] [61]
merge [154] [62]
merge [155] [63]
merge [156] [64]
merge [157] [65]
merge [158] [66]
merge [159] [67]
merge [160] [68]
merge [161] [69]
merge [70] [71]
merge [163] [72]
merge [164] [73]
merge [165] [74]
merge [166] [75]
merge [167] [76]
merge [168] [77]
merge [169] [78]
merge [170] [79]
merge [80] [81]
merge [172] [82]
merge [173] [83]
merge [174] [84]
merge [175] [85]
merge [176] [86]
merge [177] [87]
merge [178] [88]
merge [179] [89]
merge [90] [91]
merge [181] [92]
merge [182] [93]
merge [183] [94]
merge [184] [95]
merge [185] [96]
merge [186] [97]
merge [187] [98]
merge [188] [99]
merge [108] [117]
merge [190] [126]
merge [191] [135]
merge [192] [144]
merge [193] [153]
merge [194] [162]
merge [195] [171]
merge [196] [180]
merge [197] [189]
color [198] [255, 255, 255, 255]
# Solver     : MergeAll
# Inst. Cost : 2863 (6.28 %)
#  Sim. Cost : 42755 (93.72 %)
# Total Cost : 45618
# Elapsed    : 9.7e-05 s
# cost = 19748
# num_intervals = 80
cut [198] [x] [47]
cut [198.1] [65, 250]
merge [198.1.0] [198.1.1]
color [199] [0, 0, 0, 255]
cut [199] [85, 240]
merge [199.1] [199.2]
color [200] [255, 255, 255, 255]
cut [200] [100, 165]
merge [200.0] [200.1]
color [201] [0, 0, 0, 255]
cut [201] [115, 135]
merge [201.1] [201.2]
color [202] [255, 255, 255, 255]
cut [202] [x] [138]
color [202.1] [0, 0, 0, 255]
cut [202.1] [165, 150]
merge [202.1.0] [202.1.1]
color [203] [255, 255, 255, 255]
cut [203] [x] [144]
color [203.1] [0, 0, 0, 255]
cut [203.1] [195, 135]
merge [203.1.1] [203.1.2]
color [204] [255, 255, 255, 255]
cut [204] [x] [349]
cut [204.0] [x] [250]
color [204.0.1] [127, 127, 127, 255]
cut [204.0.1] [y] [135]
color [204.0.1.0] [255, 255, 255, 255]
color [203.1.0] [255, 255, 255, 255]
color [202.1.2] [255, 255, 255, 255]
color [201.0] [255, 255, 255, 255]
color [200.2] [0, 0, 0, 255]
cut [200.2] [160, 190]
merge [200.2.2] [200.2.3]
color [205] [255, 255, 255, 255]
cut [205] [x] [110]
cut [205.1] [x] [345]
color [205.1.0] [0, 0, 0, 255]
cut [205.1.0] [y] [243]
cut [205.1.0.0] [x] [201]
color [205.1.0.0.1] [255, 255, 255, 255]
color [200.2.1] [255, 255, 255, 255]
color [199.0] [255, 255, 255, 255]
color [198.1.2] [0, 0, 0, 255]
cut [198.1.2] [125, 280]
merge [198.1.2.1] [198.1.2.2]
color [206] [255, 255, 255, 255]
cut [206] [x] [135]
color [206.1] [0, 0, 0, 255]
cut [206.1] [225, 275]
merge [206.1.2] [206.1.3]
color [207] [255, 255, 255, 255]
cut [207] [x] [165]
color [207.1] [0, 0, 0, 255]
cut [207.1] [245, 290]
merge [207.1.1] [207.1.2]
color [208] [255, 255, 255, 255]
color [207.1.3] [255, 255, 255, 255]
color [206.1.1] [255, 255, 255, 255]
color [198.1.2.3] [255, 255, 255, 255]
# Solver     : IntervalDPSolver3
# Inst. Cost : 5795 (25.63 %)
#  Sim. Cost : 16816 (74.37 %)
# Total Cost : 22611
# Elapsed    : 3852.454613 s
# Solver     : BeamSearchAdjustPosition
# Inst. Cost : 5772 (25.87 %)
#  Sim. Cost : 16539 (74.13 %)
# Total Cost : 22311
# Elapsed    : 16.874918 s
# Solver     : RemoveAdverseInstructionOptimizer
# Inst. Cost : 5621 (25.37 %)
#  Sim. Cost : 16539 (74.63 %)
# Total Cost : 22160
# Elapsed    : 0.12706 s
# Solver     : ReorderColor
# Inst. Cost : 5621 (25.37 %)
#  Sim. Cost : 16539 (74.63 %)
# Total Cost : 22160
# Elapsed    : 5.1e-05 s
