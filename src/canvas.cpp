#include "stdafx.h"
#include "canvas.h"
#include <nlohmann/json.hpp>

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

std::shared_ptr<Canvas> loadCanvasFromJSONFile(const std::string& file_path) {
  nlohmann::json jconfig;
  {
    std::ifstream ifs(file_path);
    ifs >> jconfig;
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
    const Color color(
      int(jblock["color"].at(0)),
      int(jblock["color"].at(1)),
      int(jblock["color"].at(2)),
      int(jblock["color"].at(3)));
    result->blocks[block_id] = std::make_shared<SimpleBlock>(block_id, bottomLeft, topRight, color);
  }

  return result;
}
