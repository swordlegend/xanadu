//

#pragma once

#include <functional>

namespace timer
{
	enum Types
	{
		kTimerTypesBuffTimer,
		kTimerTypesCoolDownTimer
	};

	struct Id
	{
		Id(int type, int id):
			type(type),
			id(id)
		{
		}

		int type;
		int id;

		bool operator==(Id const &other) const
		{
			return type == other.type && id == other.id;
		}
	};
}

// hash_combine function invented by boost.functional library,
// this is a workaround to avoid needing wohle boost library at the moment
// the code used here is not indentical to the original and not guaranteed to work,
// so using original boost might be better, maybe along with switching to using asio from boost collection
// instead of standalone

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

namespace std
{
	template <>
	struct hash<timer::Id>
	{
		size_t operator()(const timer::Id &v) const
		{
			size_t seed = 0;

			hash_combine(seed, v.type);
			hash_combine(seed, v.id);

			//boost::hash_combine(seed, v.type);
			//boost::hash_combine(seed, v.id);

			return seed;
		}
	};
}
