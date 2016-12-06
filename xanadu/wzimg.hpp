//

#pragma once

#include "wzfile.hpp"
#include "wznode.hpp"

class WZImg
{
public:

	WZNode n;
	WZFile file;
	unsigned int size;
	unsigned int offset;
};

void img_parse(WZImg *img);
void img_extended_property(WZImg *img, WZNode n);
void img_sub_property(WZImg *img, WZNode n);
