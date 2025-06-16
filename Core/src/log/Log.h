#pragma once
#include "Channel.h" 
#include "EntryBuilder.h" 

namespace Neodot::Log
{
	IChannel* GetDefaultChannel();

	void Init();

#ifdef NDEBUG 
	inline constexpr int defaultTraceSkip = 2;
#else 
	inline constexpr int defaultTraceSkip = 6;
#endif 
}

#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }.chan(Neodot::Log::GetDefaultChannel()).trace_skip(Neodot::Log::defaultTraceSkip)