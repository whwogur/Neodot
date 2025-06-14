#include "Container.h"

namespace Neodot::IOC
{
	Container& Get() noexcept
	{
		static Container container;
		return container;
	}
}