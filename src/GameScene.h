#pragma once
#include "ModuleScene.h"
#include "Entity.h"
#include <unordered_map>
#include <vector>


class UICanvas;
class FadeCG;
class UIDialogueBoxCG;
class CombatCG;
class DialogueSystem;
class CombatSystem;
class PauseMenuCG;
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


	PlayerCharacter* GetPlayer() const;

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
	FadeCG* fade = nullptr;
	UIDialogueBoxCG* dialogueCanvas = nullptr;
	CombatCG* combatCanvas = nullptr;

	/// Map
	vector<Tilemap*> tilemaps;
	vector<Entity*> entities;
	PlayerCharacter* player = nullptr;
	CameraController* cameraController = nullptr;

	/// Extra
};