// ================================================================================
// == This file is a part of Tinkerbell UI Toolkit. (C) 2011-2012, Emil Seger�s ==
// ==                   See tinkerbell.h for more information.                   ==
// ================================================================================

#include "tb_style_edit.h"
#include "tb_style_edit_content.h"
#include <assert.h>

namespace tinkerbell {

// == TBTextFragmentContentFactory ==========================================================================

int TBTextFragmentContentFactory::GetContent(const char *text)
{
	if (text[0] == '<')
	{
		int i = 0;
		while (text[i] != '>' && text[i] > 31)
			i++;
		if (text[i] == '>')
		{
			i++;
			return i;
		}
	}
	return 0;
}

TBTextFragmentContent *TBTextFragmentContentFactory::CreateFragmentContent(const char *text, int text_len)
{
	if (strncmp(text, "<hr>", text_len) == 0)
		return new PHorizontalLineContent(100, 2, TBColor(255, 255, 255, 64));
	else if (strncmp(text, "<u>", text_len) == 0)
		return new TBTextFragmentContentUnderline();
	else if (strncmp(text, "<color ", MIN(text_len, 7)) == 0)
	{
		TBColor color;
		color.SetFromString(text + 7, text_len - 8);
		return new TBTextFragmentContentTextColor(color);
	}
	else if (strncmp(text, "</", MIN(text_len, 2)) == 0)
		return new TBTextFragmentContentStylePop();
	return nullptr;
}

// == PHorizontalLineContent ================================================================================

PHorizontalLineContent::PHorizontalLineContent(int32 width_in_percent, int32 height, const TBColor &color)
	: width_in_percent(width_in_percent)
	, height(height)
	, color(color)
{
}

void PHorizontalLineContent::Paint(TBTextFragment *fragment, int32 translate_x, int32 translate_y, TBTextProps *props)
{
	int x = translate_x + fragment->xpos;
	int y = translate_y + fragment->ypos;

	int w = fragment->block->styledit->layout_width * width_in_percent / 100;
	x += (fragment->block->styledit->layout_width - w) / 2;

	TBStyleEditListener *listener = fragment->block->styledit->listener;
	listener->DrawRectFill(TBRect(x, y, w, height), color);
}

int32 PHorizontalLineContent::GetWidth(TBFontFace *font, TBTextFragment *fragment) { return MAX(fragment->block->styledit->layout_width, 0); }

int32 PHorizontalLineContent::GetHeight(TBFontFace *font, TBTextFragment *fragment) { return height; }

// ============================================================================

void TBTextFragmentContentUnderline::Paint(TBTextFragment *fragment, int32 translate_x, int32 translate_y, TBTextProps *props)
{
	if (TBTextProps::Data *data = props->Push())
		data->underline = true;
}

void TBTextFragmentContentTextColor::Paint(TBTextFragment *fragment, int32 translate_x, int32 translate_y, TBTextProps *props)
{
	if (TBTextProps::Data *data = props->Push())
		data->text_color = color;
}

void TBTextFragmentContentStylePop::Paint(TBTextFragment *fragment, int32 translate_x, int32 translate_y, TBTextProps *props)
{
	props->Pop();
}

}; // namespace tinkerbell