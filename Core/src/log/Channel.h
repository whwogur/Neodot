#pragma once

namespace Neodot::Log
{
	struct Entry;

	class IChannel
	{
	public:
		virtual ~IChannel() = default;
		virtual void Submit(Entry&) = 0;
	};
}