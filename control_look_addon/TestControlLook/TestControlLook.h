/*
 * Copyright 2019 s40in
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		s40in, Sergei Sorokin
 */
#ifndef TEST_CONTROL_LOOK_H
#define TEST_CONTROL_LOOK_H


#include <ControlLook.h>

#include "HaikuControlLook.h"


class BBitmap;
class BControl;
class BGradientLinear;
class BView;

using BPrivate::HaikuControlLook;


class TestControlLook : public HaikuControlLook {
public:
								TestControlLook(image_id id);
	virtual						~TestControlLook();


	//  --- Menus ---
	virtual	void				DrawMenuBarBackground(BView* view, BRect& rect,
									const BRect& updateRect,
									const rgb_color& base,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS);

	virtual	void				DrawMenuItemBackground(BView* view,
									BRect& rect, const BRect& updateRect,
									const rgb_color& base, uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS);
	
	// --- Buttons ---								
	virtual	void				DrawButtonFrame(BView* view, BRect& rect,
									const BRect& updateRect,
									const rgb_color& base,
									const rgb_color& background,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS);
	virtual	void				DrawButtonFrame(BView* view, BRect& rect,
									const BRect& updateRect,
									float radius,
									const rgb_color& base,
									const rgb_color& background,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS);
	virtual	void				DrawButtonFrame(BView* view, BRect& rect,
									const BRect& updateRect,
									float leftTopRadius,
									float rightTopRadius,
									float leftBottomRadius,
									float rightBottomRadius,
									const rgb_color& base,
									const rgb_color& background,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS);
									
	virtual	void				DrawButtonBackground(BView* view, BRect& rect,
									const BRect& updateRect,
									const rgb_color& base,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS,
									orientation orientation = B_HORIZONTAL);
	virtual	void				DrawButtonBackground(BView* view, BRect& rect,
									const BRect& updateRect,
									float radius,
									const rgb_color& base,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS,
									orientation orientation = B_HORIZONTAL);
	virtual	void				DrawButtonBackground(BView* view, BRect& rect,
									const BRect& updateRect,
									float leftTopRadius,
									float rightTopRadius,
									float leftBottomRadius,
									float rightBottomRadius,
									const rgb_color& base,
									uint32 flags = 0,
									uint32 borders = B_ALL_BORDERS,
									orientation orientation = B_HORIZONTAL);
									

};


#endif	// TEST_CONTROL_LOOK_H
