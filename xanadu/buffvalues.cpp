//

#include "buffvalues.hpp"

#include <algorithm>

std::vector<Value> *Values::get_values()
{
	return &values_;
}

int Values::get_value_by_stat(unsigned long long stat)
{
	for (Value &value : values_)
	{
		if (value.get_stat() == stat)
		{
			return value.get_value();
		}
	}

	return 0;
}

// this operator overloading is needed for std::sort

bool operator<(const Value &x, const Value &y)
{
	return ((Value &)x).get_stat() < ((Value &)y).get_stat();
}

void Values::sort()
{
	std::sort(values_.begin(), values_.end());
}
