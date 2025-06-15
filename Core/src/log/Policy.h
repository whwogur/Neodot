#pragma once

namespace Neodot::Log
{
	struct Entry;

	class IPolicy
	{
	public:
		virtual ~IPolicy() = default;
		virtual bool TransformFilter(Entry&) = 0;
	};
}