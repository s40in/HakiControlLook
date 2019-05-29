/*
 * Copyright 2019 s40in. 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */





#include "AsciiControlLook.h"

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


AsciiControlLook::AsciiControlLook(image_id id)
	: HaikuControlLook()
{
}


AsciiControlLook::~AsciiControlLook()
{
}


void
AsciiControlLook::DrawMenuBarBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// the surface edges

	// colors
	/*
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
	*/
	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
}


void
AsciiControlLook::DrawMenuItemBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// surface edges
	/*
	float topTint;
	float bottomTint;
	*/
	rgb_color selectedColor = base;

	/*
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

	*/
	// draw surface top
	view->SetLowColor(selectedColor);
	//_FillGradient(view, rect, selectedColor, topTint, bottomTint);
	view->FillRect(rect, B_SOLID_LOW);
}


// Обычная кнопка
void
AsciiControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{
	
/*	if ((flags & B_FLAT) != 0 )	{
		_DrawButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
			background, 1.0, 1.0, flags, borders);
		return;
	}
	else{
*/
		_DrawAddonButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
			background, 1.0, 1.0, flags, borders);
//	}
		
}


// I dont' know who call this function :)
void
AsciiControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	float radius, const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{ 
	_DrawButtonFrame(view, rect, updateRect, radius, radius, radius, radius,
		base, background, 1.0, 1.0, flags, borders);
}


// I dont' know who call this function :)
void
AsciiControlLook::DrawButtonFrame(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	const rgb_color& background, uint32 flags,
	uint32 borders)
{
	_DrawButtonFrame(view, rect, updateRect, leftTopRadius, rightTopRadius,
		leftBottomRadius, rightBottomRadius, base, background,
		1.0, 1.0, flags, borders); 			 
}


/*
	- Обычная кнопка
	- Полосы прокрутки
	- Кнопки тулбаров
	- Кнопки-заголовки таблиц
*/
void
AsciiControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	
	// If the button is flat and neither activated nor otherwise highlighted
	// (mouse hovering or focussed), draw it flat.
//	if ((flags & B_FLAT) != 0) {
//		_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
//		base, false, flags, borders, orientation);
//	} else {
		_DrawAddonButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
		
//	}
	
	
	
}


// I dont' know who call this function :)
void
AsciiControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, radius, radius, radius,
		radius, base, false, flags, borders, orientation);
}


// I dont' know who call this function :)
void
AsciiControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base, false, flags,
		borders, orientation);
}

