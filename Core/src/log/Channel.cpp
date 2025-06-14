#pragma once
#include "Channel.h"
#include "Driver.h"

namespace Neodot::Log
{
	Channel::Channel(std::vector<std::shared_ptr<IDriver>> driverPtrs)
		:
		m_driverPtrs{ std::move(driverPtrs) }
	{
	}
	void Channel::Submit(Entry& e)
	{
		for (auto& pDriver : m_driverPtrs) {
			pDriver->Submit(e);
		}
	}

	void Channel::AttachDriver(std::shared_ptr<IDriver> pDriver)
	{
		m_driverPtrs.push_back(std::move(pDriver));
	}
}