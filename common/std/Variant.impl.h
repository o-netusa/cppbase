/**************************************************************************
 * @file: Variant.impl
 * @brief:
 *
 * Copyright (c) 2022 O-Net Communications Inc.
 * All rights reserved.
 *************************************************************************/

#include <any>

namespace cppbase { namespace internal {

struct VariantImpl
{
    std::any m_val;
};
    
}} // namespace cppbase::internal