/*

void
AsciiControlLook::DrawSliderThumb(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags, orientation orientation)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// figure out frame color
	rgb_color frameLightColor;
	rgb_color frameShadowColor;
	rgb_color shadowColor = (rgb_color){ 0, 0, 0, 60 };

	if ((flags & B_FOCUSED) != 0) {
		// focused
		frameLightColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
		frameShadowColor = frameLightColor;
	} else {
		// figure out the tints to be used
		float frameLightTint;
		float frameShadowTint;

		if ((flags & B_DISABLED) != 0) {
			frameLightTint = 1.30;
			frameShadowTint = 1.35;
			shadowColor.alpha = 30;
		} else {
			frameLightTint = 1.6;
			frameShadowTint = 1.65;
		}

		frameLightColor = tint_color(base, frameLightTint);
		frameShadowColor = tint_color(base, frameShadowTint);
	}

	BRect originalRect(rect);
	rect.right--;
	rect.bottom--;

	_DrawFrame(view, rect, frameLightColor, frameLightColor,
		frameShadowColor, frameShadowColor);

	flags &= ~B_ACTIVATED;
	//DrawButtonBackground(view, rect, updateRect, base, flags); 
	// Пока чтоб рисовала как Haiku
	_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, B_ALL_BORDERS, orientation);


	// thumb shadow
	view->SetDrawingMode(B_OP_ALPHA);
	view->SetHighColor(shadowColor);
	originalRect.left++;
	originalRect.top++;
	view->StrokeLine(originalRect.LeftBottom(), originalRect.RightBottom());
	originalRect.bottom--;
	view->StrokeLine(originalRect.RightTop(), originalRect.RightBottom());

	// thumb edge
	if (orientation == B_HORIZONTAL) {
		rect.InsetBy(0, floorf(rect.Height() / 4));
		rect.left = floorf((rect.left + rect.right) / 2);
		rect.right = rect.left + 1;
		shadowColor = tint_color(base, B_DARKEN_2_TINT);
		shadowColor.alpha = 128;
		view->SetHighColor(shadowColor);
		view->StrokeLine(rect.LeftTop(), rect.LeftBottom());
		rgb_color lightColor = tint_color(base, B_LIGHTEN_2_TINT);
		lightColor.alpha = 128;
		view->SetHighColor(lightColor);
		view->StrokeLine(rect.RightTop(), rect.RightBottom());
	} else {
		rect.InsetBy(floorf(rect.Width() / 4), 0);
		rect.top = floorf((rect.top + rect.bottom) / 2);
		rect.bottom = rect.top + 1;
		shadowColor = tint_color(base, B_DARKEN_2_TINT);
		shadowColor.alpha = 128;
		view->SetHighColor(shadowColor);
		view->StrokeLine(rect.LeftTop(), rect.RightTop());
		rgb_color lightColor = tint_color(base, B_LIGHTEN_2_TINT);
		lightColor.alpha = 128;
		view->SetHighColor(lightColor);
		view->StrokeLine(rect.LeftBottom(), rect.RightBottom());
	}

	view->SetDrawingMode(B_OP_COPY);
}

void
AsciiControlLook::DrawScrollViewFrame(BView* view, BRect& rect,
	const BRect& updateRect, BRect verticalScrollBarFrame,
	BRect horizontalScrollBarFrame, const rgb_color& base,
	border_style borderStyle, uint32 flags, uint32 _borders)
{
	// calculate scroll corner rect before messing with the "rect"
	BRect scrollCornerFillRect(rect.right, rect.bottom,
		rect.right, rect.bottom);

	if (horizontalScrollBarFrame.IsValid())
		scrollCornerFillRect.left = horizontalScrollBarFrame.right + 1;

	if (verticalScrollBarFrame.IsValid())
		scrollCornerFillRect.top = verticalScrollBarFrame.bottom + 1;

	if (borderStyle == B_NO_BORDER) {
		if (scrollCornerFillRect.IsValid()) {
			view->SetHighColor(base);
			view->FillRect(scrollCornerFillRect);
		}
		return;
	}

	bool excludeScrollCorner = borderStyle == B_FANCY_BORDER
		&& horizontalScrollBarFrame.IsValid()
		&& verticalScrollBarFrame.IsValid();

	uint32 borders = _borders;
	if (excludeScrollCorner) {
		rect.bottom = horizontalScrollBarFrame.top;
		rect.right = verticalScrollBarFrame.left;
		borders &= ~(B_RIGHT_BORDER | B_BOTTOM_BORDER);
	}

	rgb_color scrollbarFrameColor = tint_color(base, B_DARKEN_2_TINT);

	if (borderStyle == B_FANCY_BORDER)
		_DrawOuterResessedFrame(view, rect, base, 1.0, 1.0, flags, borders);

	if ((flags & B_FOCUSED) != 0) {
		rgb_color focusColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
		_DrawFrame(view, rect, focusColor, focusColor, focusColor, focusColor,
			borders);
	} else {
		_DrawFrame(view, rect, scrollbarFrameColor, scrollbarFrameColor,
			scrollbarFrameColor, scrollbarFrameColor, borders);
	}

	if (excludeScrollCorner) {
		horizontalScrollBarFrame.InsetBy(-1, -1);
		// do not overdraw the top edge
		horizontalScrollBarFrame.top += 2;
		borders = _borders;
		borders &= ~B_TOP_BORDER;
		_DrawOuterResessedFrame(view, horizontalScrollBarFrame, base,
			1.0, 1.0, flags, borders);
		_DrawFrame(view, horizontalScrollBarFrame, scrollbarFrameColor,
			scrollbarFrameColor, scrollbarFrameColor, scrollbarFrameColor,
			borders);

		verticalScrollBarFrame.InsetBy(-1, -1);
		// do not overdraw the left edge
		verticalScrollBarFrame.left += 2;
		borders = _borders;
		borders &= ~B_LEFT_BORDER;
		_DrawOuterResessedFrame(view, verticalScrollBarFrame, base,
			1.0, 1.0, flags, borders);
		_DrawFrame(view, verticalScrollBarFrame, scrollbarFrameColor,
			scrollbarFrameColor, scrollbarFrameColor, scrollbarFrameColor,
			borders);

		// exclude recessed frame
		scrollCornerFillRect.top++;
		scrollCornerFillRect.left++;
	}

	if (scrollCornerFillRect.IsValid()) {
		view->SetHighColor(base);
		view->FillRect(scrollCornerFillRect);
	}
}

*/

void
AsciiControlLook::DrawRadioButton(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
	
	rgb_color rgbButtonColor = { 0, 0, 0, 255};
	rgb_color  rgbButtonColorInactive = { 192, 192, 192, 255};
	rgb_color  rgbButtonColorFocus = { 46, 204, 64, 255};
	
	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
	
	
	if ((flags & B_DISABLED) != 0) {
		view->SetHighColor(rgbButtonColorInactive);
	} else if ((flags & B_CLICKED) != 0) {
		view->SetHighColor(rgbButtonColorFocus);
	} else if ((flags & B_FOCUSED) != 0)  {
		view->SetHighColor(rgbButtonColorFocus);
	} else {
		view->SetHighColor(rgbButtonColor);
	}
	
		
	rect.InsetBy(1,1); // make smaller
	
	BRect buttonRect;
	buttonRect = rect;
	
	view->StrokeEllipse(rect);
	rect.InsetBy(1,1);
	view->StrokeEllipse(rect);
	
	buttonRect.left += 4;
	buttonRect.right -=4;
	view->SetHighColor(base);
	view->FillRect(buttonRect, B_SOLID_HIGH);
	
	
  //if (_RadioButtonAndCheckBoxMarkColor(base, rgbButtonColor, flags)) {
  if ((flags & (B_ACTIVATED | B_PARTIALLY_ACTIVATED | B_CLICKED)) ) {	
  	rect.InsetBy(4,4);
  	view->SetHighColor(rgbButtonColor);
	view->FillEllipse(rect);

 }


}

