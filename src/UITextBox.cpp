#include "UITextBox.h"

#include "Engine.h"
#include "TextureFactory.h"
#include "DrawingTools.h"

UITextBox::UITextBox(string _text,_TTF_Font& _font, int _fontSize, SDL_Color _color, Vector2Int _position, Vector2Int _size, Vector2 _pivot, HorizontalAlignment _hAlignment, VerticalAlignment _vAlignment, bool _wrap) : UIElement(_position, _size, _pivot), fontSize(_fontSize), horizontalAligment(_hAlignment), verticalAligment(_vAlignment), wrap(_wrap), color(_color)
{
	SetText(move(_text));
	SetFont(_font);
}

UITextBox::~UITextBox()
{
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}

	for (size_t i = 0; i < childs.size(); i++)
	{
		delete childs[i];
	}
	childs.clear();
}

void UITextBox::UpdateElement()
{
	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->UpdateElement();
	}

	UIElement::UpdateElement();
}

void UITextBox::RenderElement()
{
	if (!visible)
		return;
	if (text.length() > 0) {
		if (dirty){
			if(texture!=nullptr)
				SDL_DestroyTexture(texture);
			texture = Engine::Instance().m_render->painter().RenderText(text, *font, fontSize, position, size, true, { scale, scale }, (int)horizontalAligment, (int)verticalAligment, wrap, pivot, color);
			dirty = false;
		}
		else
			Engine::Instance().m_render->painter().RenderInsideBox(*texture,position, size, { scale, scale }, (int)horizontalAligment, (int)verticalAligment,pivot);
	}
	else
		dirty = false;

	for (size_t i = 0; i < childs.size(); i++)
	{
		childs[i]->RenderElement();
	}

	if (debug)
		RenderElementDebug();
}

void UITextBox::RenderElementDebug()
{
	Engine::Instance().m_render->painter().RenderBox(position, size, { scale,scale }, pivot, false, { 255,255,0,255 });
}

const string& UITextBox::GetText()
{
	return text;
}

string& UITextBox::GetModificableText()
{
	dirty = true;
	return text;
}

void UITextBox::SetText(string _text)
{
	if (text == _text)
		return;
	text = _text;
	dirty = true;
}

const int& UITextBox::GetFontSize()
{
	return fontSize;
}

void UITextBox::SetFontSize(int _fontSize)
{
	fontSize = _fontSize;
	dirty = true;
}

const bool& UITextBox::GetIfWrap()
{
	return wrap;
}

void UITextBox::SetWrap(bool _wrap)
{
	wrap = _wrap;
	dirty = true;
}

const SDL_Color& UITextBox::GetColor()
{
	return color;
}

void UITextBox::SetColor(SDL_Color _color)
{
	color = _color;
	dirty = true;
}

_TTF_Font& UITextBox::GetFont()
{
	return *font;
}

void UITextBox::SetFont(_TTF_Font& _font)
{
	font = &_font;
	dirty = true;
}

const UITextBox::HorizontalAlignment& UITextBox::GetHorizontalAligment()
{
	return horizontalAligment;
}

void UITextBox::SetHorizontalAligment(HorizontalAlignment _horizontalAligment)
{
	horizontalAligment = _horizontalAligment;
	dirty = true;
}

const UITextBox::VerticalAlignment& UITextBox::GetVerticalAligment()
{
	return verticalAligment;
}

void UITextBox::SetVerticalAligment(VerticalAlignment _verticalAligment)
{
	verticalAligment = _verticalAligment;
	dirty = true;
}

void UITextBox::SetDirty()
{
	dirty = true;
}

bool UITextBox::GetIfDirty()
{
	return dirty;
}

