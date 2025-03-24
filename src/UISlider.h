#pragma once
#include "UIElement.h"

class UIButton;
class UIImage;
class UISlider : public UIElement {
public:

	enum class SliderMode {
		LeftToRight,
		RightToLeft,
		TopToDown,
		DownToTop
	};

	UISlider(SDL_Texture& _bgTexture, Vector2Int _position, Vector2Int _size, SDL_Rect _bgRect, SDL_Texture& _thumbTexture, Vector2Int _thumbSize, SDL_Rect _defaultThumbRect, SDL_Rect _selectedThumbRect, Vector2 _pivot = { 0,0 }, float _value = 0, float _minValue =0 , float _maxValue=1, SliderMode _mode = SliderMode::LeftToRight);
	~UISlider();
	// Inherited via UIElement
	void UpdateElement() override;
	// Inherited via UIElement
	void RenderElement() override;
	// Inherited via UIElement
	void RenderElementDebug() override;

	void SetLocalScale(float _scale);

	void SetValue(float _value);
	void SetLimitValues(float _minValue, float _maxValue);
	float GetValue();
	float GetMinValue();
	float GetMaxValue();




public:
	SystemEvent<float> onValueChange;
private:
	bool FixThumbBounds(Vector2Int thumPosition);
	void FixValueLimits();

	void UpdateValue();
	void UpdateThumbByValue();
private:
	UIButton* button_component;
	UIImage* image_component;

	float value = 0; 
	float minValue = 0;
	float maxValue = 1.f;

	bool usingSlider = false;

	SliderMode mode = SliderMode::LeftToRight;

protected:
};
