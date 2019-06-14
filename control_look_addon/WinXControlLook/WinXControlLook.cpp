/*
 * Copyright 2019 s40in. 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */





#include "WinXControlLook.h"

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


WinXControlLook::WinXControlLook(image_id id)
	: HaikuControlLook()
{
}


WinXControlLook::~WinXControlLook()
{
}


void
WinXControlLook::DrawMenuBarBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
}


void
WinXControlLook::DrawMenuFieldFrame(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base,
	const rgb_color& background, uint32 flags, uint32 borders)
{
	
	_DrawAddonButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
		background, 0.6, 1.0, flags, borders);

}


void
WinXControlLook::DrawMenuFieldFrame(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base,
	const rgb_color& background, uint32 flags, uint32 borders)
{
	_DrawButtonFrame(view, rect, updateRect, radius, radius, radius, radius,
		base, background, 0.6, 1.0, flags, borders);
}


void
WinXControlLook::DrawMenuFieldFrame(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius,
	float rightTopRadius, float leftBottomRadius,
	float rightBottomRadius, const rgb_color& base,
	const rgb_color& background, uint32 flags, uint32 borders)
{
	_DrawButtonFrame(view, rect, updateRect, leftTopRadius, rightTopRadius,
		leftBottomRadius, rightBottomRadius, base, background, 0.6, 1.0,
		flags, borders);
}

void
WinXControlLook::DrawMenuFieldBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, bool popupIndicator,
	uint32 flags)
{
	_DrawAddonMenuFieldBackgroundOutside(view, rect, updateRect,
		0.0f, 0.0f, 0.0f, 0.0f, base, popupIndicator, flags);
}


void
WinXControlLook::DrawMenuFieldBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	_DrawAddonMenuFieldBackgroundInside(view, rect, updateRect,
		0.0f, 0.0f, 0.0f, 0.0f, base, flags, borders);
}


void
WinXControlLook::DrawMenuFieldBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base,
	bool popupIndicator, uint32 flags)
{
	_DrawAddonMenuFieldBackgroundOutside(view, rect, updateRect, radius, radius,
		radius, radius, base, popupIndicator, flags);
}


void
WinXControlLook::DrawMenuFieldBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	bool popupIndicator, uint32 flags)
{
	_DrawAddonMenuFieldBackgroundOutside(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base,
		popupIndicator, flags);
}


void
WinXControlLook::DrawMenuItemBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;


	view->SetLowColor(base);
	view->FillRect(rect, B_SOLID_LOW);
	

}


// Обычная кнопка
void
WinXControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
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
WinXControlLook::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	float radius, const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{ 
	_DrawButtonFrame(view, rect, updateRect, radius, radius, radius, radius,
		base, background, 1.0, 1.0, flags, borders);
}


// I dont' know who call this function :)
void
WinXControlLook::DrawButtonFrame(BView* view, BRect& rect,
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
WinXControlLook::DrawButtonBackground(BView* view, BRect& rect,
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
WinXControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, radius, radius, radius,
		radius, base, false, flags, borders, orientation);
}


// I dont' know who call this function :)
void
WinXControlLook::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base, false, flags,
		borders, orientation);
}



void
WinXControlLook::DrawSliderThumb(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags, orientation orientation)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	
	rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color grayColor = { 122, 122, 122, 255};
	
	if ((flags & B_FOCUSED) != 0) {
		view->SetHighColor(blueColor);
	} else {
		view->SetHighColor(grayColor);
	}
	
	view->StrokeRect(rect);
	rect.InsetBy(1,1);
	
	view->SetHighColor(base);
	view->FillRect(rect);
			
}



