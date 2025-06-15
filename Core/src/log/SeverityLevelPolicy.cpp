#pragma once
#include "SeverityLevelPolicy.h"
#include "Entry.h"

namespace Neodot::Log
{
	SeverityLevelPolicy::SeverityLevelPolicy(Level level)
		: m_level{ level }
	{
	}

	/*
	*  0 None
	*  1 Fatal
	*  2 Error
	*  3 Warn
	*  4 Info
	*  5 Debug
	*  6 Verbose
	*/
	bool SeverityLevelPolicy::TransformFilter(Entry& e)
	{
		return e.m_level <= m_level;
	}
}