#pragma once
#include "ModuleScene.h"
#include "Entity.h"
#include "StepTimer.h"
#include <unordered_map>
#include <vector>
#include <random>


class UICanvas;
class FadeCG;
class UIDialogueBoxCG;
class CombatCG;
class DialogueSystem;
class CombatSystem;
class PauseMenuCG;
class GameplayCG;
class ScreenEffectsCG;
class GameState;
class Tilemap;
class PlayerCharacter;
class CameraController;

class GameScene : public ModuleScene{
	friend class Engine;
	friend class CombatGameState;
	friend class DialogueGameState;
	friend class PauseGameState;
	friend class ExploringGameState;
public:

	enum class State {
		Exploring,
		Combat,
		Dialogue,
		Menu,
		GameOver,
		NONE___DO_NOT_USE
	};

	GameScene(bool start_active = true);
	~GameScene();

	void SetDialogue(string path);
	void SetCombat(std::vector<int>team, std::vector<int> enemyTeam);

	void SetState(State _newState);
	State GetState();
	GameState* GetGameState();
	void SetPreviousState();

	void ExitGame();


	void AddTilemap(string path);
	void RemoveLastTilemap();
	Tilemap* GetLastTilemap();

	int GetTime();
	float GetTimeScale();


	PlayerCharacter* GetPlayer() const;

	void FreshStart();
	void LoadGameSaveData();
	void SaveGameSaveData();


public:

private:
	// Inherited via IInitializable
	bool Init() override;
	// Inherited via IInitializable
	bool Start() override;
	// Inherited via IUpdateable
	bool PreUpdate() override;
	// Inherited via IUpdateable
	bool Update() override;
	// Inherited via IUpdateable
	bool PostUpdate() override;
	// Inherited via ICleanable
	bool CleanUp() override;

	void UpdateRain();
	void PauseRain();
	void ResumeRain();
	void StopRain();
	void StartRain();


	void CreateNewTilemap(string path);
	void DeleteLastTilemap();

private:
	/// Core
	State state = State::NONE___DO_NOT_USE;
	State previous_state = State::NONE___DO_NOT_USE;
	unordered_map<State, GameState*> game_states;
	bool exitGame = false;

	DialogueSystem* dialogueSystem = nullptr;
	CombatSystem* combatSystem = nullptr;

	/// UI
	UICanvas* canvas = nullptr;
	PauseMenuCG* pauseCanvas = nullptr;
	GameplayCG* gameplayCanvas = nullptr;
	ScreenEffectsCG* screenEffectsCanvas = nullptr;
	FadeCG* fade = nullptr;
	UIDialogueBoxCG* dialogueCanvas = nullptr;
	CombatCG* combatCanvas = nullptr;

	/// Map
	vector<Tilemap*> tilemaps;
	PlayerCharacter* player = nullptr;
	CameraController* cameraController = nullptr;

	/// Extra
	StepTimer clock = StepTimer(12 * 3600);
	float timeScale = 300;



	/// RainSettings
	bool isInterior = false;
	bool isRaining = false;
	float rainCooldown = 0.0f;
	float rainDuration = 0.0f;

	StepTimer rainTimer = StepTimer();
	std::mt19937 rng;
	const float minRainCooldown = 3600.0f * 10;   
	const float maxRainCooldown = 3600 * 16;  
	const float minRainDuration = 3600 * 3; 
	const float maxRainDuration = 3600 * 6; 


	//// Saving
	string savePath = "Assets/Data/GameSaveData.xml";

};