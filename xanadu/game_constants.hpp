//

#pragma once

#include <string>

constexpr std::size_t kPacketHandshakeServerLength = 13; // the size of the handshake packet
constexpr short kGameVersion = 62; // mayor game version
const std::string kGameMinorVersion = ""; // minor game version, if 0, then the string is empty


namespace GameLocales
{
	const signed char kGlobalMapleStory = 8;
}

constexpr unsigned char kGameLocale = GameLocales::kGlobalMapleStory;

// standard install path would be: "C:\\Nexon\\MapleStory\\"
const std::string kGameFilePath = "C:\\Nexon\\MapleStory v0.62\\";
