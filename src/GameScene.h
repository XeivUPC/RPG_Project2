#pragma once
#include "ModuleScene.h"
#include "Entity.h"
#include <unordered_map>
#include <vector>


class UICanvas;
class FadeCG;
class UIDialogueBoxCG;
class GameState;
class Tilemap;

class GameScene : public ModuleScene{
	friend class Engine;
	friend class DialogueGameState;
public:
	GameScene(bool start_active = true);
	~GameScene();

	enum class State {
		Exploring,
		Combat,
		Dialogue,
		Menu,
		GameOver
	};

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

	void SetState(State _newState);
	State GetState();

private:
	/// Core
	State state = State::Dialogue;
	unordered_map<State, GameState*> game_states;

	/// UI
	UICanvas* canvas = nullptr;
	FadeCG* fade = nullptr;
	UIDialogueBoxCG* dialogueCanvas = nullptr;

	/// Map
	vector<Tilemap*> tilemaps;
	vector<Entity*> entities;

	/// Extra
};