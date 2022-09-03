#include "stdafx.h"
#include <glog/logging.h>
#include <gtest/gtest.h>

#include "string_util.h"

TEST(TestStringUtil, TrimLeft) {
    EXPECT_EQ("hoge", StringUtil::TrimLeft("  hoge"));
}

TEST(TestStringUtil, TrimRight) {
    EXPECT_EQ("hoge", StringUtil::TrimRight("hoge  "));
}

TEST(TestStringUtil, Trim) {
    EXPECT_EQ("hoge", StringUtil::Trim("  hoge  "));
}

// vim:ts=2 sw=2 sts=2 et ci
