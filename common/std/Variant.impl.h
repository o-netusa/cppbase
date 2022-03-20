/**************************************************************************
 * @file: Variant.impl
 * @brief:
 *
 * Copyright (c) 2021 O-Net Technologies (Group) Limited.
 * All rights reserved.
 *************************************************************************/

#include <any>

namespace cppbase { namespace internal {

struct VariantImpl
{
    std::any m_val;
};
    
}} // namespace cppbase::internal
