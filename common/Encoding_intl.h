/**************************************************************************
 * @file:  Encoding_intl.h
 * @brief: Encoding utilities based on
 *         https://github.com/chronoxor/CppCommon/blob/master/include/string/encoding.h
 *
 * Copyright 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#include <algorithm>
#include <cassert>
#include <codecvt>
#include <locale>
#include <vector>

namespace cppbase { namespace encoding {

std::string ToUTF8(std::wstring_view wstr)
{
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes((char16_t*)wstr.data(), (char16_t*)wstr.data() + wstr.size());
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr.data(), wstr.data() + wstr.size());
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.to_bytes(wstr.data(), wstr.data() + wstr.size());
#endif
}

std::wstring FromUTF8(std::string_view str)
{
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::wstring(tmp.data(), tmp.data() + tmp.size());
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#elif defined(_WIN32) || defined(_WIN64)
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::u16string UTF8toUTF16(std::string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::u16string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::u32string UTF8toUTF32(std::string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(str.data(), str.data() + str.size());
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str.data(), str.data() + str.size());
#endif
}

std::string UTF16toUTF8(std::u16string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
    return convert.to_bytes((uint16_t*)str.data(), (uint16_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
    return convert.to_bytes(str.data(), str.data() + str.size());
#endif
}

std::u32string UTF16toUTF32(std::u16string_view str)
{
    std::string bytes;
    bytes.reserve(str.size() * 2);

    for (const char16_t ch : str)
    {
        bytes.push_back((uint8_t)(ch / 256));
        bytes.push_back((uint8_t)(ch % 256));
    }

#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    auto tmp = convert.from_bytes(bytes);
    return std::u32string(tmp.data(), tmp.data() + tmp.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    return convert.from_bytes(bytes);
#endif
}

std::string UTF32toUTF8(std::u32string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> convert;
    return convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str.data(), str.data() + str.size());
#endif
}

std::u16string UTF32toUTF16(std::u32string_view str)
{
#if defined(_MSC_VER)
    std::wstring_convert<std::codecvt_utf16<uint32_t>, uint32_t> convert;
    std::string bytes = convert.to_bytes((uint32_t*)str.data(), (uint32_t*)str.data() + str.size());
#else
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> convert;
    std::string bytes = convert.to_bytes(str.data(), str.data() + str.size());
#endif

    std::u16string result;
    result.reserve(bytes.size() / 2);

    for (size_t i = 0; i < bytes.size(); i += 2)
        result.push_back((char16_t)((uint8_t)(bytes[i]) * 256 + (uint8_t)(bytes[i + 1])));

    return result;
}

std::string Base64Encode(std::string_view str)
{
    const char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const size_t mods[] = { 0, 2, 1 };

    size_t ilength = str.length();
    size_t olength = 4 * ((ilength + 2) / 3);

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint32_t octet_a = i < ilength ? (uint8_t)str[i++] : 0;
        uint32_t octet_b = i < ilength ? (uint8_t)str[i++] : 0;
        uint32_t octet_c = i < ilength ? (uint8_t)str[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        result[j++] = base64[(triple >> 3 * 6) & 0x3F];
        result[j++] = base64[(triple >> 2 * 6) & 0x3F];
        result[j++] = base64[(triple >> 1 * 6) & 0x3F];
        result[j++] = base64[(triple >> 0 * 6) & 0x3F];
    }

    for (size_t i = 0; i < mods[ilength % 3]; ++i)
        result[result.size() - 1 - i] = '=';

    return result;
}

std::string Base64Decode(std::string_view str)
{
    static const unsigned char base64[256] =
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3f,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    size_t ilength = str.length();

    if (ilength % 4 != 0)
        return "";

    size_t olength = ilength / 4 * 3;

    if (str[ilength - 1] == '=') olength--;
    if (str[ilength - 2] == '=') olength--;

    std::string result;
    result.resize(olength, 0);

    for (size_t i = 0, j = 0; i < ilength;)
    {
        uint32_t sextet_a = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_b = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_c = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];
        uint32_t sextet_d = str[i] == '=' ? 0 & i++ : base64[(uint8_t)str[i++]];

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < olength) result[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < olength) result[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < olength) result[j++] = (triple >> 0 * 8) & 0xFF;

    }

    return result;
}

}}