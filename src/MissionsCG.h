#pragma once
#include "UICanvas.h"
#include "Vector2.h"
#include "StepTimer.h"
#include <vector>

using namespace std;

class UIImage;
class UITextBox;

class MissionsCG : public UICanvas {
public:
	MissionsCG(int _renderLayer);
	~MissionsCG();

	void UpdateCanvas() override;


	void Reset();

	void GoToMission(int index);

	void ShowMissionLabel(float time);
	void HideMissionLabel(float time);

private:
	struct UIMissionLayout {
		int missionIndex;
		UIImage* missionOverlay = nullptr;
		UITextBox* missionTitleText = nullptr;
		UITextBox* missionDescriptionText = nullptr;
	};

	void CreateLayout();

	void UpdateLayout();

private:
	int selectedMissionIndex = 0;
	UIMissionLayout layOut;

	
	StepTimer timer;
	int timeToDoMove;
	Vector2 hidePosition;
	Vector2 showPosition;



};

