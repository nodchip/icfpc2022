#pragma once

#include <color.h>
#include <painter.h>

struct SimilarityChecker {
  static constexpr double alpha = 0.005;

  // static dataToFrame(data: [number, number, number, number] [] ) : Frame{
  //   let frame : Frame = [];

  //  for (const item of data) {
  //      frame.push(new RGBA(item));
  //  }

  //  return frame;
  //}
  // TODO?

  static void Initialize();

  static double pixelDiff(const RGBA& p1, const RGBA& p2);

  static int imageDiff(const Frame& f1, const Frame& f2);

  static int imageDiffinROI(const Painting& p1, const Painting& p2, Point bottomLeft, Point topRight);

};
