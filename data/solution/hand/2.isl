cut [0] [105, 107]
cut [0.2] [303, 265]
color [0.2.0] [56, 182, 255, 255]

cut [0.1] [269, 50]
cut [0.1.3] [X] [179]
cut [0.1.3.0] [X] [139]
cut [0.1.3.1] [X] [229]
color [0.1.3.0.1] [255, 222, 89, 255]
color [0.1.3.1.1] [255, 222, 89, 255]

cut [0.2.0] [Y] [138]
cut [0.2.0.0] [Y] [121]
color [0.2.0.0.1] [115, 94, 88, 255]

merge [0.2.0.0.0] [0.2.0.0.1]
merge [1] [0.2.0.1]
merge [2] [0.2.3]
merge [3] [0.3]
merge [0.2.1] [0.2.2]
merge [4] [5]

cut [6] [249, 145]
cut [6.1] [279, 115]
color [6.1.3] [131, 73, 125, 255]

cut [6.3] [X] [117]
cut [6.3.0] [77, 245]
color [6.3.0.1] [255, 22, 22, 255]

cut [6.2] [284, 218]
cut [6.2.2] [323, 312]
color [6.2.2.0] [0, 128, 55, 255]

merge [6.2.0] [6.2.3]
merge [7] [6.3.1]

cut [8] [139, 240]
cut [8.2] [259, 360]
color [8.2.0] [0, 74, 173, 255]

cut [8.2.0] [157, 290]
cut [8.2.0.1] [241, 265]
color [8.2.0.1.3] [92, 225, 230, 255]
cut [8.2.0.1.3] [223, 280]
cut [8.2.0.1.3.3] [X] [175]
color [8.2.0.1.3.3.1] [0, 74, 173, 255]

cut [8.2.0.2] [238, 335]
cut [8.2.0.2.0] [159, 312]
cut [8.2.0.2.0.2] [X] [178]
cut [8.2.0.2.0.2.1] [X] [219]
color [8.2.0.2.0.2.0] [126, 217, 87, 255]
color [8.2.0.2.0.2.1.1] [255, 255, 255, 255]