void
WinXControlLook::DrawScrollViewFrame(BView* view, BRect& rect,
	const BRect& updateRect, BRect verticalScrollBarFrame,
	BRect horizontalScrollBarFrame, const rgb_color& base,
	border_style borderStyle, uint32 flags, uint32 _borders)
{
	/*
	rgb_color g1 = { 0, 255, 0, 255};
	rgb_color r1 = { 255, 0, 0, 255};
	rgb_color b1 = { 0, 0, 255, 255};
	
	view->SetHighColor(g1);
	view->FillRect(rect, B_SOLID_HIGH);
	
	view->SetHighColor(r1);
	view->FillRect(verticalScrollBarFrame, B_SOLID_HIGH);
	
	view->SetHighColor(b1);
	view->FillRect(horizontalScrollBarFrame, B_SOLID_HIGH);
	*/
	
	
	rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color grayColor = { 122, 122, 122, 255};
	

		
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
	
	view->SetHighColor(grayColor);
	view->StrokeRect(rect);
	
	rect.InsetBy(1,1);
	
	if ((flags & B_FOCUSED) != 0) {
		view->SetHighColor(blueColor);
	} 
	else {
		view->SetHighColor(base);
	}
	view->StrokeRect(rect);
	
}

void
WinXControlLook::DrawScrollBarBackground(BView* view, BRect& rect1, BRect& rect2,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	orientation orientation)
{
	DrawScrollBarBackground(view, rect1, updateRect, base, flags, orientation);
	DrawScrollBarBackground(view, rect2, updateRect, base, flags, orientation);
}

void
WinXControlLook::DrawScrollBarBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	orientation orientation)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;


	//rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color grayColor = { 122, 122, 122, 255};
	
	
	//view->SetHighColor(grayColor);
	if ((flags & B_DISABLED) != 0) {
		view->SetHighColor(base);
		view->FillRect(rect);
	} else {
		view->SetHighColor(grayColor);
		view->FillRect(rect);	
	}
	
		
	/*
	float gradient1Tint;
	float gradient2Tint;
	float darkEdge1Tint;
	float darkEdge2Tint;
	float shadowTint;

	if ((flags & B_DISABLED) != 0) {
		gradient1Tint = 0.9;
		gradient2Tint = 0.8;
		darkEdge1Tint = B_DARKEN_2_TINT;
		darkEdge2Tint = B_DARKEN_2_TINT;
		shadowTint = gradient1Tint;
	} else {
		gradient1Tint = 1.10;
		gradient2Tint = 1.05;
		darkEdge1Tint = B_DARKEN_3_TINT;
		darkEdge2Tint = B_DARKEN_2_TINT;
		shadowTint = gradient1Tint;
	}

	rgb_color darkEdge1 = tint_color(base, darkEdge1Tint);
	rgb_color darkEdge2 = tint_color(base, darkEdge2Tint);
	rgb_color shadow = tint_color(base, shadowTint);

	if (orientation == B_HORIZONTAL) {
		// dark vertical line on left edge
		if (rect.Width() > 0) {
			view->SetHighColor(darkEdge1);
			view->StrokeLine(rect.LeftTop(), rect.LeftBottom());
			rect.left++;
		}
		// dark vertical line on right edge
		if (rect.Width() >= 0) {
			view->SetHighColor(darkEdge2);
			view->StrokeLine(rect.RightTop(), rect.RightBottom());
			rect.right--;
		}
		// vertical shadow line after left edge
		if (rect.Width() >= 0) {
			view->SetHighColor(shadow);
			view->StrokeLine(rect.LeftTop(), rect.LeftBottom());
			rect.left++;
		}
		// fill
		if (rect.Width() >= 0) {
			_FillGradient(view, rect, base, gradient1Tint, gradient2Tint,
				orientation);
		}
	} else {
		// dark vertical line on top edge
		if (rect.Height() > 0) {
			view->SetHighColor(darkEdge1);
			view->StrokeLine(rect.LeftTop(), rect.RightTop());
			rect.top++;
		}
		// dark vertical line on bottom edge
		if (rect.Height() >= 0) {
			view->SetHighColor(darkEdge2);
			view->StrokeLine(rect.LeftBottom(), rect.RightBottom());
			rect.bottom--;
		}
		// horizontal shadow line after top edge
		if (rect.Height() >= 0) {
			view->SetHighColor(shadow);
			view->StrokeLine(rect.LeftTop(), rect.RightTop());
			rect.top++;
		}
		// fill
		if (rect.Height() >= 0) {
			_FillGradient(view, rect, base, gradient1Tint, gradient2Tint,
				orientation);
		}
	}
	*/
}



