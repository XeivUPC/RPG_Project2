#include "UITestingCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleAudio.h"
#include "TextureFactory.h"

#include "UIButton.h"
#include "UIToggle.h"
#include "UISlider.h"
#include "UITextBox.h"
#include "UIInputField.h"

UITestingCG::UITestingCG()
{
    _TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("monogram");
    SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
    SDL_Texture* tex2 = Engine::Instance().m_assetsDB->GetTexture("toggle_tex1");
    SDL_Texture* tex3 = Engine::Instance().m_assetsDB->GetTexture("slider_tex1");

    btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");

    UIButton* btn = new UIButton(*tex, { 150,100 }, { 57,23 }, { 0,4,57,23 }, { 0.5f,0.5f });
    btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,101,31 });
    btn->AddRect(UIButton::ButtonStates::PRESSED, { 158,0,101,31 });
    btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
    btn->SetLocalScale(1);
   // btn->debug = true;
    AddElementToCanvas(btn);

    btn = new UIButton(*tex, { 150,170 }, { 57,23 }, { 0,4,57,23 }, { 0.5f,0.5f });
    btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,101,31 });
    btn->AddRect(UIButton::ButtonStates::PRESSED, { 158,0,101,31 });
    btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
    btn->SetLocalScale(1);
   // btn->debug = true;
    AddElementToCanvas(btn);

    btn = new UIButton(*tex, { 150,240 }, { 57,23 }, { 0,4,57,23 }, { 0.5f,0.5f });
    btn->AddRect(UIButton::ButtonStates::HOVER, { 57,0,101,31 });
    btn->AddRect(UIButton::ButtonStates::PRESSED, { 158,0,101,31 });
    btn->onMouseEnter.emplace_back([this]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
    btn->SetLocalScale(1);
   // btn->debug = true;
    AddElementToCanvas(btn);

    UIToggle* toggle = new UIToggle(*tex2, { 300,240 }, { 19,19 }, { 0,0,19,19 }, { 19,0,19,19 }, { 0.5,0.5 }, true);
    //toggle->debug = true;
    AddElementToCanvas(toggle);

    UISlider* slider2 = new UISlider(*tex3, { 300,300 }, { 92,13 }, { 0,0,98,26 }, *tex3, { 8,26 }, { 98,0,8,26 }, { 106,0,8,26 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::RightToLeft);
    slider2->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetGeneralVolume(value); });
    slider2->SetValue(Engine::Instance().m_audio->GetGeneralVolume());
   // slider2->debug = true;
    AddElementToCanvas(slider2);

    UISlider* slider1 = new UISlider(*tex3, { 400,200 }, { 13,92 }, { 0,26,26,98 }, *tex3, { 26,8 }, { 0,124,26,8 }, { 0,132,26,8 }, { 0.5f,0.5f }, 0, 0, 1, UISlider::SliderMode::TopToDown);
    slider1->onValueChange.emplace_back([this](float value) {Engine::Instance().m_audio->SetMusicVolume(value); });
    slider1->SetValue(Engine::Instance().m_audio->GetMusicVolume());
   // slider1->debug = true;
    AddElementToCanvas(slider1);

    UIInputField* textBox1 = new UIInputField("Enter text...", "", *font, 16, { 255,255,255,255 }, { 71,71,71,255 }, { 300,100 }, { 200, 100 }, { 0.5f,0.5f }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Top, false);
   // textBox1->debug = true;
    textBox1->SetLocalScale(2);
    AddElementToCanvas(textBox1);


}
