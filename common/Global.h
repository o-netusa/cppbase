/**************************************************************************
 * @file:  Global.h
 * @brief:
 *
 * Copyright(c) 2020-present O-Net Communications (ShenZhen) Limited.
 * This code is licensed under MIT license (see LICENSE for details)
 *
 *************************************************************************/

#pragma once

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT __attribute__((visibility("default")))
#  define DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(ONET_LIBRARY)
#  define DLLEXPORT DECL_EXPORT
#else
#  define DLLEXPORT DECL_IMPORT
#endif

#define DISALLOW_COPY_AND_ASSIGN(T) \
  T(const T&) = delete;   \
  T& operator=(const T&) = delete; \
  T(const T&&)= delete; \
  T& operator=(const T&&) = delete
