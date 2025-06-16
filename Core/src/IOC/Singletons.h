#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <memory>
#include <stdexcept>
#include <format>
#include "Container.h"
#include "Exception.h"
#include <Core/src/util/Assert.h>
#include <Core/src/util/String.h>

namespace Neodot::IOC
{
	class Singletons
	{
	public:
		//========
		// types
		//========
		template<class T>
		using Generator = std::function<std::shared_ptr<T>()>;
		//===========
		// functions
		//===========
		template<class T>
		void Register(Generator<T> gen)
		{
			m_serviceMap[typeid(T)] = gen;
		}

		template<class T>
		void RegisterPassthru()
		{
			Register<T>([] { return Neodot::IOC::Get().Resolve<T>(); });
		}

		template<class T>
		std::shared_ptr<T> Resolve()
		{
			// TODO: pull this out of template/header
			if (const auto i = m_serviceMap.find(typeid(T)); i != m_serviceMap.end())
			{
				auto& entry = i->second;
				try
				{
					// 인스턴스가 이미 있으면 그걸 리턴
					if (auto ppInstance = std::any_cast<std::shared_ptr<T>>(&entry)) {
						return *ppInstance;
					}
					// 인스턴스가 없으면 만들고 저장 리턴
					auto pInstance = std::any_cast<Generator<T>>(entry)();
					entry = pInstance;
					return pInstance;
				}
				catch (const std::bad_any_cast&)
				{
					neo_assert(false).msg(std::format(
						L"Could not resolve Singleton mapped type\nfrom: [{}]\n  to: [{}]\n",
						Neodot::util::ToWide(entry.type().name()),
						Neodot::util::ToWide(typeid(Generator<T>).name())
					)).ex();
					return {};
				}
			}
			else
			{
				throw std::runtime_error{ std::format("Could not find entry for type [{}] in singleton container", typeid(T).name()) };
			}
		}
	private:
		//===========
		// data
		//===========
		std::unordered_map<std::type_index, std::any> m_serviceMap;
	};

	Singletons& Sing();
}