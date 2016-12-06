//

#include "buffvalue.hpp"

#include "constants.hpp"

// constructor

Value::Value(unsigned long long stat, int value) :
value_(value),
stat_(stat)
{
}

int Value::get_value()
{
	return value_;
}

unsigned long long Value::get_stat()
{
	return stat_;
}
