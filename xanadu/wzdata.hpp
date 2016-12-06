//

#pragma once

class WZImg;

// plain old data struct

struct WZData
{
	union
	{
		double dreal;
		int    ireal;
		char  *string;
		WZImg *img;
	};

	WZData       *parent;
	WZData       *children;
	char         *name;
	int           num;
	unsigned char type;
};
