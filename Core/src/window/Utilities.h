#pragma once 
#include <string> 
#include "NeoWin.h"
#include <Core/src/DS/Spatial/Dimensions.h>
#include <Core/src/DS/Spatial/Rect.h>

namespace Neodot::window
{
	std::wstring GetErrorDescription(HRESULT hr);
	RECT ToWinRect(const DS::RectI&);
	DS::RectI ToRect(const RECT&);
	DS::DimensionsI ClientToWindowDimensions(const DS::DimensionsI& dims, DWORD styles);
}