/**************************************************************************
 * @file:  UuidTests.cpp
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 **************************************************************************/

#include <gtest/gtest.h>

#include <Eigen/Core>

TEST(EigenTests, MultiTypeVectors)
{
    Eigen::Vector4f v4f(12.1, 12.2, 12.3, 12.4);
    Eigen::Vector3i v3i(0, -1, 2);
    Eigen::Vector2d v2d(1.0, 1.5);
}
