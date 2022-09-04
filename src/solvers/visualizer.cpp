#include "stdafx.h"
#include <thread>
#include <fmt/format.h>
#include "solver_registry.h"
#include "instruction.h"
#include "timer.h"
#include "similarity_checker.h"

// TODO: pragma dll load
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>



struct MouseParams;
using MouseParamsPtr = std::shared_ptr<MouseParams>;
struct MouseParams {
  int pe, px, py, pf;
  int e, x, y, f;
  MouseParams() { e = x = y = f = pe = px = py = pf = INT_MAX; };
  inline void load(int e_, int x_, int y_, int f_) {
    pe = e;
    px = x;
    py = y;
    pf = f;
    e = e_;
    x = x_;
    y = y_;
    f = f_;
  }
  inline bool clicked_left() const { return e == 1 && pe == 0; }
  inline bool clicked_right() const { return e == 2 && pe == 0; }
  inline bool released_left() const { return e == 4; }
  inline bool released_right() const { return e == 5; }
  inline bool drugging_left() const { return e == 0 && f == 1; }
  inline bool drugging_right() const { return e == 0 && f == 2; }
  inline std::pair<int, int> coord() const { return { x, y }; }
  inline std::pair<int, int> displacement() const {
    return { abs(x - px) > 10000 ? 0 : (x - px), abs(y - py) ? 0 : (y - py) };
  }
  std::string str() const {
    return fmt::format(
      "SMouseParams [(e,x,y,f)=({},{},{},{}), (pe,px,py,pf)=({},{},{},{})", e,
      x, y, f, pe, px, py, pf);
  }
  friend std::ostream& operator<<(std::ostream& o, const MouseParams& obj) {
    o << obj.str();
    return o;
  }
  friend std::ostream& operator<<(std::ostream& o, const MouseParamsPtr& obj) {
    o << obj->str();
    return o;
  }
  friend std::ostream& operator<<(std::ostream& o, const MouseParams* obj) {
    o << obj->str();
    return o;
  }
};

struct SeekBarVisualizer {

  std::string winname = "img";

  Interpreter interpreter;
  PaintingPtr painting;

  std::vector<std::shared_ptr<Instruction>> instructions;
  std::vector<CanvasPtr> canvas_list;
  std::vector<int> inst_cost_list;
  std::vector<double> sim_cost_list;

  int frame_id = 0;
  int alpha = 0;
  bool draw_border_flag = true;
  cv::Mat_<cv::Vec4b> img;
  MouseParamsPtr mp;

  SeekBarVisualizer(const PaintingPtr& painting, const CanvasPtr& initial_canvas, bool draw_border = true)
    : painting(painting)
  {
    mp = std::make_shared<MouseParams>();
    interpreter.top_level_id_counter = initial_canvas->calcTopLevelId();
    instructions.push_back(std::make_shared<NopInstruction>());
    canvas_list.push_back(initial_canvas);
    auto res = computeCost(*painting, canvas_list.front()->Clone(), instructions);
    inst_cost_list.push_back(res->instruction);
    sim_cost_list.push_back(res->similarity);
  }

  void read_instruction(const std::shared_ptr<Instruction>& inst) {
    auto canvas = canvas_list.back()->Clone();
    interpreter.Interpret(canvas, nullptr, inst);
    instructions.push_back(inst);
    canvas_list.push_back(canvas);
    auto res = computeCost(*painting, canvas_list.front()->Clone(), instructions);
    inst_cost_list.push_back(res->instruction);
    sim_cost_list.push_back(res->similarity);
  }

