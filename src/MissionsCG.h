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

	void GoToMission(int index, bool canShowHide=true);

	void ShowMissionLabel(float time);
	void HideMissionLabel(float time);


private:
	struct UIMissionLayout {
		int missionIndex;
		UIImage* missionOverlay = nullptr;
		UITextBox* missionTitleText = nullptr;
		UITextBox* missionDescriptionText = nullptr;

		UITextBox* missionDataType = nullptr;

		UITextBox* missionNumber = nullptr;
	};

	void CreateLayout();

	void UpdateLayout(bool canShowHide = true);

private:
	int selectedMissionIndex = 0;
	UIMissionLayout layOut;

	
	StepTimer timer;
	float timeToDoMove;
	Vector2 from;
	Vector2 to;
	bool isHidden = true;
	bool isMoving = false;


	StepTimer limitTimer;
	int maxTimeShown = 10;


};

