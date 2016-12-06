//

#include "wzimg.hpp"
#include "wzcrypto.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

void img_parse(WZImg *img)
{
	img->file.seek(img->offset);

	unsigned char a = img->file.read<unsigned char>();
	if (a != 0x73)
	{
		return;
	}

	img->file.seek(img->offset + 1);
	img->file.seek(img->offset + 1);

	WZMain *wz_reader = World::get_instance()->wz_reader_;
	wz_reader->wz_crypto_.Key = wz_reader->wz_crypto_.Keys[1];
	wz_reader->wz_crypto_.AKey = wz_reader->wz_crypto_.AKeys[1];
	wz_reader->wz_crypto_.WKey = wz_reader->wz_crypto_.WKeys[1];

	char *s = img->file.read_enc_string();
	if (strcmp(s, "Property") != 0)
	{
		return;
	}

	unsigned short b = img->file.read<unsigned short>();
	if (b != 0)
	{
		return;
	}

	img_sub_property(img, img->n);

	img->n.resolve();
}

void img_extended_property(WZImg *img, WZNode n)
{
	char *name = img->file.read_prop_string(img->offset);
	if (strcmp(name, "Property") == 0)
	{
		img->file.skip(2);
		img_sub_property(img, n);
	}
	else if (strcmp(name, "Canvas") == 0)
	{
		img->file.skip(1);
		unsigned char b = img->file.read<unsigned char>();

		if (b == 1)
		{
			img->file.skip(2);
			img_sub_property(img, n);
		}
	}
	else if (strcmp(name, "Shape2D#Vector2D") == 0)
	{
		n.reserve(2);
		n.g("x", 0).set(img->file.read_value());
		n.g("y", 1).set(img->file.read_value());
	}
	else if (strcmp(name, "UOL") == 0)
	{
		img->file.skip(1);
		n.set_uol(img->file.read_prop_string(img->offset));
	}
}

void img_sub_property(WZImg *img, WZNode n)
{
	int count = img->file.read_value();
	n.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		char *name = img->file.read_prop_string(img->offset);
		unsigned char a = img->file.read<unsigned char>();

		switch (a)
		{
		case 0x00:
			n.g(name, i).set(i);
			break;

		case 0x0B:
		case 0x02:
			n.g(name, i).set(img->file.read<unsigned short>());
			break;

		case 0x03:
			n.g(name, i).set(img->file.read_value());
			break;

		case 0x04:
			if (img->file.read<unsigned char>() == 0x80)
			{
				n.g(name, i).set(img->file.read<float>());
			}
			else
			{
				n.g(name, i).set(0);
			}
			break;

		case 0x05:
			n.g(name, i).set(img->file.read<double>());
			break;

		case 0x08:
			n.g(name, i).set(img->file.read_prop_string(img->offset));
			break;

		case 0x09:
		{
			unsigned long long p = img->file.read<unsigned int>();

			p += img->file.wzfile_tell();

			img_extended_property(img, n.g(name, i));
			img->file.seek(p);

			break;
		}

		default:
			return;
		}
	}
}
