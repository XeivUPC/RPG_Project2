#include "UIInputField.h"
#include "UIImage.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "DrawingTools.h"

UIInputField::UIInputField(string _defaultText, string _text, _TTF_Font& _font, int _fontSize, SDL_Color _color, SDL_Color _defaultColor, Vector2Int _position, Vector2Int _size, Vector2 _pivot, UITextBox::HorizontalAlignment _hAlignment, UITextBox::VerticalAlignment _vAlignment, bool _wrap) : UIElement(_position, _size, _pivot), defaultText(_defaultText), defaultColor(_defaultColor)
{
	textComponent = new UITextBox(move(_text), _font, _fontSize, move(_color), { 0,0 }, _size, pivot, _hAlignment, _vAlignment, _wrap);
	textComponent->SetParent(this);

}

UIInputField::~UIInputField()
{
	delete textComponent;
	//delete imageComponent;
}

void UIInputField::UpdateElement()
{
	//imageComponent->UpdateElement();
	textComponent->UpdateElement();
	UIElement::UpdateElement();

	ModuleInput* input = Engine::Instance().m_input;
	KeyState mouseClickState = input->GetMouseButtonDown(SDL_BUTTON_LEFT);

	if (!isSelected) {
		if (isMouseOver)
		{
			if (mouseClickState == KEY_DOWN) {
				isSelected = true;
				SDL_StartTextInput();
			}
		}
	}
	else {
		if (mouseClickState == KEY_DOWN) {
			if (isMouseOver) {

			}
			else {
				isSelected = false;
				writerPointer = -1;
				SDL_StopTextInput();
			}
		}	
	}

	if (isSelected)
	{
		if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {	
			int timeToWait = movingSpeedMS;
			if (movingCount == 1) {
				timeToWait = movingSaveSpeedMS;
			}
			if (movingTimer.ReadMSec() > timeToWait) {
				movingCount++;
				writerPointer--;
				if (writerPointer < -1)
					writerPointer = -1;
				movingTimer.StartTimer();
			}
		}
		else if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {	
			int timeToWait = movingSpeedMS;
			if (movingCount == 1) {
				timeToWait = movingSaveSpeedMS;
			}
			if (movingTimer.ReadMSec() > timeToWait) {
				movingCount++;
				writerPointer++;
				if (writerPointer > (int)textComponent->text.length()-1)
					writerPointer = (int)textComponent->text.length() - 1;
				movingTimer.StartTimer();
			}
		}else 
			movingCount=0;



		if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			writerPointer++;
			textComponent->text.insert(writerPointer, 1, '\n');
		}

		const string& inputRef = input->GetTextInput();
		for (size_t i = 0; i < inputRef.length(); i++)
		{
			writerPointer++;
			textComponent->text.insert(writerPointer, 1, inputRef[i]);
		}
		
		if (input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_REPEAT) {
			int timeToWait = backspaceRemovingSpeedMS;
			if (backspaceCount==1) {
				timeToWait = backspaceSaveRemovingSpeedMS;
			}
			if (backspaceTimer.ReadMSec() > timeToWait){
				if (writerPointer < textComponent->text.length() && writerPointer != -1) {
					textComponent->text.erase(writerPointer, 1);
					writerPointer--;
					if (writerPointer < -1)
						writerPointer = -1;
				}
				backspaceTimer.StartTimer();
				backspaceCount++;
			}
		}
		else
			backspaceCount = 0;

		
	}
}

void UIInputField::RenderElement()
{
	//imageComponent->RenderElement();
	
	if (textComponent->text.length() == 0 && !isSelected) {
		Engine::Instance().m_render->painter().RenderText(defaultText, *textComponent->GetFont(), textComponent->fontSize, position, size, { scale, scale }, (int)textComponent->horizontalAligment, (int)textComponent->verticalAligment, textComponent->wrap, pivot, defaultColor);
	}
	else if (isSelected) {

		std::string maskedText = textComponent->text;
		int index = writerPointer;
		Vector2Int drawPointerPos = position;
		drawPointerPos.x -= 3;

		if (index == -1)
		{
			maskedText = "|";
			drawPointerPos.x += 1;
		}
		else {
			maskedText = "";
			for (size_t i = 0; i < textComponent->text.size(); i++) {
				if (i == index) {
					maskedText += " |";
				}
				else if (textComponent->text[i] == '\n') {
					maskedText += "\n ";
				}
				else if (textComponent->text[i] == ' ') {
					maskedText += " ";
				}
				else {
					maskedText += ' ';
				}
			}
			
		}
		Engine::Instance().m_render->painter().RenderText(maskedText, *textComponent->GetFont(), textComponent->fontSize, drawPointerPos, size, { scale, scale }, (int)textComponent->horizontalAligment, (int)textComponent->verticalAligment, textComponent->wrap, pivot, {255,0,0,255});
	}

	textComponent->RenderElement();
	
	if (debug)
		RenderElementDebug();
}

void UIInputField::RenderElementDebug()
{
}
