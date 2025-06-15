#pragma once
#include "Channel.h"
#include "Driver.h"
#include "Policy.h"

namespace Neodot::Log
{
	Channel::Channel(std::vector<std::shared_ptr<IDriver>> driverPtrs)
		:
		m_driverPtrs{ std::move(driverPtrs) }
	{
	}

	Channel::~Channel()
	{
	}

	void Channel::Submit(Entry& e)
	{
		for (auto& pPolicy : m_policyPtrs) {
				if (!pPolicy->TransformFilter(e)) {
					return;
				}
		}

		for (auto& pDriver : m_driverPtrs) {
			pDriver->Submit(e);
		}
	}

	void Channel::AttachDriver(std::shared_ptr<IDriver> pDriver)
	{
		m_driverPtrs.push_back(std::move(pDriver));
	}

	void Channel::AttachPolicy(std::unique_ptr<IPolicy> pPolicy)
	{
		m_policyPtrs.push_back(std::move(pPolicy));
	}
}