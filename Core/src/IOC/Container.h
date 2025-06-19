#pragma once
#include <memory>
#include <unordered_map>
#include <functional>
#include <any>
#include <typeindex>
#include <stdexcept>
#include <format>
#include <tuple>
#include "Exception.h"
#include <Core/src/util/Assert.h>
#include <Core/src/util/String.h>

namespace Neodot::IOC // IOC ∆—≈‰∏Æ
{
	template<class T>
	concept Parameterized = requires() {
		{ typename T::IocParams{} };
	};
	template<class T>
	concept NotParameterized = !Parameterized<T>;

	class Container
	{
	public:
		// ===========
		// types
		// ===========
		template<class T>
		using Generator = std::function<std::shared_ptr<T>()>;
		template<class T>
		using ParameterizedGenerator = std::function<std::shared_ptr<T>(typename T::IocParams)>;
		// ===========
		// functions
		// ===========
		template<Parameterized T>
		void Register(ParameterizedGenerator<T> gen)
		{
			m_serviceMap[typeid(T)] = gen;
		}
		template<NotParameterized T>
		void Register(Generator<T> gen)
		{
			m_serviceMap[typeid(T)] = gen;
		}
		template<Parameterized T>
		std::shared_ptr<T> Resolve(typename T::IocParams&& params = {}) const
		{
			return Resolve<T, ParameterizedGenerator<T>>(std::forward<typename T::IocParams>(params));
		}
		template<NotParameterized T>
		std::shared_ptr<T> Resolve() const
		{
			return Resolve<T, Generator<T>>();
		}
	private:
		// ===========
		// functions
		// ===========
		template<class T, class G/*Generator*/, typename...Ps>
		std::shared_ptr<T> Resolve(Ps&&...arg) const
		{
			// TODO: pull this out of template/header
			if (const auto i = m_serviceMap.find(typeid(T)); i != m_serviceMap.end())
			{
				const auto& entry = i->second;
				try {
					return std::any_cast<G>(entry)(std::forward<Ps>(arg)...);
				}
				catch (const std::bad_any_cast&) {
					neo_check_fail.msg(std::format(
						L"Could not resolve IoC mapped type\nfrom: [{}]\n  to: [{}]\n",
						Neodot::util::ToWide(entry.type().name()), Neodot::util::ToWide(typeid(G).name())
					)).ex();
				}
			}
			else
			{
				throw ServiceNotFound{ std::format("Could not find generator for type [{}] in IoC container", typeid(T).name()) };
			}
		}
		//=======
		// data
		//=======
		std::unordered_map<std::type_index, std::any> m_serviceMap;
	};

	Container& Get() noexcept;
}