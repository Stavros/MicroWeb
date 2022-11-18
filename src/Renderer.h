//
// Copyright (C) 2021 James Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Font.h"
class App;
struct Widget;

#define MAX_STATUS_LENGTH 80

class Renderer
{
public:
	Renderer(App& inApp);

	void Init();
	void Reset();
	void Update();
	void Scroll(int delta);
	void ScrollTo(int position);
	void ScrollTo(Widget* widget);

	void RedrawScrollBar();
	int GetMaxScrollPosition();
	int GetScrollPosition() { return scrollPosition; }

	Widget* PickPageWidget(int x, int y);
	bool IsOverWidget(Widget* widget, int x, int y);

	void SetStatus(const char* status);
	void SetTitle(const char* status);

	void RenderWidget(Widget* widget);
	void RedrawWidget(Widget* widget);
	void InvertWidget(Widget* widget);

	void RedrawModifiedTextField(Widget* widget, int position);
	void DrawTextFieldCursor(Widget* widget, int position, bool clear = false);

	int GetPageTopWidgetIndex() { return pageTopWidgetIndex; }
private:
	void DrawTruncatedString(char* message, int x, int y, int maxTextWidth, int size = 1, FontStyle::Type style = FontStyle::Regular);
	void DrawButtonRect(int x, int y, int width, int height, bool isSelected = false);
	void DrawRect(int x, int y, int width, int height);

	void RenderWidgetInternal(Widget* widget, int baseY);
	void DrawTextFieldCursorInternal(Widget* widget, int position, bool clear, int baseY);

	void BeginWidgetDraw(Widget* widget, int& baseY);
	void EndWidgetDraw();

	App& app;
	int scrollPosition;
	int pageTopWidgetIndex;
	char statusMessage[MAX_STATUS_LENGTH];
	int upperRenderLine, lowerRenderLine;
};

#endif
