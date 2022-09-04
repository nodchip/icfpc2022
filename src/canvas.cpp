#include "stdafx.h"
#include "canvas.h"
#include <regex>
#include <filesystem>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <lodepng.h>
#include "painter.h"
#include "instruction.h"

Canvas::Canvas(int width, int height, const Color& backgroundColor)
    : width(width), height(height), backgroundColor(backgroundColor)
{
    blocks["0"] = std::make_shared<SimpleBlock>("0", Point(0, 0), Point(width, height), backgroundColor);
}

Size Canvas::size() const {
  return { width, height };
}

std::vector<std::shared_ptr<SimpleBlock>> Canvas::simplify() const {
  std::vector<std::shared_ptr<SimpleBlock>> simplifiedBlocks;
  for (const auto& [id, block] : blocks) {
    for (const auto& child : block->getChildren()) {
      simplifiedBlocks.push_back(child);
    }
  }
  return simplifiedBlocks;
}

CanvasPtr Canvas::Clone() const {
  auto newCanvas = std::make_shared<Canvas>(width, height, backgroundColor);
  for (const auto& [id, block] : blocks) {
    newCanvas->blocks[id] = block->Clone();
  }
  return newCanvas;
}

int Canvas::calcTopLevelId() const {
  int top_id = 0;
  for (const auto& [id, _] : blocks) {
    if (id.find('.') != std::string::npos) continue;
    top_id = std::max(top_id, std::stoi(id));
  }
  return top_id;
}

CanvasPtr loadCanvasFromJSONFile(const std::string& file_path, bool eraseBackground) {
  nlohmann::json jconfig;
  {
    std::ifstream ifs(file_path);
    ifs >> jconfig;
  }

  PaintingPtr source_painting;
  if (jconfig.find("sourcePngPNG") != jconfig.end()) {
    std::string sourcePngPNG = jconfig["sourcePngPNG"];
    std::regex re(R"((\d+)\.source\.png)");
    std::smatch m;
    assert(std::regex_search(sourcePngPNG, m, re));
    const int pid = std::stoi(m[1].str());
    auto png_file_path = (std::filesystem::path(file_path).parent_path() / fmt::format("{}.source.png", pid)).string();
    source_painting = loadPaintingFromPNGFile(png_file_path);

    globallySetInstructionCost(true);
  } else {
    globallySetInstructionCost(false);
  }


  const int width = jconfig["width"];
  const int height = jconfig["height"];
  auto result = std::make_shared<Canvas>(width, height, RGBA(0, 0, 0, 0));
  for (nlohmann::json jblock : jconfig["blocks"]) {
    const std::string block_id = jblock["blockId"];
    const Point bottomLeft(
      int(jblock["bottomLeft"].at(0)), 
      int(jblock["bottomLeft"].at(1)));
    const Point topRight(
      int(jblock["topRight"].at(0)), 
      int(jblock["topRight"].at(1)));
    if (jblock.find("color") != jblock.end()) {
      // lightning
      const Color color(
        int(jblock["color"].at(0)),
        int(jblock["color"].at(1)),
        int(jblock["color"].at(2)),
        int(jblock["color"].at(3)));
      result->blocks[block_id] = std::make_shared<SimpleBlock>(block_id, bottomLeft, topRight, color);
    } else {
      // full div.
      if (eraseBackground) {
        LOG(INFO) << "erasing background.. this may lead to wrong score unless the entire region is color-ed.";
        return createLightningCanvas(width, height);
      } else {
        assert(jblock.find("pngBottomLeftPoint") != jblock.end());
        assert(source_painting);
        const Point pngBottomLeftPoint(
          int(jblock["pngBottomLeftPoint"].at(0)), 
          int(jblock["pngBottomLeftPoint"].at(1)));

        std::vector<std::shared_ptr<SimpleBlock>> sub_blocks;
        const int skip = 1; // > 1 for debug.
        for (int y = bottomLeft.py; y < topRight.py; y+=skip) {
          for (int x = bottomLeft.px; x < topRight.px; x+=skip) {
            const RGBA color = (*source_painting)(pngBottomLeftPoint.px + x - bottomLeft.px, pngBottomLeftPoint.py + y - bottomLeft.py);
            sub_blocks.push_back(std::make_shared<SimpleBlock>(block_id, Point(x, y), Point(x + skip, y + skip), color));
          }
        }
        result->blocks[block_id] = std::make_shared<ComplexBlock>(block_id, bottomLeft, topRight, sub_blocks);
      }
    }
  }

  return result;
}

CanvasPtr createLightningCanvas(int width, int height) {
  auto result = std::make_shared<Canvas>(width, height, RGBA(255, 255, 255, 255));
  result->blocks["0"] = std::make_shared<SimpleBlock>("0", Point(0, 0), Point(width, height), RGBA(255, 255, 255, 255));
  return result;
}

bool storeCanvasToFile(std::string file_path, const Canvas& canvas, bool drawBorder) {
  Frame frame = Painter::draw(canvas, false);

  const int pixel = drawBorder ? 2 : 1;
  const int W = canvas.width;
  const int H = canvas.height;
  std::vector<uint8_t> image(W * pixel * H * pixel * 4);
  for (int y = 0; y < H; ++y) {
    for (int x = 0; x < W; ++x) {
      // RGBA
      for (int i = 0; i < pixel; ++i) {
        for (int j = 0; j < pixel; ++j) {
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 0] = frame[W * y + x][0];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 1] = frame[W * y + x][1];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 2] = frame[W * y + x][2];
          image[4 * (W * pixel * (y * pixel + i) + (x * pixel + j)) + 3] = frame[W * y + x][3];
        }
      }
    }
  }

  if (drawBorder) {
    const RGBA border_color { 0, 255, 0, 255 };
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
  return lodepng::encode(file_path, image, W * pixel, H * pixel) == 0;
}
