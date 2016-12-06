//

#pragma once

#include <string>

class WZImg;
struct WZData;

struct WZNode
{
public:

	// default constructor
	WZNode();

	// copy constructor
	WZNode(const WZNode &other);

	// move constructor
	// CRITICAL
	// causes an access violation when compiling with Visual Studio 2015 Community
	//WZNode(WZNode &&other) = default;

	// destructor
	~WZNode() = default;

	// copy assignment operator
	WZNode &operator=(const WZNode &other) = default;

	// move assignment operator
	WZNode &operator=(WZNode &&other) = default;

	WZNode operator[](const char *) const;
	operator bool() const;
	WZNode &operator++();
	bool operator!=(const WZNode &) const;
	WZNode &operator*();

	WZNode begin() const;
	WZNode end() const;
	std::string name() const;
	int get_int_value();
	double get_double_value();
	std::string get_string_value();

	// functions/constructors for internal use only

	WZNode(WZData *);
	WZNode(WZData &);
	void init_top(std::string);
	void assign(const WZNode &);
	void set(char *);
	void set_uol(char *);
	void set(double);
	void set(int);
	void set(WZImg *);
	void resolve();
	void reserve(int);
	WZNode g(char *, int n);

private:

	WZData *data;
};
