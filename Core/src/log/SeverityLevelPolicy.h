#pragma once
#include "Policy.h"
#include "Level.h"

namespace Neodot::Log
{
	class ISeverityLevelPolicy : public IPolicy {};

	class SeverityLevelPolicy : public ISeverityLevelPolicy
	{
	public:
		SeverityLevelPolicy(Level level);
		bool TransformFilter(Entry&) override;
	private:
		Level m_level;
	};
}