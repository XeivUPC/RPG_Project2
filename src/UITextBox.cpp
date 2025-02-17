#include "UITextBox.h"

#include "Engine.h"
#include "DrawingTools.h"

UITextBox::UITextBox(string _text,_TTF_Font& _font, int _fontSize, SDL_Color _color, Vector2Int _position, Vector2Int _size, Vector2 _pivot, HorizontalAlignment _hAlignment, VerticalAlignment _vAlignment, bool _wrap) : UIElement(_position, _size, _pivot), fontSize(_fontSize), horizontalAligment(_hAlignment), verticalAligment(_vAlignment), wrap(_wrap), color(_color)
{
	text = move(_text);
	ChangeFont(_font);
}

UITextBox::~UITextBox()
{
}

void UITextBox::UpdateElement()
{
	UIElement::UpdateElement();
}

void UITextBox::RenderElement()
{
	if(text.length()>0)
		Engine::Instance().m_render->painter().RenderText(text, *font, fontSize, position, size, { scale, scale }, (int)horizontalAligment, (int)verticalAligment, wrap, pivot,color);

	if (debug)
		RenderElementDebug();
}

void UITextBox::RenderElementDebug()
{
	Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, false, { 255,255,0,255 });
}

void UITextBox::ChangeFont(_TTF_Font& _font)
{
	font = &_font;
}

TTF_Font* UITextBox::GetFont()
{
	return font;
}
