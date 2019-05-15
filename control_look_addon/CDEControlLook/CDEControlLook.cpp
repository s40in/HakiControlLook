/*
 * Copyright 2019 s40in. 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */





#include "CDEControlLook.h"

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


CDEControlLook::CDEControlLook(image_id id)
	: HaikuControlLook()
{
}


CDEControlLook::~CDEControlLook()
{
}


void
CDEControlLook::DrawMenuBarBackground(BView* view, BRect& rect,
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
CDEControlLook::DrawMenuItemBackground(BView* view, BRect& rect,
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


void
CDEControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{
	Addon_DrawButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
		background, 1.0, 1.0, flags, borders);
}


void
CDEControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	float radius, const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{
	Addon_DrawButtonFrame(view, rect, updateRect, radius, radius, radius, radius,
		base, background, 1.0, 1.0, flags, borders);
}


void
CDEControlLook::DrawButtonFrame(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	const rgb_color& background, uint32 flags,
	uint32 borders)
{
	Addon_DrawButtonFrame(view, rect, updateRect, leftTopRadius, rightTopRadius,
		leftBottomRadius, rightBottomRadius, base, background,
		1.0, 1.0, flags, borders);
}


void
CDEControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	Addon_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
}


void
CDEControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	Addon_DrawButtonBackground(view, rect, updateRect, radius, radius, radius,
		radius, base, false, flags, borders, orientation);
}


void
CDEControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	Addon_DrawButtonBackground(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base, false, flags,
		borders, orientation);
}



//----- protected 

void
CDEControlLook::Addon_DrawButtonFrame(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	const rgb_color& background, float contrast, float brightness,
	uint32 flags, uint32 borders)
{
	if (!rect.IsValid())
		return;

	// save the clipping constraints of the view
	view->PushState();

	// set clipping constraints to updateRect
	BRegion clipping(updateRect);
	view->ConstrainClippingRegion(&clipping);

	// If the button is flat and neither activated nor otherwise highlighted
	// (mouse hovering or focussed), draw it flat.
	if ((flags & B_FLAT) != 0
		&& (flags & (B_ACTIVATED | B_PARTIALLY_ACTIVATED)) == 0
		&& ((flags & (B_HOVER | B_FOCUSED)) == 0
			|| (flags & B_DISABLED) != 0)) {
		_DrawFrame(view, rect, background, background, background,
			background, borders);
		_DrawFrame(view, rect, background, background, background,
			background, borders);
		view->PopState();
		return;
	}
	

	rgb_color rgbButtonMidColor = { 198, 178, 168, 255 };
	rgb_color rgbButtonHighColor = { 231, 222, 218, 255 };
	rgb_color rgbButtonLowColor = { 106, 96, 90, 255 };

	
	if ((flags & B_DEFAULT_BUTTON) != 0) {
		_DrawFrame(view, rect, rgbButtonLowColor, rgbButtonLowColor,
			 rgbButtonHighColor,  rgbButtonHighColor, borders);
		_DrawFrame(view, rect,rgbButtonMidColor, rgbButtonMidColor,
			 rgbButtonMidColor,  rgbButtonMidColor, borders);
		_DrawFrame(view, rect,rgbButtonMidColor, rgbButtonMidColor,
			 rgbButtonMidColor,  rgbButtonMidColor, borders);	 
		
	}
	
	if ((flags & B_DISABLED) != 0) {
		_DrawFrame(view, rect,rgbButtonLowColor, rgbButtonLowColor,
			 	rgbButtonLowColor,  rgbButtonLowColor, borders);
	}

	
	if ((flags & B_ACTIVATED) != 0) {
		_DrawFrame(view, rect, rgbButtonLowColor, rgbButtonLowColor,
			 rgbButtonHighColor,  rgbButtonHighColor, borders);
	}
	else {
		_DrawFrame(view, rect, rgbButtonHighColor, rgbButtonHighColor,
			 	rgbButtonLowColor,  rgbButtonLowColor, borders);
	}
	

	// restore the clipping constraints of the view
	view->PopState();
}

void
CDEControlLook::Addon_DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	bool popupIndicator, uint32 flags, uint32 borders, orientation orientation)
{
	if (!rect.IsValid())
		return;

	// save the clipping constraints of the view
	view->PushState();

	// set clipping constraints to updateRect
	BRegion clipping(updateRect);
	view->ConstrainClippingRegion(&clipping);

	// If the button is flat and neither activated nor otherwise highlighted
	// (mouse hovering or focussed), draw it flat.
	if ((flags & B_FLAT) != 0
		&& (flags & (B_ACTIVATED | B_PARTIALLY_ACTIVATED)) == 0
		&& ((flags & (B_HOVER | B_FOCUSED)) == 0
			|| (flags & B_DISABLED) != 0)) {
		Addon_DrawFlatButtonBackground(view, rect, updateRect, base, popupIndicator,
			flags, borders, orientation);
	} else {
		//_DrawNonFlatButtonBackground(view, rect, updateRect, clipping,
		//	leftTopRadius, rightTopRadius, leftBottomRadius, rightBottomRadius,
		//	base, popupIndicator, flags, borders, orientation);
		rgb_color rgbButtonMidColor = { 198, 178, 178, 255 };
		Addon_DrawFlatButtonBackground(view, rect, updateRect, rgbButtonMidColor, popupIndicator,
			flags, borders, orientation);	
	}

	// restore the clipping constraints of the view
	view->PopState();
}


void
CDEControlLook::Addon_DrawFlatButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, bool popupIndicator,
	uint32 flags, uint32 borders, orientation orientation)
{
	_DrawFrame(view, rect, base, base, base, base, borders);
		// Not an actual frame, but the method insets our rect as needed.

	view->SetHighColor(base);
	view->FillRect(rect);

	if (popupIndicator) {
		BRect indicatorRect(rect);
		rect.right -= kButtonPopUpIndicatorWidth;
		indicatorRect.left = rect.right + 3;
			// 2 pixels for the separator

		view->SetHighColor(base);
		view->FillRect(indicatorRect);

		_DrawPopUpMarker(view, indicatorRect, base, flags);
	}
}



//-----

extern "C" BControlLook* (instantiate_control_look)(image_id id)
{
	return new (std::nothrow)CDEControlLook(id);
}

