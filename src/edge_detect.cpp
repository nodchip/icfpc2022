#include "stdafx.h"
#include "edge_detect.h"

void EdgeDetect::process(int i) {
  cv::GaussianBlur(color[i], after_process[i], cv::Size(5, 5), 5);
  cv::Sobel(after_process[i], after_soberX[i], -1, 1, 0);
  cv::Sobel(after_process[i], after_soberY[i], -1, 0, 1);
}

EdgeDetect::EdgeDetect(PaintingPtr painting_, int tick_size_ , int minimum_interval_) : painting(painting_), maximum_tick_size(tick_size_), minimum_interval(minimum_interval_) {
  border_mapX = cv::Mat::zeros(painting->width, painting->height, CV_8UC1);
  border_mapY = cv::Mat::zeros(painting->width, painting->height, CV_8UC1);
  auto interpret = [&](int i) {
    color[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
    for (int y = 0; y < painting->height; ++y) for (int x = 0; x < painting->width; ++x) {
      color[i].at<uchar>(x, y) = painting->operator()(x, y)[i];
    }
    after_process[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
    after_soberX[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
    after_soberY[i] = cv::Mat(painting->width, painting->height, CV_8UC1);
  };
  for (int i = 0; i < 4; ++i) interpret(i);
  for (int i = 0; i < 4; ++i) process(i);

  //concurrency::parallel_for(0, 4, interpret);
  //concurrency::parallel_for(0, 4, [this](int i) {process(i); });
  for (int i = 0; i < 4; ++i) {
    border_mapX += 0.2 * after_soberX[i];
    border_mapY += 0.2 * after_soberY[i];
  }
}

std::vector<int> EdgeDetect::YTicks(bool sort) {
  std::vector<std::pair<int, int>> tmp;
  for (int y = 0; y < painting->height; ++y) {
    int counter = 0;
    for (int x = 0; x < painting->width; ++x) {
      counter += border_mapX.at<uchar>(x, y);
    }
    tmp.push_back(std::make_pair(counter, y));
  }
  std::sort(tmp.begin(), tmp.end());
  std::reverse(tmp.begin(), tmp.end());
  std::vector<int> ret;
  std::vector<bool> used(painting->height, false);
  for (int i = 0; i < tmp.size(); ++i) {
    auto p = tmp[i].second;
    if (used[p]) continue;
    ret.push_back(p);
    for (int j = std::max(int(0), p - minimum_interval); j < std::min(painting->height, p + minimum_interval); ++j) used[j] = true;
    if (ret.size() >= maximum_tick_size) break;
  }
  if (sort) std::sort(ret.begin(), ret.end());
  return ret;
}

std::vector<int> EdgeDetect::XTicks(bool sort) {
  std::vector<std::pair<int, int>> tmp;
  for (int x = 0; x < painting->width; ++x) {
    int counter = 0;
    for (int y = 0; y < painting->height; ++y) {
      counter += border_mapY.at<uchar>(x, y);
    }
    tmp.push_back(std::make_pair(counter, x));
  }
  std::sort(tmp.begin(), tmp.end());
  std::reverse(tmp.begin(), tmp.end());
  std::vector<bool> used(painting->height, false);
  std::vector<int> ret;
  for (int i = 0; i < tmp.size(); ++i) {
    auto p = tmp[i].second;
    if (used[p]) continue;
    ret.push_back(p);
    for (int j = std::max(int(0), p - minimum_interval); j < std::min(painting->width, p + minimum_interval); ++j) used[j] = true;
    if (ret.size() >= maximum_tick_size) break;
  }
  if (sort) std::sort(ret.begin(), ret.end());
  return ret;
}

void EdgeDetect::Show() {
  cv::Mat debug(painting->width, painting->height, CV_8UC3);
  for (int y = 0; y < painting->height; ++y) for (int x = 0; x < painting->width; ++x) {
    auto val = border_mapX.at<uchar>(x, y);
    debug.at<cv::Vec3b>(x, y) = cv::Vec3b(val, val, val);
  }
  auto RED = cv::Scalar(0, 0, 255);
  auto GREEN = cv::Scalar(0, 255, 0);
  auto BLUE = cv::Scalar(255, 0, 0);
  for (auto e : YTicks()) cv::line(debug, cv::Point(e, 0), cv::Point(e, 400), BLUE, 1);
  for (auto e : XTicks()) cv::line(debug, cv::Point(0, e), cv::Point(400, e), GREEN, 1);
  cv::imshow("borderX", border_mapX);
  cv::imshow("borderY", border_mapY);
  cv::imshow("debug", debug);
  cv::waitKey();
}
