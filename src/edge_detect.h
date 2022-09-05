#pragma once

#include <opencv2/opencv.hpp>
#include "painter.h"

struct EdgeDetect {
  PaintingPtr painting;
  cv::Mat color[4];
  cv::Mat after_process[4];
  cv::Mat after_soberX[4];
  cv::Mat after_soberY[4];
  cv::Mat border_mapX, border_mapY;


  void process(int i);
  EdgeDetect(PaintingPtr painting_);
  std::vector<int> YTicks(const Point& bottomLeft, const Point& topRight, int minimum_interval, int maximum_tick_size, bool sort = true)const;
  std::vector<int> XTicks(const Point& bottomLeft, const Point& topRight, int minimum_interval, int maximum_tick_size, bool sort = true)const;
  void Show();
};

