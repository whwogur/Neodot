#include "Init.h" 
#include <Core/src/IOC/Container.h> 
#include <Core/src/IOC/Singletons.h> 
#include "WindowClass.h" 


namespace Neodot::window
{
	void Init()
	{
		// Container
		IOC::Get().Register<IWindowClass>(
			[] { return std::make_shared<WindowClass>();
		});

		// Singleton
		IOC::Sing().RegisterPassthru<IWindowClass>();
	}
}