void
WinXControlLook::DrawButtonWithPopUpBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	//_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
	//	base, true, flags, borders, orientation);
	_DrawAddonButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
}

void
WinXControlLook::DrawButtonWithPopUpBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, radius, radius, radius,
		radius, base, true, flags, borders, orientation);
}


void
WinXControlLook::DrawButtonWithPopUpBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base, true, flags,
		borders, orientation);
}



void
WinXControlLook::DrawRadioButton(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
	
	rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color blueBackground = { 229, 241, 251, 255};
	rgb_color blackColor= {0,0,0,255};	
	rgb_color whiteColor= {255, 255, 255, 255};	
	rgb_color markColor= {103, 103, 200, 255};
	rgb_color borderColor;
	rgb_color backgroundColor;
	rgb_color navigationColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
	
	if ((flags & B_DISABLED) != 0) {
		backgroundColor = base;
		borderColor = blackColor;
	} else if ((flags & B_CLICKED) != 0) {
		backgroundColor = blueBackground;
		borderColor = blueColor;
	} else {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	}
	
	if ((flags & B_FOCUSED) != 0) {
		borderColor = navigationColor;
	}
		
	rect.InsetBy(1,1); // make smaller
		
	view->SetHighColor(backgroundColor);
	view->FillEllipse(rect);
	
	view->SetHighColor(borderColor);
	view->StrokeEllipse(rect);
	rect.InsetBy(4,4);

	
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {
  	view->SetHighColor(borderColor);
  	view->FillEllipse(rect);
 	}

}

void
WinXControlLook::DrawCheckBox(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
		
	rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color blueBackground = { 229, 241, 251, 255};
	rgb_color blackColor= {0,0,0,255};	
	rgb_color whiteColor= {255, 255, 255, 255};	
	rgb_color markColor= {103, 103, 200, 255};
	rgb_color borderColor;
	rgb_color backgroundColor;
	rgb_color navigationColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
	
	if ((flags & B_DISABLED) != 0) {
		backgroundColor = base;
		borderColor = blackColor;
	} else if ((flags & B_CLICKED) != 0) {
		backgroundColor = blueBackground;
		borderColor = blueColor;
	} else {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	}
	
	if ((flags & B_FOCUSED) != 0) {
		borderColor = navigationColor;
	}
		
	rect.InsetBy(1,1); // make smaller
		
	view->SetLowColor(backgroundColor);
	view->FillRect(rect, B_SOLID_LOW);
	
	view->SetHighColor(borderColor);
	view->StrokeRect(rect);
	rect.InsetBy(3,3);

	
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {
  	view->SetHighColor(borderColor);
  	view->StrokeLine(rect.LeftTop(),rect.RightBottom());
  	view->StrokeLine(rect.LeftBottom(),rect.RightTop());

 }
	
}

// #pragma mark -


void
WinXControlLook::DrawGroupFrame(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 borders)
{
	rgb_color grayColor = { 122, 122, 122, 255};

	_DrawFrame(view, rect, base, base, base, base,
		borders);

	_DrawFrame(view, rect, grayColor, grayColor, grayColor, grayColor,
		borders);

	_DrawFrame(view, rect, base, base, base, base,
		borders);
}

