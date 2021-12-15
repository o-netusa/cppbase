/**************************************************************************
 * @file: Variant_any.h
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#include <any>

namespace internal {

struct Impl
{
    std::any m_val;
};
    
} // namespace
