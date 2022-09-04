#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "timer.h"

// TODO: pragma dll load
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



cv::Mat_<cv::Vec4b> createCVImage(const Canvas& canvas, bool drawBorder) {
  Frame frame = Painter::draw(canvas, false);

  const int pixel = drawBorder ? 2 : 1;
  const int W = canvas.width;
  const int H = canvas.height;
  std::vector<uint8_t> image(W * pixel * H * pixel * 4);
  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      // BGRA
      for (int i = 0; i < pixel; ++i) {
        for (int j = 0; j < pixel; ++j) {
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 0] = frame[W * y + x][2];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 1] = frame[W * y + x][1];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 2] = frame[W * y + x][0];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 3] = frame[W * y + x][3];
        }
      }
    }
  }

  if (drawBorder) {
    const RGBA border_color{ 0, 255, 0, 255 };
    for (const auto& [block_id, block] : canvas.blocks) {
      assert(block);
      // i* は画像座標
      std::vector<std::tuple<int, int>> border_pixels;
      int ixs[2] = {
        block->bottomLeft.px * pixel,
        block->topRight.px * pixel - 1,
      };
      for (int iy = block->bottomLeft.py * pixel; iy < block->topRight.py * pixel; ++iy) {
        for (auto ix : ixs) {
          border_pixels.emplace_back(ix, iy);
        }
      }
      int iys[2] = {
        block->bottomLeft.py * pixel,
        block->topRight.py * pixel - 1,
      };
      for (int ix = block->bottomLeft.px * pixel; ix < block->topRight.px * pixel; ++ix) {
        for (auto iy : iys) {
          border_pixels.emplace_back(ix, iy);
        }
      }
      for (auto [ix, iy] : border_pixels) {
        image[4 * (W * pixel * iy + ix) + 0] = border_color[0];
        image[4 * (W * pixel * iy + ix) + 1] = border_color[1];
        image[4 * (W * pixel * iy + ix) + 2] = border_color[2];
        image[4 * (W * pixel * iy + ix) + 3] = border_color[3];
      }
    }
  }

  // flip.
  for (int iy = 0; iy < H * pixel / 2; ++iy) {
    uint8_t* src_line = &image[4 * W * pixel * iy];
    uint8_t* dst_line = &image[4 * W * pixel * (H * pixel - 1 - iy)];
    for (int b = 0; b < 4 * W * pixel; ++b) {
      std::swap(src_line[b], dst_line[b]);
    }
  }

  cv::Mat_<cv::Vec4b> image_cv(H * pixel, W * pixel);
  std::memcpy(image_cv.ptr(), image.data(), sizeof(uint8_t) * 4 * H * pixel * W * pixel);

  return image_cv;
}

cv::Mat_<cv::Vec4b> createCVImageWithInfo(const Canvas& canvas, bool drawBorder, const std::string& msg = "") {

  auto img_frame = createCVImage(canvas, drawBorder);
  const int width = img_frame.cols, bottom_height = img_frame.rows;
  const int top_height = 50;

  cv::Mat_<cv::Vec4b> img_info(top_height, width, cv::Vec4b(200, 200, 200, 255));
  cv::putText(img_info, msg, cv::Point(0, top_height / 2), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 1, cv::LINE_AA);

  cv::Mat_<cv::Vec4b> img(bottom_height + top_height, width);
  img_info.copyTo(img(cv::Rect(0, 0, width, top_height)));
  img_frame.copyTo(img(cv::Rect(0, top_height, bottom_height, width)));

  return img;
}


struct SeekBarVisualizer {

  std::string winname = "img";

  Interpreter interpreter;
  std::vector<CanvasPtr> canvas_list;
  std::vector<std::shared_ptr<Instruction>> instructions;

  int frame_id = 0;
  cv::Mat_<cv::Vec4b> img;

  SeekBarVisualizer(const CanvasPtr& initial_canvas) {
    interpreter.top_level_id_counter = initial_canvas->calcTopLevelId();
    canvas_list.push_back(initial_canvas);
    instructions.push_back(nullptr);
  }

  void read_instruction(const std::shared_ptr<Instruction>& inst) {
    auto canvas = canvas_list.back()->Clone();
    int cost = -1;
    auto res = interpreter.Interpret(canvas, inst);
    canvas_list.push_back(canvas);
    instructions.push_back(inst);
  }

  void vis() {
    cv::namedWindow(winname, cv::WINDOW_AUTOSIZE);
    img = createCVImageWithInfo(*canvas_list.front(), true);
    cv::imshow(winname, img);
    cv::createTrackbar("frame id", winname, &frame_id, canvas_list.size() - 1, frame_callback, this);
    cv::setTrackbarPos("frame id", winname, 0);
    cv::setTrackbarMin("frame id", winname, 0);
    while (true) {
      int c = cv::waitKey(15);
      if (c == 27) break;
      cv::imshow(winname, img);
    }
  }

  static void frame_callback(int id, void* param) {
    auto vis = static_cast<SeekBarVisualizer*>(param);
    vis->frame_id = id;
    auto inst = vis->instructions[id];
    vis->img = createCVImageWithInfo(*vis->canvas_list[id], true, inst ? inst->toString() : "initial canvas");
  }

};



class Visualizer : public SolverBase {
public:
  Visualizer() { }
  SolverOutputs solve(const SolverArguments &args) override {

    SolverOutputs ret;

    auto canvas = args.initial_canvas->Clone();
    SeekBarVisualizer visualizer(canvas);

    auto instructions = args.optional_initial_solution;
    for (const auto& inst : instructions) {
      visualizer.read_instruction(inst);
    }

    visualizer.vis();

    ret.solution = instructions;
    return ret;

  }
};
REGISTER_SOLVER("Visualizer", Visualizer);

// vim:ts=2 sw=2 sts=2 et ci

