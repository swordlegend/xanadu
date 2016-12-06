//

#include "map_data_provider.hpp"

#include "map.hpp"
#include "map_data.hpp"
#include "mapnpc.hpp"
#include "map_mob_data.hpp"
#include "map_foothold_data.hpp"
#include "map_reactor_data.hpp"
#include "map_portal_data.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

// singleton

MapDataProvider *MapDataProvider::singleton_ = nullptr;

MapDataProvider *MapDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new MapDataProvider();
	}

	return singleton_;
}

// default constructor

MapDataProvider::MapDataProvider() :
	npc_ids_(0)
{
}

void MapDataProvider::load_data()
{
	// this shows how to use the loader

	// WZ::Node nn = wz_reader->Base[wzfilename][.img name][name of value][(sometimes only) sub names of value][value (int/string or so return)]

	// Map.wz contains map data
	// sub value Map
	// sub value Map0, Map1,  .... Map9
	// <mapid>.img
	// multiple subtypes: foothold, life, ...
	// return values then...

	// get the maptype used to navigate in the map wz data

	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Map"]["Map"];

	for (auto it : node1)
	{
		std::string valuename = it.name();

		if (valuename == "AreaCode")
		{
			continue;
		}

		WZNode node2 = wz_reader->base_["Map"]["Map"][valuename.c_str()];

		for (auto it2 : node2)
		{
			std::string propertyname = it2.name();
			int id_ = atoi(propertyname.c_str());

			int maptype = 0;

			if (id_ < 100000000)
			{
				maptype = 0;
			}
			else if (id_ >= 100000000 && id_ <= 180000000)
			{
				maptype = 1;
			}
			else if (id_ >= 200000000 && id_ <= 280090000)
			{
				maptype = 2;
			}
			else if (id_ >= 540000000 && id_ <= 541010110)
			{
				maptype = 5;
			}
			else if (id_ >= 600000000 && id_ <= 682000900)
			{
				maptype = 6;
			}
			else if (id_ >= 800000000 && id_ <= 809050017)
			{
				maptype = 8;
			}
			else if (id_ >= 900000000 && id_ <= 990001101)
			{
				maptype = 9;
			}

			std::string maptype2 = "Map" + std::to_string(maptype); // Map0, Map1... Map9

			WZMain *wz_reader = World::get_instance()->wz_reader_;

			WZNode node1 = wz_reader->base_["Map"]["Map"][maptype2.c_str()];

			for (auto it : node1)
			{
				std::string id3 = it.name();

				int id4 = std::stoi(id3);

				if (id4 != id_)
				{
					continue;
				}

				MapData *data = new MapData(id4);

				maps_[id4] = data;

				WZNode node2 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()];

				for (auto it2 : node2)
				{

					std::string valuename = it2.name();

					if (valuename == "info")
					{
						// load general map info

						WZNode node3 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["info"];

						for (auto it3 : node3)
						{
							std::string valuename3 = it3.name();

							if (valuename3 == "forcedReturn")
							{
								int forced_return = it3.get_int_value();
								data->set_forced_return(forced_return);
							}

							if (valuename3 == "returnMap")
							{
								int return_map = it3.get_int_value();
								data->set_return_map(return_map);
							}

							if (valuename3 == "mobRate")
							{
								double mob_rate = it3.get_double_value();
								data->set_mob_rate(mob_rate);
							}
						}
					}

					else if (valuename == "life")
					{
						// load map life

						WZNode node3 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["life"];

						for (auto it3 : node3)
						{
							std::string valuename2 = it3.name();

							bool flip = false;
							int life_id = 0;
							short x = 0;
							short y = 0;
							short cy = 0;
							short rx0 = 0;
							short rx1 = 0;
							short foothold = 0;
							std::string life_type = "invalid";

							WZNode node4 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["life"][valuename2.c_str()];

							for (auto it4 : node4)
							{
								std::string propertyname = it4.name();

								if (propertyname == "type")
								{
									life_type = it4.get_string_value();
								}

								else if (propertyname == "id")
								{
									std::string life_id_s = it4.get_string_value();
									life_id = std::stoi(life_id_s);
								}

								else if (propertyname == "x")
								{
									x = it4.get_int_value();
								}

								else if (propertyname == "y")
								{
									y = it4.get_int_value();
								}

								else if (propertyname == "cy")
								{
									cy = it4.get_int_value();
								}

								else if (propertyname == "rx0")
								{
									rx0 = it4.get_int_value();
								}

								else if (propertyname == "rx1")
								{
									rx1 = it4.get_int_value();
								}

								else if (propertyname == "fh")
								{
									foothold = it4.get_int_value();
								}
							}

							if (life_type == "invalid")
							{
								continue;
							}

							else if (life_type == "n")
							{
								// load npcs

								int object_id = ++npc_ids_;

								Npc *npc = new Npc(flip, x, cy, rx0, rx1, foothold, life_id, object_id);

								data->add_npc(npc);
							}

							else if (life_type == "m")
							{
								// load mobs

								MapMobData *mob = new MapMobData();

								mob->mob_id = life_id;
								mob->flip = flip;
								mob->position_x = x;
								mob->position_y = cy;
								mob->foothold = foothold;

								data->add_mob(mob);
							}

						}
					}

					else if (valuename == "portal")
					{
						// load portals

						WZNode node3 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["portal"];

						for (auto it3 : node3)
						{
							std::string valuename2 = it3.name();

							int portal_id = std::stoi(valuename2);
							std::string name = "";
							int x = 0;
							int y = 0;
							int to_map = 0;
							int type = 0;
							std::string to_portal = "";
							std::string script = "";

							WZNode node4 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["portal"][valuename2.c_str()];

							for (auto it4 : node4)
							{
								std::string propertyname = it4.name();

								if (propertyname == "x")
								{
									x = it4.get_int_value();
								}

								else if (propertyname == "y")
								{
									y = it4.get_int_value();
								}

								else if (propertyname == "script")
								{
									script = it4.get_string_value();
								}

								else if (propertyname == "pn")
								{
									name = it4.get_string_value();
								}

								else if (propertyname == "pt")
								{
									type = it4.get_int_value();
								}

								else if (propertyname == "tn")
								{
									to_portal = it4.get_string_value();
								}

								else if (propertyname == "tm")
								{
									to_map = it4.get_int_value();
								}
							}

							MapPortalData *portal = new MapPortalData(portal_id, x, y, to_map, type, name, to_portal, script);

							data->add_portal(portal, name);
						}
					}

					else if (valuename == "foothold")
					{
						// load map foothold data

						WZNode node3 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["foothold"];

						for (auto it3 : node3)
						{
							std::string valuename2 = it3.name();

							WZNode node4 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["foothold"][valuename2.c_str()];

							for (auto it4 : node4)
							{
								std::string valuename3 = it4.name();

								WZNode node5 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["foothold"][valuename2.c_str()][valuename3.c_str()];

								for (auto it5 : node5)
								{
									std::string foothold_id_s = it5.name();

									int foothold_id = std::stoi(foothold_id_s);
									short x1 = 0;
									short x2 = 0;
									short y1 = 0;
									short y2 = 0;

									WZNode node6 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["foothold"][valuename2.c_str()][valuename3.c_str()][foothold_id_s.c_str()];

									for (auto it6 : node6)
									{
										std::string property_name = it6.name();

										if (property_name == "x1")
										{
											x1 = it6.get_int_value();
										}

										else if (property_name == "x2")
										{
											x2 = it6.get_int_value();
										}

										else if (property_name == "y1")
										{
											y1 = it6.get_int_value();
										}

										else if (property_name == "y2")
										{
											y2 = it6.get_int_value();
										}
									}

									MapFootholdData *foothold = new MapFootholdData();

									foothold->id = foothold_id;
									foothold->x1 = x1;
									foothold->x2 = x2;
									foothold->y1 = y1;
									foothold->y2 = y2;

									data->add_foothold(foothold);
								}
							}
						}
					}

					else if (valuename == "reactor")
					{
						// load reactors

						WZNode node3 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["reactor"];

						for (auto it3 : node3)
						{
							std::string valuename2 = it3.name();

							bool flip = false;
							int reactor_id = 0;
							short x = 0;
							short y = 0;

							WZNode node4 = wz_reader->base_["Map"]["Map"][maptype2.c_str()][id3.c_str()]["reactor"][valuename2.c_str()];

							for (auto it4 : node4)
							{
								std::string propertyname = it4.name();

								if (propertyname == "id")
								{
									std::string life_id_s = it4.get_string_value();
									reactor_id = std::stoi(life_id_s);
								}

								else if (propertyname == "x")
								{
									x = it4.get_int_value();
								}

								else if (propertyname == "y")
								{
									y = it4.get_int_value();
								}
							}

							MapReactorData *reactor = new MapReactorData();

							reactor->reactor_id = reactor_id;
							reactor->position_x = x;
							reactor->position_y = y;
							reactor->stance = flip;
							reactor->state = 0;

							data->add_reactor(reactor);
						}
					}
				}
			}
		}
	}
}

MapData *MapDataProvider::get_map_data_by_id(int id)
{
	if (maps_.find(id) != maps_.end())
	{
		return maps_[id];
	}

	return nullptr;
}

std::unordered_map<int, MapData *> *MapDataProvider::get_data()
{
	return &maps_;
}