void
WinXControlLook::DrawActiveTab(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags, uint32 borders, uint32 side)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// Snap the rectangle to pixels to avoid rounding errors.
	rect.left = floorf(rect.left);
	rect.right = floorf(rect.right);
	rect.top = floorf(rect.top);
	rect.bottom = floorf(rect.bottom);

	// save the clipping constraints of the view
	view->PushState();


	rgb_color grayColor = { 122, 122, 122, 255};
	//rgb_color blueColor = { 0, 120, 215, 255};
	
	uint32 bordersToDraw = 0;
	switch (side) {
		case B_TOP_BORDER:
			bordersToDraw = (B_LEFT_BORDER | B_TOP_BORDER | B_RIGHT_BORDER);
			break;
		case B_BOTTOM_BORDER:
			bordersToDraw = (B_LEFT_BORDER | B_BOTTOM_BORDER | B_RIGHT_BORDER);
			break;
		case B_LEFT_BORDER:
			bordersToDraw = (B_LEFT_BORDER | B_BOTTOM_BORDER | B_TOP_BORDER);
			break;
		case B_RIGHT_BORDER:
			bordersToDraw = (B_RIGHT_BORDER | B_BOTTOM_BORDER | B_TOP_BORDER);
			break;
	}
	
	if (side == B_TOP_BORDER || side == B_BOTTOM_BORDER) {
		if ((borders & B_LEFT_BORDER) == 0)
			rect.left++;
		if ((borders & B_RIGHT_BORDER) == 0)
			rect.right--;
	} else if (side == B_LEFT_BORDER || side == B_RIGHT_BORDER) {
		if ((borders & B_TOP_BORDER) == 0)
			rect.top++;
		if ((borders & B_BOTTOM_BORDER) == 0)
			rect.bottom--;
	}
	
	_DrawFrame(view, rect, grayColor, grayColor, grayColor, grayColor, bordersToDraw);
		
	view->SetLowColor(base);
	view->FillRect(rect,B_SOLID_LOW);

	// restore the clipping constraints of the view
	view->PopState();
}


void
WinXControlLook::DrawInactiveTab(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags, uint32 borders, uint32 side)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;
		
	view->PushState();
		
	rgb_color grayColor = { 122, 122, 122, 255};
	
	view->SetLowColor(base);
	view->FillRect(rect,B_SOLID_LOW);
	
		switch (side) {
		case B_TOP_BORDER:
			rect.top += 2;
			break;
		case B_BOTTOM_BORDER:
			rect.bottom -= 2;
			break;
		case B_LEFT_BORDER:
			rect.left += 2;
			break;
		case B_RIGHT_BORDER:
			rect.right -= 2;
		break;
	}
	
	_DrawFrame(view, rect, grayColor, grayColor, grayColor, grayColor, borders);

	
		
	
	view->PopState();
}


void
WinXControlLook::DrawTextControlBorder(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	if (!rect.Intersects(updateRect))
		return;

	//rgb_color dark1BorderColor;
	//rgb_color dark2BorderColor;
	///rgb_color navigationColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
	//rgb_color invalidColor = ui_color(B_FAILURE_COLOR);
	rgb_color grayColor = { 122, 122, 122, 255};
	rgb_color blueColor = { 0, 120, 215, 255};
	rgb_color whiteColor = { 255, 255, 255, 255};
	

	if ((flags & B_DISABLED) != 0) {
		_DrawFrame(view, rect, grayColor, grayColor, grayColor, grayColor, borders);
	} else if ((flags & B_CLICKED) != 0) {
		_DrawFrame(view, rect, blueColor, blueColor, blueColor, blueColor, borders);
		_DrawFrame(view, rect, blueColor, blueColor, blueColor, blueColor, borders);
	} else {
		
		_DrawFrame(view, rect, grayColor, grayColor, grayColor, grayColor, borders);
		_DrawFrame(view, rect, whiteColor, whiteColor, whiteColor, whiteColor, borders);
	}
	
}

// #pragma mark -

void
WinXControlLook::DrawBorder(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, border_style borderStyle, uint32 flags,
	uint32 borders)
{
	if (borderStyle == B_NO_BORDER)
		return;

	rgb_color grayColor = { 122, 122, 122, 255};
	rgb_color blueColor = { 0, 120, 215, 255};
	
	rgb_color scrollbarFrameColor = grayColor;
	if ((flags & B_FOCUSED) != 0)
		scrollbarFrameColor = blueColor;

	if (borderStyle == B_FANCY_BORDER)
		_DrawFrame(view, rect, blueColor, blueColor, blueColor, blueColor, borders);

	_DrawFrame(view, rect, scrollbarFrameColor, scrollbarFrameColor,
		scrollbarFrameColor, scrollbarFrameColor, borders);
}

