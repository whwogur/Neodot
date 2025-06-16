#include "NoReturn.h"
#include "Assert.h"

namespace Neodot::util
{
	void NoReturnImpl_()
	{
		neo_assert(false).msg(L"[ no return guard ]");
	}
}