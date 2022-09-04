#pragma once

#include <opencv2/opencv.hpp>
#include "painter.h"

struct EdgeDetect {
  PaintingPtr painting;
  int maximum_tick_size;
  int minimum_interval;
  cv::Mat color[4];
  cv::Mat after_process[4];
  cv::Mat after_soberX[4];
  cv::Mat after_soberY[4];
  cv::Mat border_mapX, border_mapY;


  void process(int i);
  EdgeDetect(PaintingPtr painting_, int tick_size_ = 20, int minimum_interval_ = 7);
  std::vector<int> YTicks(bool sort = true);
  std::vector<int> XTicks(bool sort = true);;
  void Show();
};