void
WinXControlLook::DrawRaisedBorder(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders)
{
	rgb_color lightColor;
	rgb_color shadowColor;
	rgb_color grayColor = { 122, 122, 122, 255};


	if ((flags & B_DISABLED) != 0) {
		lightColor = base;
		shadowColor = base;
	} else {
		lightColor = grayColor;
		shadowColor = grayColor;
	}

	_DrawFrame(view, rect, lightColor, lightColor, shadowColor, shadowColor,
		borders);
}

void
WinXControlLook::GetFrameInsets(frame_type frameType, uint32 flags, float& _left,
	float& _top, float& _right, float& _bottom)
{
	// All frames have the same inset on each side.
	float inset = 0;

	switch (frameType) {
		case B_BUTTON_FRAME:
			inset = (flags & B_DEFAULT_BUTTON) != 0 ? 5 : 2;
			break;
		case B_GROUP_FRAME:
		case B_MENU_FIELD_FRAME:
			inset = 3;
			break;
		case B_SCROLL_VIEW_FRAME:
		case B_TEXT_CONTROL_FRAME:
			inset = 1;
			break;
	}

	_left = inset;
	_top = inset;
	_right = inset;
	_bottom = inset;
}


void
WinXControlLook::GetBackgroundInsets(background_type backgroundType,
	uint32 flags, float& _left, float& _top, float& _right, float& _bottom)
{
	// Most backgrounds have the same inset on each side.
	float inset = 0;

	switch (backgroundType) {
		case B_BUTTON_BACKGROUND:
		case B_MENU_BACKGROUND:
		case B_MENU_BAR_BACKGROUND:
		case B_MENU_FIELD_BACKGROUND:
		case B_MENU_ITEM_BACKGROUND:
			inset = 1;
			break;
		case B_BUTTON_WITH_POP_UP_BACKGROUND:
			_left = 1;
			_top = 1;
			_right = 1 + kButtonPopUpIndicatorWidth;
			_bottom = 1;
			return;
		case B_HORIZONTAL_SCROLL_BAR_BACKGROUND:
			_left = 2;
			_top = 0;
			_right = 1;
			_bottom = 0;
			return;
		case B_VERTICAL_SCROLL_BAR_BACKGROUND:
			_left = 0;
			_top = 2;
			_right = 0;
			_bottom = 1;
			return;
	}

	_left = inset;
	_top = inset;
	_right = inset;
	_bottom = inset;
}

// #pragma mark -


void
WinXControlLook::_DrawAddonButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	bool popupIndicator, uint32 flags, uint32 borders, orientation orientation)
{
	if (!rect.IsValid())
		return;

	rgb_color rgbButtonActivatedBackground = { 229, 241, 251, 255};

	view->PushState();

	// set clipping constraints to updateRect
	BRegion clipping(updateRect);
	view->ConstrainClippingRegion(&clipping);
	
	if ((flags & B_ACTIVATED) != 0) {
		view->SetHighColor(rgbButtonActivatedBackground);
	} else {
		view->SetHighColor(base);
	}
	

	
	view->FillRect(rect, B_SOLID_HIGH);
	

	view->PopState();
}


