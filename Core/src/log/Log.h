#pragma once
#include "Channel.h" 
#include "EntryBuilder.h" 

namespace Neodot::Log
{
	IChannel* GetDefaultChannel();

	void Init();
}

#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }.chan(Neodot::Log::GetDefaultChannel())