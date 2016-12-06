//

#pragma once

namespace BuddylistSendPacketActions
{
	constexpr signed char kUpdate = 7;
	constexpr signed char kInvite = 9;
	constexpr signed char kUpdateChannel = 20;
	constexpr signed char kUpdateCapacity = 21;
}

namespace BuddylistReceivePacketActions
{
	enum
	{
		kAdd = 1,
		kAccept,
		kDelete
	};
}

namespace Buddylist
{
	enum
	{
		kOppositeStatusRegistered = 0,
		kOppositeStatusRequested = 1,
		kOppositeStatusUnregistered = 2
	};
}
