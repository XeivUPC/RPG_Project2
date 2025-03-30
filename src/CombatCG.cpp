#pragma once
#include "CombatCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "TextureAtlas.h"
#include "DialogueSystem.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UISlider.h"

#include "Globals.h"

CombatCG::CombatCG(CombatSystem* _combatSystem)
{
	SDL_Texture* tex = Engine::Instance().m_assetsDB->GetTexture("dialogue_box");
	_TTF_Font* font = Engine::Instance().m_assetsDB->GetFont("alagard");

	Vector2Int textureSize = Engine::Instance().m_assetsDB->GetTextureSize(*tex);
	attackSelectionBackground = new UIImage(*tex, {0,0}, textureSize);
	combatSystem = _combatSystem;

	AddElementToCanvas(attackSelectionBackground);
}