void
WinXControlLook::_DrawAddonButtonFrame(BView* view, BRect& rect,
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
	

	
	rgb_color rgbButtonDefaultFrame = { 0, 120, 215, 255};
	rgb_color rgbButtonNormalFrame = {  122, 122, 122, 255};
	rgb_color rgbButtonActivatedFrame = { 0, 120, 215, 255};
	rgb_color rgbButtonInactiveFrame = { 173, 173, 173, 255};


	view->SetLowColor(background);
	view->FillRect(rect, B_SOLID_LOW);
	
	if ((flags & B_DEFAULT_BUTTON) != 0) {
		_DrawFrame(view, rect, background, background, background,
			background, borders);
		_DrawFrame(view, rect, background, background, background,
			background, borders);
		_DrawFrame(view, rect, rgbButtonDefaultFrame, rgbButtonDefaultFrame,
			 rgbButtonDefaultFrame,  rgbButtonDefaultFrame, borders);	
		_DrawFrame(view, rect, rgbButtonDefaultFrame, rgbButtonDefaultFrame,
			 rgbButtonDefaultFrame,  rgbButtonDefaultFrame, borders);		  	 
	} else	if ((flags & B_DISABLED) != 0) {
		_DrawFrame(view, rect, rgbButtonInactiveFrame, rgbButtonInactiveFrame,
			rgbButtonInactiveFrame, rgbButtonInactiveFrame, borders);
	} else if ((flags & B_ACTIVATED) != 0) {
		_DrawFrame(view, rect,rgbButtonActivatedFrame, rgbButtonActivatedFrame,
			rgbButtonActivatedFrame, rgbButtonActivatedFrame, borders);
	} else {
		_DrawFrame(view, rect, rgbButtonNormalFrame, rgbButtonNormalFrame,
			 rgbButtonNormalFrame,  rgbButtonNormalFrame, borders);
	}

	//view->FillRect(rect, B_SOLID_HIGH);
	//rect.InsetBy(1,1);
	
	/*
	if ((flags & B_ACTIVATED) != 0) {
		_DrawFrame(view, rect, rgbButtonLowColor, rgbButtonLowColor,
			 rgbButtonHighColor,  rgbButtonHighColor, borders);
		_DrawFrame(view, rect, rgbButtonLowColor, rgbButtonLowColor,
			 rgbButtonHighColor,  rgbButtonHighColor, borders);
	}
	else {
		_DrawFrame(view, rect, rgbButtonHighColor, rgbButtonHighColor,
			 	rgbButtonLowColor,  rgbButtonLowColor, borders);
		_DrawFrame(view, rect, rgbButtonHighColor, rgbButtonHighColor,
			 	rgbButtonLowColor,  rgbButtonLowColor, borders);
	}
	*/
	
	view->PopState();
}

// #pragma mark - Protected methods

void
WinXControlLook::_DrawAddonMenuFieldBackgroundOutside(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	bool popupIndicator, uint32 flags)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	if (popupIndicator) {
		//BRect leftRect(rect);
		//leftRect.right -= 10;

		BRect rightRect(rect);
		rightRect.left = rightRect.right - 10;

		/*
		_DrawAddonMenuFieldBackgroundInside(view, leftRect, updateRect,
			leftTopRadius, 0.0f, leftBottomRadius, 0.0f, base, flags,
			B_LEFT_BORDER | B_TOP_BORDER | B_BOTTOM_BORDER);

		_DrawAddonMenuFieldBackgroundInside(view, rightRect, updateRect,
			0.0f, rightTopRadius, 0.0f, rightBottomRadius, base, flags,
			B_TOP_BORDER | B_RIGHT_BORDER | B_BOTTOM_BORDER);
		*/
		
		_DrawAddonMenuFieldBackgroundInside(view, rect, updateRect, leftTopRadius,
			rightTopRadius, leftBottomRadius, rightBottomRadius, base, flags);	

		_DrawPopUpMarker(view, rightRect, base, flags);

		// draw a line on the left of the popup frame
		/*
		rgb_color bevelShadowColor = _BevelShadowColor(base, flags);
		view->SetHighColor(bevelShadowColor);
		BPoint leftTopCorner(floorf(rightRect.left - 1.0),
			floorf(rightRect.top - 1.0));
		BPoint leftBottomCorner(floorf(rightRect.left - 1.0),
			floorf(rightRect.bottom + 1.0));
		view->StrokeLine(leftTopCorner, leftBottomCorner);
		*/

		rect.right -=10;
	} else {
		_DrawAddonMenuFieldBackgroundInside(view, rect, updateRect, leftTopRadius,
			rightTopRadius, leftBottomRadius, rightBottomRadius, base, flags);
	}
}


