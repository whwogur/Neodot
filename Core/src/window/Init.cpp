#include "Init.h" 
#include <Core/src/IOC/Container.h> 
#include <Core/src/IOC/Singletons.h> 
#include "WindowClass.h" 
#include "Window.h"

template<class T>
std::shared_ptr<T> operator|(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return lhs ? lhs : rhs;
}

namespace Neodot::window
{
	void Init()
	{
		// Container
		IOC::Get().Register<IWindow>(
			[](IWindow::IocParams args)
			{
				return std::make_shared<Window>(
					(args.pClass | IOC::Sing().Resolve<IWindowClass>()),
					args.wName.value_or(L"Main Window"),
					args.iSize.value_or(DS::DimensionsI{ 1280, 720 }),
					args.vPosition
			);
		});
		IOC::Get().Register<IWindowClass>(
			[] { return std::make_shared<WindowClass>();
		});

		// Singleton
		IOC::Sing().RegisterPassthru<IWindowClass>();
	}
}