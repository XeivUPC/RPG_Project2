#include "MissionsCG.h"
#include "MissionManager.h"
#include "MissionHolder.h"


#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleInput.h"
#include "ModuleTime.h"

#include "UIImage.h"
#include "UITextBox.h"


/// Anañir si esta acabada, en proceso, o rewardeada

MissionsCG::MissionsCG(int _renderLayer)
{
	renderLayer = _renderLayer;


	CreateLayout();
	UpdateLayout();

	MissionManager::Instance().onMissionAdded.Subscribe([this](MissionHolder& mission) {selectedMissionIndex = MissionManager::Instance().GetMissionIndex(mission); GoToMission(selectedMissionIndex); limitTimer.Start(); });
	MissionManager::Instance().onMissionCompleted.Subscribe([this](MissionHolder& mission) {UpdateLayout(false);});
	MissionManager::Instance().onMissionRemoved.Subscribe([this](MissionHolder& mission) {GoToMission(selectedMissionIndex); });

}

MissionsCG::~MissionsCG()
{
}

void MissionsCG::UpdateCanvas()
{
	int missionAmount = MissionManager::Instance().GetMissionsAmount();
	if(missionAmount  == 0){
		if (layOut.missionOverlay->localVisible)
			isVisible = false;
		return;
	}else if(!layOut.missionOverlay->localVisible)
		isVisible = true;

	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		GoToMission(selectedMissionIndex - 1);
		limitTimer.Start();
	}
	else if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		GoToMission(selectedMissionIndex + 1);
		limitTimer.Start();
	}


	if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || Engine::Instance().m_input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		if (!isHidden) {
			layOut.missionDescriptionText->localVisible = !layOut.missionDescriptionText->localVisible;
			layOut.missionTitleText->localVisible = !layOut.missionTitleText->localVisible;
			UpdateLayout(false);
			limitTimer.Start();
		}
		else {
			if (Engine::Instance().m_input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
				GoToMission(selectedMissionIndex - 1, true, false);
			else
				GoToMission(selectedMissionIndex + 1, true, false);
			UpdateLayout(false);

		}

		
	}

	

	if (isMoving) {
		timer.Step(ModuleTime::deltaTime);
		layOut.missionOverlay->SetLocalPosition(Vector2::Lerp(from, to, timer.ReadSec() / timeToDoMove));
		if (timer.ReadSec() > timeToDoMove) {
			isMoving = false;
			layOut.missionOverlay->SetLocalPosition(to);
		}
	}
	else if (!isHidden) {
		limitTimer.Step(ModuleTime::deltaTime);
		if (limitTimer.ReadSec() > maxTimeShown) {
			HideMissionLabel(0.5f);
			limitTimer.Start();
		}
	}


	UICanvas::UpdateCanvas();
}

void MissionsCG::Reset()
{
	selectedMissionIndex = 0;
	UpdateLayout();
}

void MissionsCG::GoToMission(int index, bool _forceChange, bool _canShowHide)
{

	bool forceChange = _forceChange;
	if (index == selectedMissionIndex) {
		forceChange = true;
	}

	int maxAmount = MissionManager::Instance().GetMissionsAmount();
	if (index < 0)
		index = maxAmount - 1;
	else if (index >= maxAmount)
		index = 0;
	else
		forceChange = _forceChange;

	if(!isHidden || forceChange)
		selectedMissionIndex = index;

	UpdateLayout(_canShowHide);
}

void MissionsCG::ShowMissionLabel(float time)
{
	if (!isHidden)
		return;
	isHidden = false;
	isMoving = true;
	timeToDoMove = time;
	from = { 20,-72 };
	to = { 20,0 };
	timer.Start();
}

void MissionsCG::HideMissionLabel(float time)
{
	if (isHidden)
		return;
	isHidden = true;
	isMoving = true;
	timeToDoMove = time;
	from = { 20,0 };
	to = { 20,-72 };
	timer.Start();
}

void MissionsCG::CreateLayout()
{
	_TTF_Font* textFont = Engine::Instance().m_assetsDB->GetFont("alagard");

	SDL_Texture* layout = Engine::Instance().m_assetsDB->GetTexture("missions_layOut");
	SDL_Texture* statustexture = Engine::Instance().m_assetsDB->GetTexture("toggle_tex3");
	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*layout);

	layOut.missionOverlay = new UIImage(*layout, { 20,-72 }, textureSize, { 0,0 });
	layOut.missionTitleText = new UITextBox("Title", *textFont, 16, { 184,132,78,255 }, { 4, 27 }, { 144, 39 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionTitleText->SetParent(layOut.missionOverlay);

	layOut.missionDescriptionText = new UITextBox("Description", *textFont, 16, { 184,132,78,255 }, { 4, 27 }, { (144)*2, 39*2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionDescriptionText->SetLocalScale(0.5f);
	layOut.missionDescriptionText->SetParent(layOut.missionOverlay);
	layOut.missionDescriptionText->localVisible = false;

	layOut.missionDataType = new UITextBox("Title", *textFont, 16, { 184,132,78,255 }, { 26,3}, { 100, 16 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionDataType->SetParent(layOut.missionOverlay);

	layOut.missionNumber = new UITextBox("Mission", *textFont, 16, { 184,132,78,255 }, { 51,77 }, { 54*2, 7*2 }, { 0,0 }, UITextBox::HorizontalAlignment::Middle, UITextBox::VerticalAlignment::Middle);
	layOut.missionNumber->SetLocalScale(0.5f);
	layOut.missionNumber->SetParent(layOut.missionOverlay);

	layOut.missionStatus = new UIImage(*statustexture, { 110,74 }, { 11,11 }, { 0,0 }, true, {33,0,11,11});
	layOut.missionStatus->SetParent(layOut.missionOverlay);

	AddElementToCanvas(layOut.missionOverlay);
}

void MissionsCG::UpdateLayout(bool canShowHide)
{
	
	MissionHolder* mission = MissionManager::Instance().GetMissionByIndex(selectedMissionIndex);

	if (mission == nullptr) {
		if(canShowHide)
			HideMissionLabel(0.5f);
		layOut.missionOverlay->localVisible = false;
		isVisible = false;
		isHidden = true;
		layOut.missionTitleText->SetText("");
		layOut.missionDescriptionText->SetText("");
		layOut.missionDataType->SetText("");
		layOut.missionNumber->SetText("");
	}
	else {
		if(canShowHide)
			ShowMissionLabel(0.5f);
		layOut.missionOverlay->localVisible = true;

		layOut.missionTitleText->SetText(mission->GetTitle());
		layOut.missionDescriptionText->SetText(mission->GetDescription());

		layOut.missionNumber->SetText("Mission " + to_string(selectedMissionIndex));
		
		if (layOut.missionTitleText->localVisible) {
			layOut.missionDataType->SetText("Title");
		}else
			layOut.missionDataType->SetText("Description");


		layOut.missionStatus->SetRect({ (int)mission->GetState() * 11,0,11,11 });
		
	}
	
}
