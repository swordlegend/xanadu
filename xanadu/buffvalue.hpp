//

#pragma once

class Value
{
public:

	// constructor
	Value(unsigned long long stat, int value);

	// default constructor
	Value() = delete;

	// copy constructor
	Value(const Value &other) = default;

	// move constructor
	Value(Value &&other) = default;
	
	// destructor
	~Value() = default;

	// copy assignment operator
	Value &operator=(const Value &other) = default;

	// move assignment operator
	Value &operator=(Value &&other) = default;

	int get_value();
	unsigned long long get_stat();

private:

	int value_;
	unsigned long long stat_;
};
