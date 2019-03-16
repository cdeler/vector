//
// Created by cdeler on 3/15/19.
//

#include <random>
#include <memory>
#include <chrono>
#include <cassert>
#include <cstddef>

#include "vector.h"

namespace cdeler
{
	namespace vector
	{
		class RandomGenerator
		{
			std::default_random_engine generator;
			std::uniform_int_distribution<size_t> distribution;

		public:
			explicit RandomGenerator(const int s = 0) : generator{s}, distribution{0, 1}
				{}

			size_t get_next(size_t min, size_t max)
				{
				std::uniform_int_distribution<size_t>::param_type para{min, max};
				distribution.param(para);

				return distribution(generator);
				}
		};
	};
};

static std::unique_ptr<cdeler::vector::RandomGenerator> __generator = nullptr;

static void __attribute__((constructor, used))
__module_init()
	{
	using namespace cdeler::vector;
	using namespace std::chrono;

	const int seed = (int) system_clock::now().time_since_epoch().count();
	__generator = std::make_unique<RandomGenerator>(RandomGenerator(seed));
	}

#ifdef __cplusplus
extern "C"
{
#endif
void
vector_random_seed(const int seed)
	{
	using namespace cdeler::vector;

	if (__generator != nullptr)
		{
		__generator = nullptr;
		}

	__generator = std::make_unique<RandomGenerator>(RandomGenerator(seed));
	}


size_t
vector_random_get_next(size_t min, size_t max)
	{
	assert(__generator != nullptr);

	return __generator->get_next(min, max);
	}

#ifdef __cplusplus
};
#endif