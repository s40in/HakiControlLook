/*
 * Copyright 2019 s40in. 
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */





#include "HakiloControlLook2.h"

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


HakiloControlLook2::HakiloControlLook2(image_id id)
	: HaikuControlLook()
{
}


HakiloControlLook2::~HakiloControlLook2()
{
}


void
HakiloControlLook2::DrawMenuBarBackground(BView* view, BRect& rect,
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
HakiloControlLook2::DrawMenuItemBackground(BView* view, BRect& rect,
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
HakiloControlLook2::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{
	if ((flags & B_FLAT) != 0 )	{
		_DrawButtonFrame(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
			background, 1.0, 1.0, flags, borders);
		return;
	}
	else{
	// Draw Hakilo button with new background
	_DrawHakiloButton(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f, base,
			background, 1.0, 1.0, flags, borders);
	
	}
			
			
}


// I dont' know who call this function :)
void
HakiloControlLook2::DrawButtonFrame(BView* view, BRect& rect, const BRect& updateRect,
	float radius, const rgb_color& base, const rgb_color& background, uint32 flags,
	uint32 borders)
{ 
	_DrawButtonFrame(view, rect, updateRect, radius, radius, radius, radius,
		base, background, 1.0, 1.0, flags, borders);
}


// I dont' know who call this function :)
void
HakiloControlLook2::DrawButtonFrame(BView* view, BRect& rect,
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
HakiloControlLook2::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	
	// If the button is flat and neither activated nor otherwise highlighted
	// (mouse hovering or focussed), draw it flat.
	if ((flags & B_FLAT) != 0) {
		_DrawButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
	
	
	} else {
		_DrawHakiloButtonBackground(view, rect, updateRect, 0.0f, 0.0f, 0.0f, 0.0f,
		base, false, flags, borders, orientation);
		
	}
	
	
}


// I dont' know who call this function :)
void
HakiloControlLook2::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float radius, const rgb_color& base, uint32 flags,
	uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, radius, radius, radius,
		radius, base, false, flags, borders, orientation);
}


// I dont' know who call this function :)
void
HakiloControlLook2::DrawButtonBackground(BView* view, BRect& rect,
	const BRect& updateRect, float leftTopRadius, float rightTopRadius,
	float leftBottomRadius, float rightBottomRadius, const rgb_color& base,
	uint32 flags, uint32 borders, orientation orientation)
{
	_DrawButtonBackground(view, rect, updateRect, leftTopRadius,
		rightTopRadius, leftBottomRadius, rightBottomRadius, base, false, flags,
		borders, orientation);
}


