#pragma once

namespace StringUtil
{
  std::string TrimLeft(const std::string& s);
  std::string TrimRight(const std::string& s);
  std::string Trim(const std::string& s);
  std::string ReplaceAll(const std::string& s, const std::string& src, const std::string& dst);
}
