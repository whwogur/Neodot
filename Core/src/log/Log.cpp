#include "Log.h"
#include <Core/src/ioc/Container.h>
#include <Core/src/ioc/Singletons.h>
#include "SeverityLevelPolicy.h"
#include "MsvcDebugDriver.h"
#include "TextFormatter.h"

namespace Neodot::Log
{
	IChannel* GetDefaultChannel()
	{
		static std::shared_ptr<IChannel> channelCachePtr = Neodot::IOC::Sing().Resolve<IChannel>();
		return channelCachePtr.get();
	}

	void Init()
	{
		using namespace Neodot;

		// container
		IOC::Get().Register<Log::IChannel>([] {
			std::vector drivers{ IOC::Get().Resolve<Log::IDriver>() };
			auto pChan = std::make_shared<Log::Channel>(std::move(drivers));
			pChan->AttachPolicy(IOC::Get().Resolve<Log::SeverityLevelPolicy>());
			return pChan;
			});
		IOC::Get().Register<Log::IDriver>([] {
			return std::make_shared<Log::MsvcDebugDriver>(IOC::Get().Resolve<Log::ITextFormatter>());
			});
		IOC::Get().Register<Log::ITextFormatter>([] {
			return std::make_shared<Log::TextFormatter>();
			});
		IOC::Get().Register<Log::SeverityLevelPolicy>([] {
			return std::make_shared<Log::SeverityLevelPolicy>(Log::Level::Error);
			});

		// Singleton
		IOC::Sing().RegisterPassthru<Log::IChannel>();
	}
}