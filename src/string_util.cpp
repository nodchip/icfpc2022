#include "stdafx.h"
#include "string_util.h"

// C ++で文字列をトリミングする–先頭と末尾のスペースを削除します https://www.techiedelight.com/ja/trim-string-cpp-remove-leading-trailing-spaces/

const std::string WHITESPACE = " \n\r\t\f\v";

std::string StringUtil::TrimLeft(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string StringUtil::TrimRight(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string StringUtil::Trim(const std::string& s)
{
  return TrimRight(TrimLeft(s));
}
