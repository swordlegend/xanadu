//

#include "wznode.hpp"

#include "wzimg.hpp"
#include "wzcrypto.hpp"
#include "wzmain.hpp"
#include "wzdata.hpp"
#include "wzdata_constants.hpp"
#include "world.hpp"

// default constructor

WZNode::WZNode() :
	data(nullptr)
{
}

// copy constructor

WZNode::WZNode(const WZNode &other) :
	data(other.data)
{
}

WZNode::WZNode(WZData *data) :
	data(data)
{
}

WZNode::WZNode(WZData &data) :
	data(&data)
{
}

WZNode WZNode::operator[](const char *key) const
{
	if (!data)
	{
		return WZNode();
	}

	if (strcmp(key, "..") == 0)
	{
		return data->parent;
	}

	if (strcmp(key, ".") == 0)
	{
		return *this;
	}

	if (data->type == kWzDataTypesImg)
	{
		data->type = kWzDataTypesNone;
		img_parse(data->img);
	}

	if (!data->children)
	{
		return WZNode();
	}

	for (int i = 0; i < data->num; ++i)
	{
		if (strcmp(data->children[i].name, key) == 0)
		{
			return data->children[i];
		}
	}

	return WZNode();
}

WZNode &WZNode::operator++()
{
	++data;
	return *this;
}

bool WZNode::operator!=(const WZNode &other) const
{
	return data != other.data;
}

WZNode &WZNode::operator*()
{
	return *this;
}

WZNode WZNode::begin() const
{
	if (!data)
	{
		return nullptr;
	}
	if (data->type == kWzDataTypesImg)
	{
		data->type = kWzDataTypesNone;
		img_parse(data->img);
	}

	return data->children;
}

WZNode WZNode::end() const
{
	if (!data)
	{
		return nullptr;
	}
	if (data->type == kWzDataTypesImg)
	{
		data->type = kWzDataTypesNone;
		img_parse(data->img);
	}

	return data->children + data->num;
}

WZNode WZNode::g(char *key, int n)
{
	data->children[n].name = key;
	data->children[n].parent = this->data;

	return data->children[n];
}

std::string WZNode::name() const
{
	if (!data)
	{
		return std::string();
	}

	return data->name;
}

void WZNode::init_top(std::string s)
{
	size_t _Count1 = 1;
	size_t _Size1 = sizeof(WZData);
	void *memory1 = calloc(_Count1, _Size1);

	World::get_instance()->wz_reader_->wz_string_buffers_.push_back(memory1);

	data = (WZData *)memory1;

	size_t _Size2 = s.length() + 1;
	void *memory2 = malloc(_Size2);

	World::get_instance()->wz_reader_->wz_string_buffers_.push_back(memory2);

	data->name = (char*)memory2;

	strcpy(data->name, s.data());
	data->parent = this->data;
}

void WZNode::assign(const WZNode &other)
{
	data = other.data;
}

WZNode::operator bool() const
{
	return data == nullptr;
}

int WZNode::get_int_value()
{
	if (!data)
	{
		return 0;
	}

	if (data->type == kWzDataTypesIreal)
	{
		return data->ireal;
	}
	else if (data->type == kWzDataTypesDreal)
	{
		return static_cast<int>(data->dreal);
	}
	else
	{
		return 0;
	}
}

double WZNode::get_double_value()
{
	if (!data)
	{
		return 0;
	}

	if (data->type == kWzDataTypesIreal)
	{
		return data->ireal;
	}
	else if (data->type == kWzDataTypesDreal)
	{
		return data->dreal;
	}
	else
	{
		return 0;
	}
}
std::string WZNode::get_string_value()
{
	if (!data)
	{
		return std::string();
	}

	switch (data->type)
	{
	case kWzDataTypesIreal:
	{
		return std::to_string(data->ireal);
	}
	case kWzDataTypesDreal:
	{
		return std::to_string(data->dreal);
	}
	case kWzDataTypesString:
	{
		return data->string;
	}
	default:
	{
		return std::string();
	}
	}
}


void WZNode::set(char *v)
{
	if (!v)
	{
		v = World::get_instance()->wz_reader_->alloc_string(1);
	}

	data->string = v;
	data->type = kWzDataTypesString;
}

void WZNode::set_uol(char *v)
{
	data->string = v;
	data->type = kWzDataTypesUol;
}

void WZNode::set(double v)
{
	data->dreal = v;
	data->type = kWzDataTypesDreal;
}

void WZNode::set(int v)
{
	data->ireal = v;
	data->type = kWzDataTypesIreal;
}

void WZNode::set(WZImg *img)
{
	data->img = img;
	data->type = kWzDataTypesImg;
}

void WZNode::resolve()
{
	if (!data)
	{
		return;
	}

	if (data->type == kWzDataTypesUol)
	{
		char *s = data->string;
		static char *parts[10];
		int n = 1;
		char *it = s;
		parts[0] = s;

		while (*it != '\0')
		{
			if (*it == '/')
			{
				*it = '\0';
				parts[n] = it + 1;
				++n;
			}
			++it;
		}

		WZNode nn = data->parent;

		for (int i = 0; i < n; ++i)
		{
			if (!nn)
			{
				break;
			}

			nn = nn[(const char *)parts[i]];
		}
		if (nn)
		{
			data = nn.data;
		}
	}
	else
	{
		for (WZNode n : *this)
		{
			n.resolve();
		}
	}
}

void WZNode::reserve(int n)
{
	static WZData *d = nullptr;
	static std::size_t remain = 0;

	if (remain < n)
	{
		size_t _Count = 0x10000;
		size_t _Size = sizeof(WZData);

		void *memory = calloc(_Count, _Size);

		World::get_instance()->wz_reader_->wz_string_buffers_.push_back(memory);

		d = (WZData *)memory;
		remain = _Count;
	}

	data->children = d;
	remain -= n;
	d += n;
	data->num = n;
}
