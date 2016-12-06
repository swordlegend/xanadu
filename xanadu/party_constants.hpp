//

#pragma once

namespace PartySendPaketActions
{
	constexpr signed char kInvite = 4;
	constexpr signed char kUpdate = 7;
	constexpr signed char kCreate = 8;
	constexpr signed char kRemoveOrLeave = 12;
	constexpr signed char kJoin = 15;
	constexpr signed char kChangeLeader = 26;
}

namespace PartyReceivePacketActions
{
	enum
	{
		kCreate = 1,
		kLeave,
		kJoin,
		kInvite,
		kExpel,
		kChangeLeader
	};
}

namespace PartyInviteSendPacketActions
{
	constexpr signed char kInvite = 23;
}
