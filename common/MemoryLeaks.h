/**************************************************************************
 * @file:  MemoryLeaks.h
 * @brief: Include this file to detect memory leaks for MSVC builds.
 *         See details at https://github.com/KindDragon/vld
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 *************************************************************************/

#pragma once

#if defined(__clang__)
#pragma clang system_header
#elif defined(__GNUC__)
#pragma GCC system_header
#elif defined(_MSC_VER)
#pragma system_header
#endif

#if defined(_MSC_VER)
// #define VLD_FORCE_ENABLE
// #include <vld.h>
#endif

// See tests/common/MemoryLeaksTests.cpp for examples