  cv::Mat_<cv::Vec4b> create_board_image() {

    const int pixel = 2;
    const int W = painting->width;
    const int H = painting->height;

    auto canvas = canvas_list[frame_id];

    std::vector<uint8_t> arr(W * pixel * H * pixel * 4);
    {
      const auto& pframe = painting->frame;
      auto cframe = Painter::draw(*canvas, false);
      for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
          // BGRA
          for (int i = 0; i < pixel; ++i) {
            for (int j = 0; j < pixel; ++j) {
              arr[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 0]
                = ((int)pframe[W * y + x][2] * alpha + (int)cframe[W * y + x][2] * (255 - alpha)) / 255;
              arr[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 1]
                = ((int)pframe[W * y + x][1] * alpha + (int)cframe[W * y + x][1] * (255 - alpha)) / 255;
              arr[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 2]
                = ((int)pframe[W * y + x][0] * alpha + (int)cframe[W * y + x][0] * (255 - alpha)) / 255;
              arr[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 3]
                = ((int)pframe[W * y + x][3] * alpha + (int)cframe[W * y + x][3] * (255 - alpha)) / 255;
            }
          }
        }
      }
    }

    if (draw_border_flag) {
      const RGBA border_color{ 0, 255, 0, 255 };
      for (const auto& [block_id, block] : canvas->blocks) {
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
          arr[4 * (W * pixel * iy + ix) + 0] = border_color[0];
          arr[4 * (W * pixel * iy + ix) + 1] = border_color[1];
          arr[4 * (W * pixel * iy + ix) + 2] = border_color[2];
          arr[4 * (W * pixel * iy + ix) + 3] = border_color[3];
        }
      }
    }

    // flip.
    for (int iy = 0; iy < H * pixel / 2; ++iy) {
      uint8_t* src_line = &arr[4 * W * pixel * iy];
      uint8_t* dst_line = &arr[4 * W * pixel * (H * pixel - 1 - iy)];
      for (int b = 0; b < 4 * W * pixel; ++b) {
        std::swap(src_line[b], dst_line[b]);
      }
    }

    cv::Mat_<cv::Vec4b> img(H * pixel, W * pixel);
    std::memcpy(img.ptr(), arr.data(), sizeof(uint8_t) * 4 * H * pixel * W * pixel);

    return img;

  }

  cv::Mat_<cv::Vec4b> create_info_image() {
    const int width = painting->width * 2;
    const int height = 200;
    std::vector<std::string> msgs;
    msgs.push_back(fmt::format("command: {}", instructions[frame_id]->toString()));
    msgs.push_back(fmt::format(" inst cost: {}", inst_cost_list[frame_id]));
    msgs.push_back(fmt::format(" sim cost: {}", sim_cost_list[frame_id]));
    msgs.push_back(fmt::format("total cost: {}", inst_cost_list[frame_id] + sim_cost_list[frame_id]));
    msgs.push_back("");
    msgs.push_back(mp->str());
    msgs.push_back(fmt::format("border(b): {}", draw_border_flag ? "on" : "off"));
    cv::Mat_<cv::Vec4b> img_info(height, width, cv::Vec4b(200, 200, 200, 255));
    for (int i = 0; i < msgs.size(); i++) {
      cv::putText(img_info, msgs[i], cv::Point(5, (i + 1) * 25), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0, 0), 1, cv::LINE_AA);
    }
    return img_info;
  }

  cv::Mat_<cv::Vec4b> create_image() {

    auto img_frame = create_board_image();
    const int width_frame = img_frame.cols;
    const int height_frame = img_frame.rows;

    auto img_info = create_info_image();
    const int width_info = img_info.cols;
    const int height_info = img_info.rows;

    assert(width_frame == width_info);

    cv::Mat_<cv::Vec4b> img(height_info + height_frame, width_frame);
    img_info.copyTo(img(cv::Rect(0, 0, width_info, height_info)));
    img_frame.copyTo(img(cv::Rect(0, height_info, width_frame, height_frame)));

    return img;
  }

  void vis() {

    cv::namedWindow(winname, cv::WINDOW_AUTOSIZE);
    img = create_image();
    cv::imshow(winname, img);

    cv::createTrackbar("frame id", winname, &frame_id, canvas_list.size() - 1, frame_callback, this);
    cv::setTrackbarPos("frame id", winname, 0);
    cv::setTrackbarMin("frame id", winname, 0);

    cv::createTrackbar("alpha", winname, &alpha, 255, alpha_callback, this);
    cv::setTrackbarPos("alpha", winname, 0);
    cv::setTrackbarMin("alpha", winname, 0);

    cv::setMouseCallback(winname, mouse_callback, this);

    while (true) {
      int c = cv::waitKey(15);
      if (c == 'b') draw_border_flag = !draw_border_flag;
      if (c == 27) break;
      img = create_image();
      cv::imshow(winname, img);
    }
  }

  static void frame_callback(int id, void* param) {
    auto vis = static_cast<SeekBarVisualizer*>(param);
    vis->frame_id = id;
  }

  static void alpha_callback(int val, void* param) {
    auto vis = static_cast<SeekBarVisualizer*>(param);
    vis->alpha = val;
  }

  static void mouse_callback(int e, int x, int y, int f, void* param) {
    auto vis = static_cast<SeekBarVisualizer*>(param);
    vis->mp->load(e, x, y, f);
  }

};



class Visualizer : public SolverBase {
public:
  Visualizer() { }
  SolverOutputs solve(const SolverArguments &args) override {

    SolverOutputs ret;

    auto canvas = args.initial_canvas->Clone();
    SeekBarVisualizer visualizer(args.painting, canvas);

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

