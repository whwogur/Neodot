#pragma once 
#include <string> 
#include "NeoWin.h"
#include <Core/src/DS/Dimensions.h>
#include <Core/src/DS/Rect.h>

namespace Neodot::window
{
	std::wstring GetErrorDescription(HRESULT hr);
	RECT ToWinRect(const DS::RectI&);
	DS::RectI ToRect(const RECT&);
	DS::DimensionsI ClientToWindowDimensions(const DS::DimensionsI& dims, DWORD styles);
}