void
WinXControlLook::_DrawAddonMenuFieldBackgroundInside(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders)
{
	if (!rect.IsValid() || !rect.Intersects(updateRect))
		return;

	// save the clipping constraints of the view
	view->PushState();
	
	/*
	// set clipping constraints to updateRect
	BRegion clipping(updateRect);
	view->ConstrainClippingRegion(&clipping);

	// frame colors
	rgb_color frameLightColor  = _FrameLightColor(base, flags);
	rgb_color frameShadowColor = _FrameShadowColor(base, flags);

	// indicator background color
	rgb_color indicatorBase;
	if ((borders & B_LEFT_BORDER) != 0)
		indicatorBase = base;
	else {
		if ((flags & B_DISABLED) != 0)
			indicatorBase = tint_color(base, 1.05);
		else
			indicatorBase = tint_color(base, 1.12);
	}

	// bevel colors
	rgb_color cornerColor = tint_color(indicatorBase, 0.85);
	rgb_color bevelColor1 = tint_color(indicatorBase, 0.3);
	rgb_color bevelColor2 = tint_color(indicatorBase, 0.5);
	rgb_color bevelColor3 = tint_color(indicatorBase, 1.03);

	if ((flags & B_DISABLED) != 0) {
		cornerColor = tint_color(indicatorBase, 0.8);
		bevelColor1 = tint_color(indicatorBase, 0.7);
		bevelColor2 = tint_color(indicatorBase, 0.8);
		bevelColor3 = tint_color(indicatorBase, 1.01);
	} else {
		cornerColor = tint_color(indicatorBase, 0.85);
		bevelColor1 = tint_color(indicatorBase, 0.3);
		bevelColor2 = tint_color(indicatorBase, 0.5);
		bevelColor3 = tint_color(indicatorBase, 1.03);
	}

	// surface top gradient
	BGradientLinear fillGradient;
	_MakeButtonGradient(fillGradient, rect, indicatorBase, flags);

	// rounded corners

	if ((borders & B_LEFT_BORDER) != 0 && (borders & B_TOP_BORDER) != 0
		&& leftTopRadius > 0) {
		// draw left top rounded corner
		BRect leftTopCorner(floorf(rect.left), floorf(rect.top),
			floorf(rect.left + leftTopRadius - 2.0),
			floorf(rect.top + leftTopRadius - 2.0));
		clipping.Exclude(leftTopCorner);

		BRegion cornerClipping(leftTopCorner);
		view->ConstrainClippingRegion(&cornerClipping);

		BRect ellipseRect(leftTopCorner);
		ellipseRect.InsetBy(-1.0, -1.0);
		ellipseRect.right = ellipseRect.left + ellipseRect.Width() * 2;
		ellipseRect.bottom = ellipseRect.top + ellipseRect.Height() * 2;

		// draw the frame (again)
		view->SetHighColor(frameLightColor);
		view->FillEllipse(ellipseRect);

		// draw the bevel and background
		_DrawRoundCornerBackgroundLeftTop(view, leftTopCorner, updateRect,
			bevelColor1, fillGradient);
	}

	if ((borders & B_TOP_BORDER) != 0 && (borders & B_RIGHT_BORDER) != 0
		&& rightTopRadius > 0) {
		// draw right top rounded corner
		BRect rightTopCorner(floorf(rect.right - rightTopRadius + 2.0),
			floorf(rect.top), floorf(rect.right),
			floorf(rect.top + rightTopRadius - 2.0));
		clipping.Exclude(rightTopCorner);

		BRegion cornerClipping(rightTopCorner);
		view->ConstrainClippingRegion(&cornerClipping);

		BRect ellipseRect(rightTopCorner);
		ellipseRect.InsetBy(-1.0, -1.0);
		ellipseRect.left = ellipseRect.right - ellipseRect.Width() * 2;
		ellipseRect.bottom = ellipseRect.top + ellipseRect.Height() * 2;

		// draw the frame (again)
		if (frameLightColor == frameShadowColor) {
			view->SetHighColor(frameLightColor);
			view->FillEllipse(ellipseRect);
		} else {
			BGradientLinear gradient;
			gradient.AddColor(frameLightColor, 0);
			gradient.AddColor(frameShadowColor, 255);
			gradient.SetStart(rightTopCorner.LeftTop());
			gradient.SetEnd(rightTopCorner.RightBottom());
			view->FillEllipse(ellipseRect, gradient);
		}

		// draw the bevel and background
		_DrawRoundCornerBackgroundRightTop(view, rightTopCorner, updateRect,
			bevelColor1, bevelColor3, fillGradient);
	}

	if ((borders & B_LEFT_BORDER) != 0 && (borders & B_BOTTOM_BORDER) != 0
		&& leftBottomRadius > 0) {
		// draw left bottom rounded corner
		BRect leftBottomCorner(floorf(rect.left),
			floorf(rect.bottom - leftBottomRadius + 2.0),
			floorf(rect.left + leftBottomRadius - 2.0),
			floorf(rect.bottom));
		clipping.Exclude(leftBottomCorner);

		BRegion cornerClipping(leftBottomCorner);
		view->ConstrainClippingRegion(&cornerClipping);

		BRect ellipseRect(leftBottomCorner);
		ellipseRect.InsetBy(-1.0, -1.0);
		ellipseRect.right = ellipseRect.left + ellipseRect.Width() * 2;
		ellipseRect.top = ellipseRect.bottom - ellipseRect.Height() * 2;

		// draw the frame (again)
		if (frameLightColor == frameShadowColor) {
			view->SetHighColor(frameLightColor);
			view->FillEllipse(ellipseRect);
		} else {
			BGradientLinear gradient;
			gradient.AddColor(frameLightColor, 0);
			gradient.AddColor(frameShadowColor, 255);
			gradient.SetStart(leftBottomCorner.LeftTop());
			gradient.SetEnd(leftBottomCorner.RightBottom());
			view->FillEllipse(ellipseRect, gradient);
		}

		// draw the bevel and background
		_DrawRoundCornerBackgroundLeftBottom(view, leftBottomCorner,
			updateRect, bevelColor2, bevelColor3, fillGradient);
	}

	if ((borders & B_RIGHT_BORDER) != 0 && (borders & B_BOTTOM_BORDER) != 0
		&& rightBottomRadius > 0) {
		// draw right bottom rounded corner
		BRect rightBottomCorner(floorf(rect.right - rightBottomRadius + 2.0),
			floorf(rect.bottom - rightBottomRadius + 2.0), floorf(rect.right),
			floorf(rect.bottom));
		clipping.Exclude(rightBottomCorner);

		BRegion cornerClipping(rightBottomCorner);
		view->ConstrainClippingRegion(&cornerClipping);

		BRect ellipseRect(rightBottomCorner);
		ellipseRect.InsetBy(-1.0, -1.0);
		ellipseRect.left = ellipseRect.right - ellipseRect.Width() * 2;
		ellipseRect.top = ellipseRect.bottom - ellipseRect.Height() * 2;

		// draw the frame (again)
		view->SetHighColor(frameShadowColor);
		view->FillEllipse(ellipseRect);

		// draw the bevel and background
		_DrawRoundCornerBackgroundRightBottom(view, rightBottomCorner,
			updateRect, bevelColor3, fillGradient);
	}

	// clip out the corners
	view->ConstrainClippingRegion(&clipping);

	// draw the bevel
	_DrawFrame(view, rect,
		bevelColor2, bevelColor1,
		bevelColor3, bevelColor3,
		cornerColor, cornerColor,
		borders);
	*/
	
	view->SetLowColor(base);
	// fill in the background
	view->FillRect(rect, B_SOLID_LOW);

	// restore the clipping constraints of the view
	view->PopState();
}




//-----

extern "C" BControlLook* (instantiate_control_look)(image_id id)
{
	return new (std::nothrow)WinXControlLook(id);
}

