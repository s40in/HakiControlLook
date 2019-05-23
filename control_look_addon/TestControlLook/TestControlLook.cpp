/*
 * Copyright 2019 s40in. 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */





#include "TestControlLook.h"

#include <algorithm>
#include <cmath>
#include <new>
#include <stdio.h>

#include <WindowPrivate.h>

#include <Autolock.h>
#include <Debug.h>
#include <GradientLinear.h>
#include <Rect.h>
#include <Region.h>
#include <View.h>


//#define DEBUG_CONTROL_LOOK
#ifdef DEBUG_CONTROL_LOOK
#	define STRACE(x) printf x
#else
#	define STRACE(x) ;
#endif

static const float kButtonPopUpIndicatorWidth = 11;


TestControlLook::TestControlLook(image_id id)
	: HaikuControlLook()
{
}


TestControlLook::~TestControlLook()
{
}


void
TestControlLook::DrawMenuBarBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// the surface edges

	// colors
	float topTint;
	float bottomTint;

	if ((flags & B_ACTIVATED) != 0) {
		rgb_color bevelColor1 = tint_color(base, 1.40);
		rgb_color bevelColor2 = tint_color(base, 1.25);

		topTint = 1.25;
		bottomTint = 1.20;

		_DrawFrame(view, rect,
			bevelColor1, bevelColor1,
			bevelColor2, bevelColor2,
			borders & B_TOP_BORDER);
	} else {
		rgb_color cornerColor = tint_color(base, 0.9);
		rgb_color bevelColorTop = tint_color(base, 0.5);
		rgb_color bevelColorLeft = tint_color(base, 0.7);
		rgb_color bevelColorRightBottom = tint_color(base, 1.08);

		topTint = 0.69;
		bottomTint = 1.03;

		_DrawFrame(view, rect,
			bevelColorLeft, bevelColorTop,
			bevelColorRightBottom, bevelColorRightBottom,
			cornerColor, cornerColor,
			borders);
	}

	// draw surface top
	//_FillGradient(view, rect, base, topTint, bottomTint);
	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
}


void
TestControlLook::DrawMenuItemBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// surface edges
	float topTint;
	float bottomTint;
	rgb_color selectedColor = base;

	if ((flags & B_ACTIVATED) != 0) {
		topTint = 0.9;
		bottomTint = 1.05;
	} else if ((flags & B_DISABLED) != 0) {
		topTint = 0.80;
		bottomTint = 1.07;
	} else {
		topTint = 0.6;
		bottomTint = 1.12;
	}

	rgb_color bevelLightColor = tint_color(selectedColor, topTint);
	rgb_color bevelShadowColor = tint_color(selectedColor, bottomTint);

	// draw surface edges
	_DrawFrame(view, rect,
		bevelLightColor, bevelLightColor,
		bevelShadowColor, bevelShadowColor,
		borders);

	// draw surface top
	view->SetLowColor(selectedColor);
	//_FillGradient(view, rect, selectedColor, topTint, bottomTint);
	view->FillRect(rect, B_SOLID_LOW);
}


// Обычная кнопка
void
TestControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{
	if ((flags & B_FLAT) != 0 )	{
		_DrawButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
			background, 1.0, 1.0, flags, borders);
		return;
	}
	else{
	
	rgb_color rgbRed2Color = { 255, 0, 220, 255 };	
		
	_DrawFrame(view, rect, rgbRed2Color, rgbRed2Color,
			 rgbRed2Color,  rgbRed2Color, borders);
	rect.InsetBy(1,1);
	_DrawFrame(view, rect, rgbRed2Color, rgbRed2Color,
			 rgbRed2Color,  rgbRed2Color, borders);
	
	}
			
			
}


void
TestControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	float radius, const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{

			 
			 	rgb_color rgbBlue2Color = { 127, 255, 255, 255 };	
		
	_DrawFrame(view, rect, rgbBlue2Color, rgbBlue2Color,
			rgbBlue2Color,  rgbBlue2Color, borders);
	rect.InsetBy(1,1);
	_DrawFrame(view, rect, rgbBlue2Color, rgbBlue2Color,
			rgbBlue2Color,  rgbBlue2Color, borders);
	
			 
}


void
TestControlLook::DrawButtonFrame(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	const rgb_color& background, uint32 flags,
	uint32 borders)
{
	rgb_color rgbYellow2Color = { 255, 244, 53, 255 };	
		
	_DrawFrame(view, rect, rgbYellow2Color, rgbYellow2Color,
			 rgbYellow2Color,  rgbYellow2Color, borders);
	rect.InsetBy(1,1);	
	_DrawFrame(view, rect, rgbYellow2Color, rgbYellow2Color,
			 rgbYellow2Color,  rgbYellow2Color, borders);	 
			 
}


/*
	- Обычная кнопка
	- Полосы прокрутки
	- Кнопки тулбаров
	- Кнопки-заголовки таблиц
*/
void
TestControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	
	// If the button is flat and neither activated nor otherwise highlighted
	// (mouse hovering or focussed), draw it flat.
	if ((flags & B_FLAT) != 0) {
		_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
	
	
	} else {
		rgb_color rgbGreenColor = { 0, 255, 33, 255 };
		view->SetHighColor(rgbGreenColor);
		view->FillRect(rect);	
	}
	
	
}


void
TestControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	rgb_color rgbRedColor = { 255, 0, 0, 255 };
		
	view->SetHighColor(rgbRedColor);
	view->FillRect(rect);
}


void
TestControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	rgb_color rgbBlueColor = { 0, 38, 255, 255 };
		
	view->SetHighColor(rgbBlueColor);
	view->FillRect(rect);
}









//-----

extern "C" BControlLook* (instantiate_control_look)(image_id id)
{
	return new (std::nothrow)TestControlLook(id);
}

