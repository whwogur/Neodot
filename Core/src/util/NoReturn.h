#pragma once

namespace Neodot::util
{
	[[noreturn]]
	void NoReturnImpl_();

#define no_return Neodot::util::NoReturnImpl_()
}