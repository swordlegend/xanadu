//

#pragma once

constexpr unsigned short kServerPort = 8484;

constexpr signed char kChannelsCount = 2;

constexpr int kExpRate = 5;
constexpr int kMesoRate = 5;
constexpr int kDropRate = 3;

constexpr auto kWorld1Name = "Scania";
constexpr auto kWorld1EventMessage = "";
constexpr signed char kWorld1Flag = 0;
constexpr auto kHeaderMessage = "";

constexpr int kMinKeymapPos = 0;
constexpr int kMaxKeymapPos = 90;
constexpr int kNoMap = 999999999;

constexpr long long kPermanentTime = 150841440000000000;
constexpr long long kNoExpirationTime = 150842304000000000;
constexpr long long kZeroTime = 94354848000000000;

constexpr signed char kCreateCharacterEquipSlotsTop = 5;
constexpr signed char kCreateCharacterEquipSlotsBottom = 6;
constexpr signed char kCreateCharacterEquipSlotsShoes = 7;
constexpr signed char kCreateCharacterEquipSlotsWeapon = 11;

constexpr int kNpcConstantsNo = 0;
constexpr int kNpcConstantsYes = 1;

constexpr signed char kInventoryConstantsTypesEquipped = 0;
constexpr signed char kInventoryConstantsTypesEquip = 1;
constexpr signed char kInventoryConstantsTypesUse = 2;
constexpr signed char kInventoryConstantsTypesSetup = 3;
constexpr signed char kInventoryConstantsTypesEtc = 4;
constexpr signed char kInventoryConstantsTypesCash = 5;

constexpr signed char kHiredMerchantConstantsSlotsMin = 1;
constexpr signed char kHiredMerchantConstantsSlotsMax = 3;

constexpr signed char kGenderConstantsMale = 0;
constexpr signed char kGenderConstantsFemale = 1;

constexpr short kItemConstantsFlagsNone = 0x00; // No Flag
constexpr short kItemConstantsFlagsLock = 0x01; // Disables dropping/trading of the item
constexpr short kItemConstantsFlagsSpikes = 0x02; // "Add Prevent Slipping", enables normal movement on ice maps (EL Nath etc.)
constexpr short kItemConstantsFlagsCold = 0x04; // "Add Prevent Coldness", prevents damage by coldness on ice maps (EL Nath etc.)
constexpr short kItemConstantsFlagsUntradeable = 0x08; // Disables trading of the item

constexpr signed char kItemConstantsEquippedSlotsTamingMob = -18;
constexpr signed char kItemConstantsEquippedSlotsTamingMobSaddle = -19;
constexpr signed char kItemConstantsEquippedSlotsMedal = -49;

constexpr int kCharacterStatsSkin = 0x1;
constexpr int kCharacterStatsFace = 0x2;
constexpr int kCharacterStatsHair = 0x4;
constexpr int kCharacterStatsPet1 = 0x8;
constexpr int kCharacterStatsLevel = 0x10;
constexpr int kCharacterStatsJob = 0x20;
constexpr int kCharacterStatsStr = 0x40;
constexpr int kCharacterStatsDex = 0x80;
constexpr int kCharacterStatsInt = 0x100;
constexpr int kCharacterStatsLuk = 0x200;
constexpr int kCharacterStatsHp = 0x400;
constexpr int kCharacterStatsMaxHp = 0x800;
constexpr int kCharacterStatsMp = 0x1000;
constexpr int kCharacterStatsMaxMp = 0x2000;
constexpr int kCharacterStatsAp = 0x4000;
constexpr int kCharacterStatsSp = 0x8000;
constexpr int kCharacterStatsExp = 0x10000;
constexpr int kCharacterStatsFame = 0x20000;
constexpr int kCharacterStatsMesos = 0x40000;
constexpr int kCharacterStatsPet2 = 0x80000;
constexpr int kCharacterStatsPet3 = 0x100000;
constexpr int kCharacterStatsPet = kCharacterStatsPet1 | kCharacterStatsPet2 | kCharacterStatsPet3;

constexpr short kPetLevels[29] = { 1, 3, 6, 14, 31, 60, 108, 181, 287, 434, 632, 891, 1224, 1642, 2161, 2793, 3557, 4467, 5542, 6801, 8263, 9950, 11882, 14084, 16578, 19391, 22548, 26074, 30000 };

constexpr int kCharacterStatsExpTable[199] =
{
	15, 34, 57, 92, 135, 372, 560, 840, 1242, 1144,
	1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, 13478,
	16268, 19320, 22881, 27009, 31478, 36601, 42446, 48722, 55816, 76560,
	86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, 235372,
	259392, 285532, 312928, 342624, 374760, 408336, 444544, 483532, 524160, 567772,
	598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917705, 967995,
	1021040, 1076993, 1136012, 1198265, 1263930, 1333193, 1406252, 1483314, 1564600, 1650340,
	1740778, 1836172, 1936794, 2042930, 2154882, 2272969, 2397528, 2528912, 2667496, 2813674,
	2967863, 3130501, 3302052, 3483004, 3673872, 3875200, 4087561, 4311559, 4547832, 4797052,
	5059931, 5337215, 5629694, 5938201, 6263614, 6606860, 6968915, 7350811, 7753635, 8178534,
	8626717, 9099461, 9598112, 10124088, 10678888, 11264090, 11881362, 12532460, 13219239, 13943652,
	14707764, 15513749, 16363902, 17260644, 18206527, 19204244, 20256636, 21366700, 22537594, 23772654,
	25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428272, 38424541, 40530206,
	42751261, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344, 69100311,
	72887008, 76881216, 81094306, 85538273, 90225770, 95170142, 100385465, 105886588, 111689173, 117809740,
	124265713, 131075474, 138258409, 145834970, 153826726, 162256430, 171148082, 180526996, 190419876, 200854884,
	211861732, 223471754, 235718006, 248635352, 262260569, 276632448, 291791906, 307782102, 324648561, 342439302,
	361204976, 380999008, 401877753, 423900654, 447130409, 471633156, 497478652, 524740482, 553496260, 583827855,
	615821621, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660769, 995373379,
	1049919840, 1107455447, 1168144005, 1232158296, 1299680571, 1370903066, 1446028554, 1525270918, 1608855764
};
