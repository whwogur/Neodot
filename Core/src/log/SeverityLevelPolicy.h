#pragma once
#include "Policy.h"
#include "Level.h"

namespace Neodot::Log
{
	class SeverityLevelPolicy : public IPolicy
	{
	public:
		SeverityLevelPolicy(Level level);
		bool TransformFilter(Entry&) override;
	private:
		Level m_level;
	};
}