#pragma once
#include <unordered_map>
#include <typeindex>
#include <any>
#include <functional>
#include <memory>
#include <stdexcept>
#include <format>

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
			serviceMap_[typeid(T)] = gen;
		}
		template<class T>
		std::shared_ptr<T> Resolve()
		{
			// TODO: pull this out of template/header
			if (const auto i = serviceMap_.find(typeid(T)); i != serviceMap_.end())
			{
				auto& entry = i->second;
				try {
					// 인스턴스가 이미 있으면 그걸 리턴
					if (auto ppInstance = std::any_cast<std::shared_ptr<T>>(&entry)) {
						return *ppInstance;
					}
					// 인스턴스가 없으면 만들고 저장 리턴
					auto pInstance = std::any_cast<Generator<T>>(entry)();
					entry = pInstance;
					return pInstance;
				}
				catch (const std::bad_any_cast&) {
					// TODO: assert
					throw std::logic_error{ std::format(
						"Could not resolve Singleton mapped type\nfrom: [{}]\n  to: [{}]\n",
						entry.type().name(), typeid(Generator<T>).name()
					) };
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
		std::unordered_map<std::type_index, std::any> serviceMap_;
	};

	Singletons& Sing();
}