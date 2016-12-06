//

#pragma once

// plain old data struct

struct ItemData
{
	int id;
	int price;
	short max_per_slot;
	bool trade_able;
	bool is_cash;

	// item effect data
	bool recover;
	short hp;
	short mp;
	short watk;
	short matk;
	short wdef;
	short mdef;
	short acc;
	short avo;
	short jump;
	short speed;
	short hhp;
	short hmp;
	short hpr;
	short mpr;
	short success;
	short cursed;
	short str;
	short dex;
	short intt;
	short luk;
	int time;
	int move_to;
};