void
HakiloControlLook2::DrawSliderThumb(BView* view, BRect& rect, const BRect& updateRect,
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
HakiloControlLook2::DrawScrollViewFrame(BView* view, BRect& rect,
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



//-------------------------------------------------

// #pragma mark -


void
HakiloControlLook2::_DrawHakiloButtonBackground(BView* view, BRect& rect,
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

//	view->SetLowColor(base);
//	view->FillRect(rect, B_SOLID_LOW);
	
	rgb_color buttonBgColor;
	rgb_color buttonFrameColor = { 0, 0, 0, 255};
		
	
	if ((flags & B_DISABLED) != 0) {
		buttonBgColor = base;
	}
	else {
		buttonBgColor = (rgb_color) {240, 240, 240, 255};
	}
	view->SetHighColor(buttonBgColor);
	view->FillRoundRect(rect, 2.0, 2.0);
		
//	view->SetHighColor(buttonFrameColor);
//	view->StrokeRoundRect(rect, 2.0, 2.0);


	// restore the clipping constraints of the view
	view->PopState();
}


void
HakiloControlLook2::_DrawHakiloButton(BView* view, BRect& rect,
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

	rgb_color buttonFrameColor = { 0, 0, 0, 255};
	rgb_color buttonShadowHighColor = { 184, 184, 184, 255};
	rgb_color buttonShadowLowColor = { 152, 152, 152, 255};
	rgb_color markColor= {103, 103, 200, 255};
	
	if ((flags & B_DEFAULT_BUTTON) != 0) {
		rect.InsetBy(1,1);
	//	view->SetLowColor(base);
	//	view->SetHighColor(markColor);
	//	view->FillRoundRect(rect, 2.0, 2.0);
		rect.InsetBy(2,2);
	}
	
	rect.right -= 2;
	rect.bottom -= 2;
	
	BRect rectShadowLow(rect);
	rectShadowLow.left += 1;
	rectShadowLow.top += 1;
	rectShadowLow.right += 1;
	rectShadowLow.bottom += 1;
		
	BRect rectShadowHigh(rect);
	rectShadowHigh.left += 2;
	rectShadowHigh.top += 2;
	rectShadowHigh.right += 2;
	rectShadowHigh.bottom += 2;	
	
	view->SetLowColor(base);
	
	
	// button background color and shadow
	rgb_color buttonBgColor;
	if ((flags & B_DISABLED) != 0) {
		buttonBgColor = base;
		
		view->SetHighColor(base);
		view->FillRoundRect(rectShadowHigh, 2.0, 2.0);
		view->FillRoundRect(rectShadowLow, 2.0, 2.0);
	}
	else {
		buttonBgColor = (rgb_color) {240, 240, 240, 255};
		
		view->SetHighColor(buttonShadowHighColor);
		view->FillRoundRect(rectShadowHigh, 2.0, 2.0);
		
		view->SetHighColor(buttonShadowLowColor);
		view->FillRoundRect(rectShadowLow, 2.0, 2.0);
	}

	// clip out the corners
	view->ConstrainClippingRegion(&clipping);

	if ((flags & B_ACTIVATED) != 0) {
		//buttonBgColor = buttonActivatedColor;
		buttonBgColor =  (rgb_color) {240, 240, 240, 255};
		
		
		if ((flags & B_DEFAULT_BUTTON) != 0) {
			view->SetHighColor(markColor);
		}
		else {
			view->SetHighColor(base);
		}
		
		//view->SetHighColor(base);
		view->FillRoundRect(rectShadowHigh, 2.0, 2.0);
		view->FillRoundRect(rectShadowLow, 2.0, 2.0);
	} 
	
	// move to draw _HakiloButtonBackground
	//---view->SetHighColor(buttonBgColor);
	//---view->FillRoundRect(rect, 2.0, 2.0);
		
	view->SetHighColor(buttonFrameColor);
	view->StrokeRoundRect(rect, 2.0, 2.0);
	rect.InsetBy(1,1);

	// restore the clipping constraints of the view
	view->PopState();
}


void
HakiloControlLook2::DrawRadioButton(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
	rgb_color blackColor= {0,0,0,255};	
	rgb_color whiteColor= {255, 255, 255, 255};	
	rgb_color markColor= {103, 103, 200, 255};
	rgb_color ShadowHighColor= {184, 184, 184, 255};
	rgb_color ShadowLowColor= {136, 136, 136, 255};	
	rgb_color borderColor;
	rgb_color backgroundColor;
	rgb_color navigationColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
	
	if ((flags & B_DISABLED) != 0) {
		backgroundColor = base;
		borderColor = blackColor;
	} else if ((flags & B_CLICKED) != 0) {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	} else {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	}
	
	if ((flags & B_FOCUSED) != 0) {
		borderColor = navigationColor;
	}
	
		
	BRect circleRect(rect);
	float circleR = (rect.right-rect.left)*0.64; //размер круга
	
	circleRect.left += 1;
	circleRect.right = circleRect.left + circleR;
	circleRect.top = rect.top + (rect.bottom - rect.top - circleR)/2;
	circleRect.bottom = circleRect.top + circleR;
	
	BRect circleShadowLowRect(circleRect);
	circleShadowLowRect.left += 1;
	circleShadowLowRect.top += 1;
	circleShadowLowRect.right += 1;
	circleShadowLowRect.bottom += 1;
	
	BRect circleShadowHighRect(circleRect);
	circleShadowHighRect.left += 2;
	circleShadowHighRect.top += 2;
	circleShadowHighRect.right += 2;
	circleShadowHighRect.bottom += 2;
	
	BRect markRect(circleRect);
	markRect.left += 3;
	markRect.top += 4 ;
	markRect.bottom -= 3;
	markRect.right += 3;
	
	BRect markShadowLowRect(markRect);
	markShadowLowRect.left += 1;
	markShadowLowRect.top += 1;
	markShadowLowRect.right += 1;
	markShadowLowRect.bottom += 1;
	
	BRect markShadowHighRect(markRect);
	markShadowHighRect.left += 2;
	markShadowHighRect.top += 2;
	markShadowHighRect.right += 2;
	markShadowHighRect.bottom += 2;	
	
	
	

	// shadow
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {	
	// mark shadow
	view->SetHighColor(ShadowHighColor);
	view->FillRoundRect(markShadowHighRect, 2.0, 2.0);
	
	view->SetHighColor(ShadowLowColor);
	view->FillRoundRect(markShadowLowRect, 2.0, 2.0);
	// end mark shadow

  }
  else {
  	// shadow circle
	view->SetHighColor(ShadowHighColor);
	view->FillEllipse(circleShadowHighRect);
	view->SetHighColor(ShadowLowColor);
	view->FillEllipse(circleShadowLowRect);
	// end shadow circle
  	
  }
	
	view->SetHighColor(backgroundColor);
	view->FillEllipse(circleRect);
	
	view->SetHighColor(borderColor);
	view->StrokeEllipse(circleRect);
	
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {	
		
	// mark
	view->SetHighColor(markColor);
	view->FillRoundRect(markRect, 2.0, 2.0);
	
	view->SetHighColor(borderColor);
	view->StrokeRoundRect(markRect, 2.0, 2.0);	
	// end mark
 }


}

void
HakiloControlLook2::DrawCheckBox(BView* view, BRect& rect, const BRect& updateRect,
	const rgb_color& base, uint32 flags)
{
	if (!rect.Intersects(updateRect))
		return;
		
	
	rgb_color blackColor= {0,0,0,255};	
	rgb_color whiteColor= {255, 255, 255, 255};	
	rgb_color markColor= {103, 103, 200, 255};
	rgb_color ShadowHighColor= {184, 184, 184, 255};
	rgb_color ShadowLowColor= {136, 136, 136, 255};	
	rgb_color borderColor;
	rgb_color backgroundColor;
	rgb_color navigationColor = ui_color(B_KEYBOARD_NAVIGATION_COLOR);
	
	if ((flags & B_DISABLED) != 0) {
		backgroundColor = base;
		borderColor = blackColor;
	} else if ((flags & B_CLICKED) != 0) {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	} else {
		backgroundColor = whiteColor;
		borderColor = blackColor;
	}
	
	if ((flags & B_FOCUSED) != 0) {
		borderColor = navigationColor;
	}
		
	BRect sqRect(rect);
	float sqR = (rect.right-rect.left)*0.59; //размер круга
	
	sqRect.left += 1;
	sqRect.right = sqRect.left + sqR;
	sqRect.top = rect.top + 3;
	sqRect.bottom = sqRect.top + sqR;
	
	BRect sqShadowLowRect(sqRect);
	sqShadowLowRect.left += 1;
	sqShadowLowRect.top += 1;
	sqShadowLowRect.right += 1;
	sqShadowLowRect.bottom += 1;
	
	BRect sqShadowHighRect(sqRect);
	sqShadowHighRect.left += 2;
	sqShadowHighRect.top += 2;
	sqShadowHighRect.right += 2;
	sqShadowHighRect.bottom += 2;
	
	
	BRect markRect(sqRect);
	markRect.left += 3;
	markRect.top -= 3 ;
	markRect.bottom -= 3;
	markRect.right += 3;
	
	BRect markShadowLowRect(markRect);
	markShadowLowRect.left += 1;
	markShadowLowRect.top += 1;
	markShadowLowRect.right += 1;
	markShadowLowRect.bottom += 1;
	
	BRect markShadowHighRect(markRect);
	markShadowHighRect.left += 2;
	markShadowHighRect.top += 2;
	markShadowHighRect.right += 2;
	markShadowHighRect.bottom += 2;	
	
		
		// shadow
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {	
	// mark shadow
	view->SetHighColor(ShadowHighColor);
	view->FillRoundRect(markShadowHighRect, 1.0, 1.0);
	view->SetHighColor(ShadowLowColor);
	view->FillRoundRect(markShadowLowRect, 1.0, 1.0);

	// end mark shadow

  }
  else {
  	// shadow box
	view->SetHighColor(ShadowHighColor);
	view->FillRoundRect(sqShadowHighRect, 1.0, 1.0);
	view->SetHighColor(ShadowLowColor);
	view->FillRoundRect(sqShadowLowRect, 1.0, 1.0);
	// end shadow box
  	
  }
		
	view->SetHighColor(backgroundColor);
	view->FillRoundRect(sqRect, 1.0, 1.0);
	
	view->SetHighColor(borderColor);
	view->StrokeRoundRect(sqRect, 1.0, 1.0);
	
  if (_RadioButtonAndCheckBoxMarkColor(base, markColor, flags)) {	
	// mark
	view->SetHighColor(markColor);
	view->FillRoundRect(markRect, 1.0, 1.0);
	
	view->SetHighColor(borderColor);
	view->StrokeRoundRect(markRect, 1.0, 1.0);	
	// end mark
 }
	
}



//-----

extern "C" BControlLook* (instantiate_control_look)(image_id id)
{
	return new (std::nothrow)HakiloControlLook2(id);
}