void
AsciiControlLook::DrawCheckBox(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
		
	
	rgb_color rgbButtonColor = { 0, 0, 0, 255};
	rgb_color  rgbButtonColorInactive = { 192, 192, 192, 255};
	rgb_color  rgbButtonColorFocus = { 46, 204, 64, 255};
	
	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
	
	
	if ((flags & B_DISABLED) != 0) {
		view->SetHighColor(rgbButtonColorInactive);
	} else if ((flags & B_CLICKED) != 0) {
		view->SetHighColor(rgbButtonColorFocus);
	} else if ((flags & B_FOCUSED) != 0)  {
		view->SetHighColor(rgbButtonColorFocus);
	} else {
		view->SetHighColor(rgbButtonColor);
	}
	
		
	rect.InsetBy(1,1); // make smaller
	
	BRect buttonRect;
	buttonRect = rect;
	
	view->StrokeRect(rect);
	rect.InsetBy(1,1);
	view->StrokeRect(rect);
	
	buttonRect.left += 4;
	buttonRect.right -=4;
	view->SetHighColor(base);
	view->FillRect(buttonRect, B_SOLID_HIGH);
	
	
  //if (_RadioButtonAndCheckBoxMarkColor(base, rgbButtonColor, flags)) {
  if ((flags & (B_ACTIVATED | B_PARTIALLY_ACTIVATED | B_CLICKED)) ) {	
  	rect.InsetBy(4,4);
  	view->SetHighColor(rgbButtonColor);
	view->FillRect(rect,  B_SOLID_HIGH);

 }
	
}


//-------------------------------------------------

// #pragma mark -


void
AsciiControlLook::_DrawAddonButtonBackground(BView* view, BRect& rect,
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

	view->SetHighColor(base);
	view->FillRect(rect, B_SOLID_HIGH);
	

	view->PopState();
}


void
AsciiControlLook::_DrawAddonButtonFrame(BView* view, BRect& rect,
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
		_DrawFrame(view, rect, background, background, background,
			background, borders);	
		view->PopState();
		return;
	}
	

	
	rgb_color rgbButtonColor = { 0, 0, 0, 255};
	rgb_color  rgbButtonColorInactive = { 192, 192, 192, 255};
	rgb_color  rgbButtonColorFocus = { 46, 204, 64, 255};

	view->SetLowColor(background);
	view->FillRect(rect, B_SOLID_LOW);
	
	if ((flags & B_DEFAULT_BUTTON) != 0) {
		view->SetHighColor(rgbButtonColor);
		view->StrokeRect(rect);
		rect.InsetBy(3,3); 
	}
	
	
	
	if ((flags & B_DISABLED) != 0) {
		view->SetHighColor(rgbButtonColorInactive);
	} 
	else if ((flags & B_FOCUSED) != 0) {
		view->SetHighColor(rgbButtonColorFocus);
	} 
	else {
		view->SetHighColor(rgbButtonColor);
	}

	
	if ((flags & B_ACTIVATED) != 0) {
		/*view->StrokeRect(rect);
		rect.top += 2;
		rect.left += 2;
		view->StrokeLine(rect.LeftBottom(), rect.LeftTop());
		view->StrokeLine(rect.LeftTop(), rect.RightTop());
		rect.InsetBy(1,1);
		*/
		_DrawAsciiUpFrame(view, rect, true);
		
	}
	else {
		_DrawAsciiUpFrame(view, rect, false);
		/*
		view->StrokeRect(rect);
		rect.bottom -= 2;
		rect.right -= 2;
		view->StrokeLine(rect.RightBottom(), rect.RightTop());
		view->StrokeLine(rect.RightBottom(), rect.LeftBottom());
		rect.InsetBy(1,1);
		*/
	}
	
	view->PopState();
}

//#pragma mark - Private methods

void
AsciiControlLook::_DrawAsciiUpFrame(BView* view, BRect & rect, bool push)
{
	if (push) {
		view->StrokeRect(rect);
		rect.top += 2;
		rect.left += 2;
		view->StrokeLine(rect.LeftBottom(), rect.LeftTop());
		view->StrokeLine(rect.LeftTop(), rect.RightTop());
	}
	else {
		view->StrokeRect(rect);
		rect.bottom -= 2;
		rect.right -= 2;
		view->StrokeLine(rect.RightBottom(), rect.RightTop());
		view->StrokeLine(rect.RightBottom(), rect.LeftBottom());	
	}
	rect.InsetBy(1,1);
	
}


//-----

extern "C" BControlLook* (instantiate_control_look)(image_id id)
{
	return new (std::nothrow)AsciiControlLook(id);
}

