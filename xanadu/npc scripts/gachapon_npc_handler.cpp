//

#include "../player.hpp"
#include "../packetcreator.hpp"
#include "../constants.hpp"
#include "../tools.hpp"

void Player::handle_gachapon_npc()
{
	int items[] =
	{
		
		1002033, // old wisconsin
		1002584, // red wisconsin
		1002585, // blue wisconsin
		1002586, // purple wisconsin
		1002587, // black wisconsin
		1422005, // golden mole
		1402037, // stonetooth sword
		1402012, // doombringer
		1432008, // fish spear
		1432010, // omega spear
		1432015, // red ski
		1432016, // orange ski
		1432017, // green ski
		1432018, // sky ski
		1442043, // eclipse
		1422026, // the morningstar
		1452014, // golden arund
		1462012, // golden raven
		1472027, // green scarab
		1482010, // steelno
		1492009, // abyss shooter
		1322027, // frying pan
		1322025, // emergency tube
		1322048, // tamus
		1332027, // varkit
		1332029, // liu bei dagger
		1332053, // kebob
		1372017, // streetlight
		1372031, // heart staff
		1372035, // elemental wand 1
		1372036, // elemental wand 2
		1372037, // elemental wand 3
		1372038, // elemental wand 4
		1372039, // elemental wand 5
		1372040, // elemental wand 6
		1372041, // elemental wand 7
		1372042, // elemental wand 8
		1382014, // sun quai staff
		1382015, // poison mushroom
		1382016, // pyogo mushroom
		1382037, // doomsday staff
		1382045, // elemental staff 1
		1382046, // elemental staff 2
		1382047, // elemental staff 3
		1382048, // elemental staff 4
		1382049, // elemental staff 5
		1382050, // elemental staff 6
		1382051, // elemental staff 7
		1382052, // elemental staff 8
		1472073, // night raven's claw
		1472074, // dawn raven's claw
		1472075, // dusk raven's claw
		1702030, // diao chan sword
		1702031, // liu-bei sword
		1702032, // zhu-ge-liang wand
		1702034, // guan yu spear
		1702035, // cao cao bow
		1302049, // glowing whip
		1302037, // trumpet
		1302013, // red whip
		1302021, // pico-pico hammer
		1302024, // newspaper sword
		1302017, // sky blue umbrella
		1302025, // red umbrella
		1302026, // black umbrella
		1302027, // green umbrella 
		1302028, // light purple umbrella
		1302029, // beige umbrella
		1322051, // fruity bamboo
		1302106, // crystal blade
		1302095, // lifeguard saver
		1302088, // stirge-on-a-string
		1302089, // stirge-on-a-rope
		1302090, // stirge-o-whip
		1302091, // stirge grappler 
		1302092, // swooping stirge
		1302093, // frantic stirge
		1302094, // angry stirge
		1322012, // red brick
		1012106, // rat mouth
		1012108, // smiling mask
		1012109, // crying mask
		1012110, // angry mask
		1012111, // sad mask
		1022058, // raccoon mask
		1022060, // white raccoon mask
		1032008, // cat's eye
		1032028, // red emerald earrings
		1032026, // gold emerald earrings
		1032027, // black emerald earrings
		1072261, // yellow strap shoes
		1072262, // black strap shoes
		1072263, // green strap shoes
		1072264, // silver strap shoes
		1072238, // violet snowshoes
		1072239, // yellow snowshoes
		1072344, // facestompers
		1092008, // pan lid
		1092022, // palette
		1092021, // mystic shield
		1092029, // esther shield
		1092018, // seclusion wristguard
		1092019, // nimble wristguard
		1092020, // jurgen wristguard
		1092050, // khanjar
		1092049, // dragon khanjar
		1092052, // black phoenix shield
		1092061, // crossheider
		1050100, // bathrobe for men
		1051098, // bathrobe for women
		1051017, // red sauna robe
		1050018, // blue sauna robe
		1050067, // blue requiem
		1050047, // orange calass
		1050102, // green varuna
		1051010, // dark engrit
		1051004, // purple doroness robe
		1051041, // red choro
		1102021, // blue gaia cape
		1102022, // red gaia cape
		1102023, // white gaia cape
		1102024, // black gaia cape
		1102061, // oxygen tank
		1102079, // ragged red cape
		1102080, // ragged blue cape
		1102080, // ragged yellow cape
		1102080, // ragged black cape
		1102080, // ragged red cape
		1102084, // pink gaia cape
		1102085, // yellow gaia cape
		1102086, // purple gaia cape
		1102040, // yellow adventurer cape
		1102041, // pink adventurer cape
		1102042, // purple adventurer cape
		1102043, // brown adventurer cape
		1082002, // work gloves
		1082145, // yellow work gloves
		1082146, // red work gloves
		1082147, // blue work gloves
		1082148, // purple work gloves
		1082149, // brown work gloves
		1082150, // grey work gloves
		1082174, // lunar gloves
		1082174, // red marker
		1082174, // blue marker
		1082174, // purple marker 
		1082174, // pink marker
		1082174, // yellow marker
		1082222, // mechanical gloves
		1082223, // stormcaster gloves
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2022002, // cider
		2022245, // heartstopper
		2022251, // maple pop
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2000000, // red potion
		2000001, // orange potion
		2000002, // white potion
		2000003, // blue potion
		2000004, // elixir
		2000005, // power elixir
		2000006, // mana elixir
		2020008, // fat sausage
		2020012, // melting cheese
		2020013, // reindeer milk
		2020014, // sunrise dew
		2020015, // sunset dew
		2022002, // cider
		2022245, // heartstopper
		2022251, // maple pop
		2022282, // naricain's demon elixir
		2022283, // subani's mystic cauldron
		2022345, // the energizer drink
		2040000, // scroll for helmet for dex
		2040004, // scroll for helmet for hp
		2040012, // helmet for int 70%
		2040013, // helmet for int 30%
		2040014, // helmet for acc 70%
		2040015, // helmet for acc 30%
		2040025, // helmet for int 60%
		2040028, // helmet for dex 70%
		2040029, // helmet for dex 60%
		2040030, // helmet for dex 30%
		2040031, // helmet for dex 10%
		2040105, // scroll for face accessory for avoidability
		2040201, // scroll for eye accessory for accuracy
		2040206, // scroll for eye accessory for int
		2040301, // scroll for earring for int
		2040302, // scroll for earring for int
		2040304, // dark scroll for earring for int
		2040305, // dark scroll for earring for int
		2040306, // dark scroll for earring for dex
		2040307, // dark scroll for earring for dex
		2040310, // scroll for earring for def
		2040405, // dark scroll for topwear for def
		2040410, // dark scroll for topwear for luk
		2040411, // dark scroll for topwear for luk
		2040427, // scroll for topwear for luk 10%
		2040413, // scroll for topwear for luk
		2040418, // scroll for topwear for str 60%
		2040419, // scroll for topwear for str 10%
		2040501, // scroll for overall armor for dex
		2040504, // scroll for overall armor for def
		2040513, // scroll for overall armor for int 
		2040518, // dark scroll for overall armor for int
		2040519, // dark scroll for overall armor for int
		2040520, // dark scroll for overall armor for luk
		2040521, // dark scroll for overall armor for luk
		2040532, // scroll for overall armor for str 60%
		2040533, // scroll for overall armor for str 30%
		2040534, // scroll for overall armor for str 10%
		2040606, // dark scroll for bottomwear for jump
		2040607, // dark scroll for bottomwear for jump
		2040624, // scroll for bottomwear for dex 70%
		2040625, // scroll for bottomwear for dex 60%
		2040626, // scroll for bottomwear for dex 30%
		2040627, // scroll for bottomwear for dex 10%
		2040712, // dark scroll for shoes for dex
		2040713, // dark scroll for shoes for dex
		2040714, // dark scroll for shoes for jump
		2040715, // dark scroll for shoes for jump
		2040716, // dark scroll for shoes for speed
		2040717, // dark scroll for shoes for speed
		2040727, // scroll for spikes on shoes 10%
		2040801, // scroll for gloves for dex
		2040803, // scroll for gloves for att
		2040804, // scroll for gloves for att
		2040805, // scroll for gloves for att
		2040810, // dark scroll for gloves for att
		2040811, // dark scroll for gloves for att
		2040814, // dark scroll for gloves for m.att
		2040815, // dark scroll for gloves for m.att
		2040816, // scroll for gloves for m.att
		2040817, // scroll for gloves for m.att
		2040818, // scroll for gloves for m.att
		2040902, // scroll for shield for def
		2040906, // dark scroll for shield for luk
		2040907, // dark scroll for shield for luk
		2040914, // scroll for shield for w.att
		2040915, // scroll for shield for w.att
		2040916, // dark scroll for shield for w.att
		2040917, // dark scroll for shield for w.att
		2040918, // scroll for shield for m.att
		2040919, // scroll for shield for m.att
		2040920, // scroll for shield for m.att
		2040921, // dark scroll for shield for m.att
		2040922, // dark scroll for shield for m.att
		2040931, // scroll for shield for str 60%
		2040933, // scroll for shield for str 10%
		2041034, // dark scroll for cape for str
		2041035, // dark scorll for cape for str
		2041036, // dark scorll for cape for int
		2041037, // dark scorll for cape for int
		2041038, // dark scorll for cape for dex
		2041039, // dark scorll for cape for dex 
		2041040, // dark scorll for cape for luk
		2041041, // dark scorll for cape for luk
		2041050, // scroll for cape for str
		2041051, // scroll for cape for str
		2041052, // scroll for cape for int
		2041053, // scroll for cape for int
		2041054, // scroll for cape for dex
		2041055, // scroll for cape for dex
		2041056, // scroll for cape for luk
		2041057, // scroll for cape for luk
		2043001, // scroll for one-handed sword for att
		2043005, // dark scroll for one-handed sword for att
		2043011, // scroll for one-handed sword for att
		2043201, // scroll for one-handed bw for att
		2043204, // dark scroll for one-handed bw for att
		2043301, // scroll for dagger for att
		2043304, // dark scroll for dagger for att
		2043701, // scroll for wand for m.att
		2043704, // dark scroll for wand for m.att
		2043801, // scroll for staff for m.att
		2043704, // dark scroll for staff for m.att
		2044001, // scroll for two-handed sword for att
		2044004, // dark scroll for two-handed-sword for att
		2044101, // scroll for two-handed axe for att
		2044004, // dark scroll for two handed axe for att
		2044201, // scroll for two-handed bw for att
		2044204, // dark scroll for two-handed bw for att
		2044301, // scroll for spear for att
		2044304, // dark scroll for spear for att
		2044401, // scroll for pole arm for att
		2044304, // dark scroll for pole arm for att
		2044501, // scroll for bow for att
		2044504, // dark scroll for bow for att
		2044601, // scroll for crossbow for att
		2044604, // dark scroll for crossbow for att
		2044701, // scroll for claw for att
		2044704, // dark scroll for claw for att
		2044801, // scroll for knuckler for att
		2044804, // dark scroll for knuckler for att
		2044901, // scroll for gun for att
		2044904, // dark scroll for gun for att
		2048010, // scroll for pet equip for str 60%
		2048011, // scroll for pet equip for int 60%
		2048012, // scroll for pet equip for dex 60%
		2048013, // scroll for pet equip for luk 60%
		2049000, // clean slate scroll 1%
		2049001, // clean slate scroll 3%
		2049002, // clean slate scroll 5%
		2049003, // clean slate scroll 20%
		2049100, // chaos scroll 60%
		2340000, // white scroll
		2050001, // eyedrop
		2050002, // tonic
		2050003, // holy water
		2050004, // all cure potion
		2330000, // bullet
		2330001, // split bullet
		2330002, // mighty bullet
		2330003, // vital bullet
		2330004, // shiny bullet
		2330005, // eternal bullet
		2331000, // blaze capsule
		2332000, // glaze capsule
		2060004, // diamond arrow for bow
		2061004, // diamond arrow for crossbow
		2070000, // subi throwing-stars
		2070001, // wolbi throwing-stars
		2070002, // mokbi throwing-stars
		2070003, // kumbi throwing-stars
		2070004, // tobi throwing-stars
		2070005, // steely throwing-stars
		2070006, // ilbi throwing-stars
		2070007, // hwabi throwing-stars
		2070016, // crystal ilbi throwing-stars
		2070018, // balanced fury
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1060031, // blue pao bottom
		1060032, // red pao bottom
		1060063, // green legolier pants
		1060064, // dark legolier pants
		1060081, // blue orientican pants
		1060082, // dark orientican pants
		1060077, // oaker shouldermail pants
		1060075, // brown jangoon pants
		1060069, // brown piette pants
		1060070, // blue piette pants
		1060034, // blue rider pants
		1060035, // shine rider pants
		1060022, // red cloth pants
		1060016, // black martial arts pants
		1060017, // white martial arts pants
		1060010, // steel sergeant kilt
		1040009, // steel corporal
		1040022, // green bennis chainmail
		1040033, // black cloth vest
		1040043, // red pao
		1040046, // shine rider
		1040050, // black sneak
		1040058, // dark silver stealer
		1040079, // brown piette
		1040080, // dark piette
		1040083, // marine shadow
		1040085, // maroon jangoon armor
		1040115, // green osfa suit
		1002044, // mithril kolf
		1002049, // gold burgernet helm
		1002068, // yellow headband
		1002094, // bronze planet
		1002109, // green thief hood
		1002110, // black thief hood
		1002119, // green robin hat
		1002120, // black robin hat
		1002121, // brown robin hat
		1002123, // blue ghetto beanie
		1002127, // red loosecap
		1002144, // brown matty
		1002152, // blue guiltian
		1002180, // dark burgler
		1002183, // green pilfer
		1002020, // red starry bandana
		1002085, // mithril crusader helm
		1002019, // white bandana
		1002080, // red bandana
		1002081, // blue bandana
		1002082, // yellow bandana
		1002083, // black bandana
		1002391, // green bandana
		1002392, // brown pandana
		1002393, // pink bandana
		1002394, // grey bandana
		1002395 // purple bandana

	};

	Inventory *inventory = get_inventory(kInventoryConstantsTypesCash);

	if (!inventory)
	{
		return;
	}

	short ticket_amount = inventory->get_item_amount(5220000);

	if (get_state() == 0)
	{
		std::string text = "Hi! I am Gachapon! You currently have " + std::to_string(ticket_amount) + " Gachapon Tickets. Do you want to use one?";

		send_yes_no(text);
	}

	if (get_state() == 1)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			if (ticket_amount >= 1)
			{
				inventory->remove_item(5220000, 1);

				int array_size = sizeof(items) / sizeof(items[0]);

				int random_value = tools::random_int(0, array_size - 1);

				int item_id = items[random_value];

				inventory = get_inventory(static_cast<signed char>(tools::get_inventory_id_from_item_id(item_id)));

				if (!inventory)
				{
					return;
				}

				bool success = false;

				auto itemx = inventory->give_item_special(success, item_id);

				if (success)
				{

					short watt = itemx->get_weapon_attack();
					short matt = itemx->get_magic_attack();

					itemx->set_weapon_attack(watt + static_cast<short>(tools::random_int(0, 0)));
					itemx->set_magic_attack(matt + static_cast<short>(tools::random_int(0, 0)));

					// packet
					PacketCreator packet118;
					packet118.NewItem(itemx, false);
					send_packet(&packet118);
				}
			}
		}
	}
}
