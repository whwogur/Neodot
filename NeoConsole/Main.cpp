#include <iostream>
#include <Core/src/log/EntryBuilder.h>
#include <Core/src/log/Channel.h>
#include <Core/src/log/MsvcDebugDriver.h>
#include <Core/src/log/TextFormatter.h>

using namespace std::string_literals;

#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }.chan(pChan.get())

int main()
{
	std::unique_ptr<Neodot::Log::IChannel> pChan = std::make_unique<Neodot::Log::Channel>(std::vector<std::shared_ptr<Neodot::Log::IDriver>>{
		std::make_shared<Neodot::Log::MsvcDebugDriver>(std::make_unique<Neodot::Log::TextFormatter>())
	});

	neolog.fatal(L"Test Failed");
	return 0;
}