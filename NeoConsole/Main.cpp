#include <iostream>
#include <Core/src/log/EntryBuilder.h>
#include <Core/src/log/Channel.h>
#include <Core/src/log/MsvcDebugDriver.h>
#include <Core/src/log/TextFormatter.h>
#include <Core/src/log/SeverityLevelPolicy.h>

using namespace std::string_literals;

#define neolog Neodot::Log::EntryBuilder{ __FILEW__, __FUNCTIONW__, __LINE__ }.chan(pChan.get())

std::unique_ptr<Neodot::Log::IChannel> pChan;

void foo()
{
	neolog.error(L"Found an Error!");
}

int main()
{
	pChan = std::make_unique<Neodot::Log::Channel>(
		std::vector<std::shared_ptr<Neodot::Log::IDriver>>{
		std::make_shared<Neodot::Log::MsvcDebugDriver>(std::make_unique<Neodot::Log::TextFormatter>())}
	);
	pChan->AttachPolicy(std::make_unique<Neodot::Log::SeverityLevelPolicy>(Neodot::Log::Level::Error));
	neolog.fatal(L"Fatal Error!");
	neolog.warn(L"Warning");
	foo();

	return 0;
}