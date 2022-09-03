#include "stdafx.h"
#include "parser.h"

#include <regex>

#include <fmt/color.h>

#include "string_util.h"

ParseResult Parser::Parse(const std::string& code)
{
  // TODO(nodchip): Implement.
  return std::make_shared<ParseError>(0, "");
}

ParseResult Parser::ParseLine(int line_number, std::string line)
{
  if (StringUtil::Trim(line) == "") {
    return std::make_shared<NopInstruction>();
  }

  if (line.starts_with("#")) {
    auto result = std::make_shared<CommentInstruction>(line.substr(1));
    result->comment = line.substr(1);
    return result;
  }

  line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
  const std::string number_re = "(0|[1-9][0-9]*)";
  const std::string byte_re = "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])";
  const std::string color_re = fmt::format("{},{},{},{}", byte_re, byte_re, byte_re, byte_re);
  const std::string block_id_re = fmt::format("{}(\\.{})*", number_re, number_re);
  const std::string point_re = fmt::format("{},{}", number_re, number_re);
  const std::regex COLOR_INSTRUCTION_REGEX = std::regex(fmt::format("color\\[({})\\]\\[({})\\]", block_id_re, color_re));
  const std::regex LINE_CUT_INSTRUCTION_REGEX = std::regex(fmt::format("cut\\[({})\\]\\[(x|X|y|Y)\\]\\[({})\\]", block_id_re, number_re));
  const std::regex POINT_CUT_INSTRUCTION_REGEX = std::regex(fmt::format("cut\\[({})\\]\\[({})\\]", block_id_re, point_re));
  const std::regex MERGE_INSTRUCTION_REGEX = std::regex(fmt::format("merge\\[({})\\]\\[({})\\]", block_id_re, block_id_re));
  const std::regex SWAP_INSTRUCTION_REGEX = std::regex(fmt::format("swap\\[({})\\]\\[({})\\]", block_id_re, block_id_re));

  std::smatch color_match_result;
  if (std::regex_match(line, color_match_result, COLOR_INSTRUCTION_REGEX)) {
    auto block_id = color_match_result[1].str();
    auto r = std::stoi(color_match_result[6].str());
    auto g = std::stoi(color_match_result[7].str());
    auto b = std::stoi(color_match_result[8].str());
    auto a = std::stoi(color_match_result[9].str());
    return std::make_shared<ColorInstruction>(block_id, Color(r, g, b, a));
  }

  std::smatch line_cut_match_result;
  if (std::regex_match(line, line_cut_match_result, LINE_CUT_INSTRUCTION_REGEX)) {
    auto orientation = line_cut_match_result[5].str();
    if (orientation == "x" || orientation == "X") {
      auto block_id = line_cut_match_result[1].str();
      auto line_number = std::stoi(line_cut_match_result[6].str());
      return std::make_shared<VerticalCutInstruction>(block_id, line_number);
    }
    else {
      auto block_id = line_cut_match_result[1].str();
      auto line_number = std::stoi(line_cut_match_result[6].str());
      return std::make_shared<HorizontalCutInstruction>(block_id, line_number);
    }
  }

  std::smatch point_cut_match_result;
  if (std::regex_match(line, point_cut_match_result, POINT_CUT_INSTRUCTION_REGEX)) {
    auto block_id = point_cut_match_result[1].str();
    auto px = std::stoi(point_cut_match_result[6].str());
    auto py = std::stoi(point_cut_match_result[7].str());
    return std::make_shared<PointCutInstruction>(block_id, Point(px, py));
  }

  std::smatch merge_match_result;
  if (std::regex_match(line, merge_match_result, MERGE_INSTRUCTION_REGEX)) {
    auto block_id1 = merge_match_result[1].str();
    auto block_id2 = merge_match_result[5].str();
    return std::make_shared<MergeInstruction>(block_id1, block_id2);
  }

  std::smatch swap_match_result;
  if (std::regex_match(line, swap_match_result, SWAP_INSTRUCTION_REGEX)) {
    auto block_id1 = swap_match_result[1].str();
    auto block_id2 = swap_match_result[5].str();
    return std::make_shared<SwapInstruction>(block_id1, block_id2);
  }

  auto message = fmt::format("Cannot parse the instruction[{}]!", line);
  return std::make_shared<ParseError>(line_number, message);
}
