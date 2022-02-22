/**************************************************************************
 * @file: PointCloud.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#pragma once

#include <common/Global.h>

#include <Eigen/Core>
#include <vector>

namespace onet { namespace lidar {

struct PointXYZI : public Eigen::Vector4f
{
    PointXYZI() : Eigen::Vector4f(0.f, 0.f, 0.f, 0.f) {}

    PointXYZI(float x, float y, float z, float i) : Eigen::Vector4f(x, y, z, i) {}
};

struct PointXYZRGB : public PointXYZI
{
    Eigen::Vector3f color{0.f, 0.f, 0.f};  // r, g, b

    PointXYZRGB() : PointXYZI() {}

    PointXYZRGB(float x, float y, float z, float r, float g, float b)
        : PointXYZI(x, y, z, 1), color(r, g, b)
    {}
};

/**
 * @brief Point cloud bounding box represented by min and max vertices
 */
struct BoundingBox
{
    Eigen::Vector3d min_vertex;
    Eigen::Vector3d max_vertex;
};

template <typename POINT_T>
struct PointCloud : public std::vector<POINT_T>
{
    PointCloud() = default;

    PointCloud(uint32_t size) : std::vector<POINT_T>(size) {}
};

}}  // namespace onet::lidar
