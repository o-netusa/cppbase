/**************************************************************************
 * @file:  Encoding.h
 * @brief: Encoding utilities based on
 *         https://github.com/chronoxor/CppCommon/blob/master/include/string/encoding.h
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#include <string>

namespace cppbase { namespace encoding {

//! Convert system wide-string to UTF-8 encoded string
/*!
    System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

    \param wstr - System wide-string to convert
    \return UTF-8 encoded string
*/
std::string ToUTF8(std::wstring_view wstr);

//! Convert UTF-8 encoded string to system wide-string
/*!
    System wide-string could be UTF-16 (Windows) or UTF-32 (Unix).

    \param str - UTF-8 encoded string to convert
    \return System wide-string
*/
std::wstring FromUTF8(std::string_view str);

//! Convert UTF-8 encoded string to UTF-16 encoded string
/*!
    \param str - UTF-8 encoded string to convert
    \return UTF-16 encoded string
*/
std::u16string UTF8toUTF16(std::string_view str);
//! Convert UTF-8 encoded string to UTF-32 encoded string
/*!
    \param str - UTF-8 encoded string to convert
    \return UTF-32 encoded string
*/
std::u32string UTF8toUTF32(std::string_view str);

//! Convert UTF-16 encoded string to UTF-8 encoded string
/*!
    \param str - UTF-16 encoded string to convert
    \return UTF-8 encoded string
*/
std::string UTF16toUTF8(std::u16string_view str);
//! Convert UTF-16 encoded string to UTF-32 encoded string
/*!
    \param str - UTF-16 encoded string to convert
    \return UTF-32 encoded string
*/
std::u32string UTF16toUTF32(std::u16string_view str);

//! Convert UTF-32 encoded string to UTF-8 encoded string
/*!
    \param str - UTF-32 encoded string to convert
    \return UTF-8 encoded string
*/
std::string UTF32toUTF8(std::u32string_view str);
//! Convert UTF-32 encoded string to UTF-16 encoded string
/*!
    \param str - UTF-32 encoded string to convert
    \return UTF-16 encoded string
*/
std::u16string UTF32toUTF16(std::u32string_view str);

//! Base64 encode string
/*!
    \param str - String to encode
    \return Base64 encoded string
*/
std::string Base64Encode(std::string_view str);
//! Base64 decode string
/*!
    \param str - Base64 encoded string
    \return Decoded string
*/
std::string Base64Decode(std::string_view str);

}}

#include "Encoding_intl.h"
