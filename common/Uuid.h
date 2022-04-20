/**************************************************************************
 * @file: Uuid.h
 * @brief:
 *
 * Copyright (c) 2022 PIQS Technologies.
 * All rights reserved.
*************************************************************************/

#pragma once

#include <uuid.h>

namespace
{
   // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0205r0.html
   template <typename EngineT, std::size_t StateSize = EngineT::state_size>
   void seed_rng(EngineT& engine)
   {
      using engine_type = typename EngineT::result_type;
      using device_type = std::random_device::result_type;
      using seedseq_type = std::seed_seq::result_type;
      constexpr auto bytes_needed = StateSize * sizeof(engine_type);
      constexpr auto numbers_needed = (sizeof(device_type) < sizeof(seedseq_type))
         ? (bytes_needed / sizeof(device_type))
         : (bytes_needed / sizeof(seedseq_type));
      std::array<device_type, numbers_needed> numbers{};
      std::random_device rnddev{};
      std::generate(std::begin(numbers), std::end(numbers), std::ref(rnddev));
      std::seed_seq seedseq(std::cbegin(numbers), std::cend(numbers));
      engine.seed(seedseq);
   }
}

namespace cppbase
{

class Uuid
{
public:
    static uuids::uuid Generate()
    {
        static auto engine = uuids::uuid_random_generator::engine_type{};
        seed_rng(engine);
        static auto generator = uuids::uuid_random_generator{engine};
        return generator();
    }
};
    
} // namespace name
