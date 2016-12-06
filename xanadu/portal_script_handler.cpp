//

#include "player.hpp"

#include "map.hpp"
#include "packetcreator.hpp"
#include "map_portal_data.hpp"
#include "map_data_provider.hpp"
#include "map_data.hpp"

void Player::handle_use_scripted_portal()
{
	skip_bytes(1);
	std::string portal_name = read<std::string>();

	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(map_->get_id());
	MapPortalData *portal = data->get_portal(portal_name);
	if (!portal)
	{
		return;
	}

	std::string script_name = portal->get_script();

	if (script_name == "hontale_morph") // Horntail entrance map
	{
		// this script is called too often by the client -> do nothing
		return;
	}
	else if (script_name == "advice00")
	{
		// beginner maps
		return;
	}
	else if (script_name == "alian_in")
	{
		set_map(600010300);
	}
	else if (script_name == "alian_out_1")
	{
		set_map(600010200);
	}
	else if (script_name == "EnterSWmer")
	{
		set_map(105000000);
	}
	else if (script_name == "gotoNext1")
	{
		set_map(211060300);
	}
	else if (script_name == "gotoNext2_1")
	{
		set_map(211060500);
	}
	else if (script_name == "gotoNext3_1")
	{
		set_map(211060700);
	}
	else if (script_name == "gotoNext4")
	{
		set_map(211060800);
	}
	else if (script_name == "halloween_enter")
	{
		set_map(682000100);
	}
	else if (script_name == "in_guild")
	{
		set_map(200000301);
	}
	else if (script_name == "out_guild")
	{
		set_map(200000300);
	}
	else if (script_name == "LudiElevator_in")
	{
		if (map_->get_id() == 222020200)
		{
			set_map(222020100);
		}
		else if (map_->get_id() == 222020100)
		{
			set_map(222020200);
		}
	}
	else if (script_name == "market00")
	{
		set_map(100000000);
	}
	else if (script_name == "market01")
	{
		set_map(910000000);
	}
	else if (script_name == "market02")
	{
		set_map(910000000);
	}
	else if (script_name == "market03")
	{
		set_map(910000000);
	}
	else if (script_name == "market04")
	{
		set_map(910000000);
	}
	else if (script_name == "market05")
	{
		set_map(910000000);
	}
	else if (script_name == "market06")
	{
		set_map(910000000);
	}
	else if (script_name == "market07")
	{
		set_map(910000000);
	}
	else if (script_name == "market08")
	{
		set_map(910000000);
	}
	else if (script_name == "market09")
	{
		set_map(910000000);
	}
	else if (script_name == "market10")
	{
		set_map(910000000);
	}
	else if (script_name == "market11")
	{
		set_map(910000000);
	}
	else if (script_name == "market12")
	{
		set_map(910000000);
	}
	else if (script_name == "market13")
	{
		set_map(910000000);
	}
	else if (script_name == "market14")
	{
		set_map(910000000);
	}
	else if (script_name == "market15")
	{
		set_map(910000000);
	}
	else if (script_name == "market16")
	{
		set_map(910000000);
	}
	else if (script_name == "market17")
	{
		set_map(910000000);
	}
	else if (script_name == "market18")
	{
		set_map(910000000);
	}
	else if (script_name == "market19")
	{
		set_map(910000000);
	}
	else if (script_name == "market20")
	{
		set_map(910000000);
	}
	else if (script_name == "market21")
	{
		set_map(910000000);
	}
	else if (script_name == "market22")
	{
		set_map(910000000);
	}
	else if (script_name == "market23")
	{
		set_map(910000000);
	}
	else if (script_name == "subway_in2")
	{
		set_map(103020100);
	}
	else if (script_name == "Zakumgo") // The Cave of Trial III - The Door to Zakum
	{
		set_map(211042300);
	}
	else if (script_name == "Zakum05") // The Door to Zakum - Entrance to Zakum Altar
	{
		set_map(211042400);
	}
	else if (script_name == "Populatus00") // Papulatus Entrance
	{
		set_map(220080001);
	}
	else if (script_name == "bigban")
	{
		set_map(600020000);
	}
	else if (script_name == "Pianus")
	{
		set_map(230040420);
	}
	else if (script_name == "enterAchter")
	{
		set_map(100000201);
	}
	else if (script_name == "outArchterMap")
	{
		set_map(100000000);
	}
	else if (script_name == "dracoout")
	{
		set_map(240000100);
	}
	else if (script_name == "enterBlackMine")
	{
		set_map(310040200);
	}
	else
	{
		// no handler for the portal script name
	
		// log the information
		char destination[100] = "logs//\0";
		strcat(destination, "script_engine.txt");

		FILE *file = fopen(destination, "a");
		if (file != nullptr)
		{
			char text[100] = "";
			sprintf(text, "scripted portal: %s missing in map: %d", script_name.c_str(), map_->get_id());
			fprintf(file, "%s", text);
			fprintf(file, "\n");
			fclose(file);
		}

		// send this packet to prevent the client from blocking itself
		PacketCreator packet;
		packet.EnableAction();
		send_packet(&packet);
	}
}
