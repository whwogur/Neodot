#pragma once
#include <memory>
#include <vector>

namespace Neodot::Log
{
	struct Entry;
	class IDriver;
	class IPolicy;

	class IChannel
	{
	public:
		virtual ~IChannel() = default;
		virtual void Submit(Entry&) = 0;
		virtual void Flush() = 0;
		virtual void AttachDriver(std::shared_ptr<IDriver>) = 0;
		virtual void AttachPolicy(std::shared_ptr<IPolicy>) = 0;
	};

	class Channel : public IChannel
	{
	public:
		Channel(std::vector<std::shared_ptr<IDriver>> driverPtrs = {});
		~Channel();
		void Submit(Entry&) override;
		void Flush() override;
		void AttachDriver(std::shared_ptr<IDriver>) override;
		void AttachPolicy(std::shared_ptr<IPolicy>) override;

	private:
		std::vector<std::shared_ptr<IDriver>> m_driverPtrs;
		std::vector<std::shared_ptr<IPolicy>> m_policyPtrs;
	};
}