#pragma once
#include "ModuleScene.h"
#include "IRendereable.h"


class UICanvas;
class FadeCG;

class TitleScene : public ModuleScene, public IRendereable {
	friend class Engine;
public:
	TitleScene(bool start_active = true);
	~TitleScene();

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
	// Inherited via IRendereable
	void Render() override;
	// Inherited via ICleanable
	bool CleanUp() override;

private:
	UICanvas* canvas = nullptr;
	FadeCG* fade = nullptr;
};
