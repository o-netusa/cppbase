/**************************************************************************
 * @file:  EncodingTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 **************************************************************************/

#include <common/Encoding.h>
#include <gtest/gtest.h>

using namespace cppbase::encoding;

static void test(const std::string& utf8, const std::u16string& utf16, const std::u32string& utf32)
{
    EXPECT_EQ(utf16, UTF8toUTF16(utf8));
    EXPECT_EQ(utf32, UTF8toUTF32(utf8));
    EXPECT_EQ(utf8, UTF16toUTF8(utf16));
    EXPECT_EQ(utf32, UTF16toUTF32(utf16));
    EXPECT_EQ(utf8, UTF32toUTF8(utf32));
    EXPECT_EQ(utf16, UTF32toUTF16(utf32));

    std::wstring wstr = FromUTF8(utf8);
    std::string str = ToUTF8(wstr);
    EXPECT_EQ(utf8, str);
}

TEST(EncodingTests, Encoding)
{
    test("\x61", u"\x0061", U"\x00000061");
    test("\xEF\xBD\x81", u"\xFF41", U"\x0000FF41");
    test("\xC4\x8D", u"\x010D", U"\x010D");
    test("\x63\xCC\x8C", u"\x0063\x030C", U"\x00000063\x0000030C");
    test("\xC4\xB3", u"\x0133", U"\x00000133");
    test("\x69\x6A", u"\x0069\x006A", U"\x00000069\x0000006A");
    test("\xCE\xA9", u"\x03A9", U"\x000003A9");
    test("\xE2\x84\xA6", u"\x2126", U"\x00002126");
    test("\xF0\x9D\x93\x83", u"\xD835\xDCC3", U"\x0001D4C3");
}

TEST(EncodingTests, Base64Encoding)
{
    EXPECT_EQ("", Base64Encode(""));
    EXPECT_EQ("Zg==", Base64Encode("f"));
    EXPECT_EQ("Zm8=", Base64Encode("fo"));
    EXPECT_EQ("Zm9v", Base64Encode("foo"));
    EXPECT_EQ("Zm9vYg==", Base64Encode("foob"));
    EXPECT_EQ("Zm9vYmE=", Base64Encode("fooba"));
    EXPECT_EQ("Zm9vYmFy", Base64Encode("foobar"));
    EXPECT_EQ("U2FtcGxlIEJhc2U2NCBlbmNvZGluZzogfmAnIiE/QCMkJV4mKigpe31bXTw+LC46Oy0rPV98L1w=",
              Base64Encode("Sample Base64 encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\"));
    EXPECT_EQ("Sample Base64 encoding: ~`'\"!?@#$%^&*(){}[]<>,.:;-+=_|/\\",
              Base64Decode(
                  "U2FtcGxlIEJhc2U2NCBlbmNvZGluZzogfmAnIiE/QCMkJV4mKigpe31bXTw+LC46Oy0rPV98L1w="));
}
