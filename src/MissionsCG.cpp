#include "MissionsCG.h"
#include "MissionManager.h"


#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleInput.h"

#include "UIImage.h"
#include "UITextBox.h"


MissionsCG::MissionsCG(int _renderLayer)
{
	renderLayer = _renderLayer;

	CreateLayout();
}

MissionsCG::~MissionsCG()
{
}

void MissionsCG::UpdateCanvas()
{
	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_UP)) {
		GoToMission(selectedMissionIndex - 1);
	}
	else if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_DOWN)) {
		GoToMission(selectedMissionIndex + 1);
	}
	UICanvas::UpdateCanvas();
}

void MissionsCG::Reset()
{
}

void MissionsCG::GoToMission(int index)
{
	if(index<0)
		index = MissionMana
}

void MissionsCG::ShowMissionLabel(float time)
{
}

void MissionsCG::HideMissionLabel(float time)
{

}

void MissionsCG::CreateLayout()
{
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	SDL_Texture* layout = Engine::Instance().m_assetsDB->GetTexture("missions_layOut");
	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*layout);

	layOut.missionOverlay = new UIImage(*layout, { 20,0 }, textureSize, { 0,0 });
	layOut.missionTitleText = new UITextBox("Title", *textFont, 16, { 184,132,78,255 }, { 6, 27 }, { textureSize.x -12, 39 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionTitleText->SetParent(layOut.missionOverlay);

	layOut.missionDescriptionText = new UITextBox("Description", *textFont, 16, { 184,132,78,255 }, { 6, 27 }, { (textureSize.x - 12)*2, 39*2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionDescriptionText->SetLocalScale(0.5f);
	layOut.missionDescriptionText->SetParent(layOut.missionOverlay);
	layOut.missionDescriptionText->localVisible = false;

	AddElementToCanvas(layOut.missionOverlay);
}

void MissionsCG::UpdateLayout()
{
	
}
