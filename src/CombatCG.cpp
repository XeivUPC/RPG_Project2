
#include "CombatCG.h"

#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "ModuleTime.h"
#include "ModuleInput.h"
#include "ModuleDebug.h"
#include "ModuleAudio.h"
#include "TextureAtlas.h"
#include "DrawingTools.h"

#include "UIImage.h"
#include "UIAnimatedImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UISlider.h"
#include "UIToggle.h"

#include "Animator.h"
#include "AnimationClip.h"

#include "AlertDisplayerCG.h"

#include "Globals.h"
#include "AttackList.h"
#include "CharacterDatabase.h"

#include <cmath>

CombatCG::CombatCG(CombatSystem* _combatSystem)
{
	combat = _combatSystem;
	combat->onCombatStateChanged.Subscribe([this]() {OnCombatStateChanged(); });
	

	combat_magic = Engine::Instance().m_assetsDB->GetAudio("combat_magic");
	combat_dead = Engine::Instance().m_assetsDB->GetAudio("combat_dead");
	combat_hurt = Engine::Instance().m_assetsDB->GetAudio("combat_hurt");
	combat_physic = Engine::Instance().m_assetsDB->GetAudio("combat_physic");
}

CombatCG::~CombatCG()
{
	//UICanvas::~UICanvas();
	if (alert != nullptr) {
		delete alert;
		alert = nullptr;
	}
}

void CombatCG::UpdateCanvas()
{
	UICanvas::UpdateCanvas();

	for (size_t i = 0; i < charactersSlot.size(); i++)
	{
		CombatSystem::CharacterStats& charStats = charactersSlot[i].characterRef->stats;

		string pjName = CharacterDatabase::Instance().GetCharacterDefinition(charactersSlot[i].characterRef->id).name;
		float maxHealth = (float)charStats.GetHpStatValue();
		float currentHealth = (float)charStats.currentHp;
		float healthRatio = currentHealth / maxHealth;
		if(healthRatio>1)
			healthRatio = 1.f;

		charactersSlot[i].hpBar->size.x = (int)(charactersSlot[i].hpBarMaxWidth * healthRatio);

		bool statusValue = charStats.HasStatusEffect("Poison");
		if(statusValue != charactersSlot[i].poison->IsOn())
			charactersSlot[i].poison->SetValue(statusValue, false);

		statusValue = charStats.HasStatusEffect("Regeneration");
		if (statusValue != charactersSlot[i].poison->IsOn())
			charactersSlot[i].regeneration->SetValue(statusValue, false);

		statusValue = charStats.HasStatusEffect("Burn");
		if (statusValue != charactersSlot[i].poison->IsOn())
			charactersSlot[i].burn->SetValue(statusValue, false);
	}
	if (debug_immortalEnabled != nullptr) {
		if (Engine::Instance().m_debug->godmode)
			debug_immortalEnabled->localVisible = true;
		else
			debug_immortalEnabled->localVisible = false;
	}
	if (combat->GetCombatState() == CombatSystem::PREPARE_ATTACKS)
	{
		visualEffects.first = false;
		visualEffects.second = false;

		animationEffect.first = false;

		targetVisualsCompleted = pair<int, int>(0, 0);
		targetVisualsRequest = pair<int, int>(0, 0);

		firstTick = true;
	}
	if (combat->GetCombatState() == CombatSystem::ATTACKS)
	{
		if (!visualEffects.first && !visualEffects.second)
		{
			if (firstTick)
			{
				targetVisualsRequest.second = 1;
				UICharacterSlot* slotSelected = GetSlotByCharacter(combat->GetCurrentAttackAttacker());
				CombatSystem::TurnAttack* turnData = combat->GetCurrentTurnAttack();
				slotSelected->combatEffect->GetJsonAnimator()->CleanUp();
				slotSelected->combatEffect->GetJsonAnimator()->AddJsonAnimationClip(turnData->attack->animation_hit_data, 0.1f, {0.5f,0.5f});
				slotSelected->combatEffect->GetJsonAnimator()->GetCurrentAnimationClip()->Loop(false);
				slotSelected->combatEffect->GetJsonAnimator()->GetCurrentAnimationClip()->onAnimationFinished.Subscribe([this, slotSelected]() {FinishEffectVisuals(slotSelected->combatEffect); });

				if (turnData->attack->type == Attack::AttackType::Aggressive) {
					slotSelected->characterImage->GetAnimator()->Animate("physic-attack");
					Engine::Instance().m_audio->PlaySFX(combat_physic);
				}
				else {
					slotSelected->characterImage->GetAnimator()->Animate("special-attack");
					Engine::Instance().m_audio->PlaySFX(combat_magic);
				}
				//Animate effects
				if (targetVisualsRequest.second == targetVisualsCompleted.second)
					animationEffect.second = true;
				firstTick = false;
			}


			if (animationEffect == pair<bool, bool>(true, true))
			{
				animationEffect = pair<bool, bool>(false, true);
				visualEffects.first = true;
				firstTick = true;
			}
		}
		if (visualEffects.first && !visualEffects.second)
		{
			if (firstTick)
			{
				targetVisualsRequest.first = combat->CurrentAttackTargetAmount();
				targetVisualsRequest.second = combat->CurrentAttackTargetAmount();
				CombatSystem::TurnAttack* turnData = combat->GetCurrentTurnAttack();

				for (size_t i = 0; i < combat->CurrentAttackTargetAmount(); i++)
				{
					UICharacterSlot* slot = GetSlotByCharacter(combat->GetCurrentAttackTargetList()[i]);
					if (slot->characterRef->stats.currentHp > 0) {
						slot->combatEffect->GetJsonAnimator()->CleanUp();
						slot->combatEffect->GetJsonAnimator()->AddJsonAnimationClip(turnData->attack->animation_hurt_data, 0.1f, { 0.5f,0.5f });
						slot->combatEffect->GetJsonAnimator()->GetCurrentAnimationClip()->Loop(false);
						slot->combatEffect->GetJsonAnimator()->GetCurrentAnimationClip()->onAnimationFinished.Subscribe([this, slot]() {FinishEffectVisuals(slot->combatEffect); });

						slot->characterImage->GetAnimator()->Animate("hurt");
						Engine::Instance().m_audio->PlaySFX(combat_hurt);
					}
					else
						targetVisualsCompleted.first++;

					if (targetVisualsRequest.first == targetVisualsCompleted.first)
						animationEffect.first = true;
					if (targetVisualsRequest.second == targetVisualsCompleted.second)
						animationEffect.second = true;
				}
				//Animate effects
				firstTick = false;
				combat->NextAttack();
			}
			if (animationEffect == pair<bool, bool>(true, true))
			{
				animationEffect = pair<bool, bool>(false, false);
				visualEffects.second = true;
				targetVisualsCompleted = pair<int, int>(0, 0);
				targetVisualsRequest = pair<int, int>(0, 0);
				firstTick = true;
			}
		}
		if (visualEffects.first && visualEffects.second)
		{
			visualEffects = pair<bool, bool>(false, false);
			
		}
	}

	if(alert!=nullptr)
		alert->UpdateCanvas();
}

void CombatCG::LoadCanvas()
{

	attackButtons.clear();
	charactersSlot.clear();
	attacksToExecute.clear();

	alert = new AlertDisplayerCG(1.5f, nullptr, { LOGIC_SCREEN_WIDTH / 2 - 150,0 }, { 300,32 }, {0.f,0.f});
	alert->renderLayer = 7;
	alert->onAlertOpened.Subscribe([this]() {Engine::Instance().m_audio->PlaySFX(Engine::Instance().m_assetsDB->GetAudio("alert")); });

	//// SetBackground
	SDL_Texture* bg_texture = Engine::Instance().m_assetsDB->GetTexture("battle_bg");
	combatBg = new UIImage(*bg_texture, {0,0}, {LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT});
	//combatBg = new UIImage({ 0,0 }, { LOGIC_SCREEN_WIDTH,LOGIC_SCREEN_HEIGHT }, { 0,0 }, false, { 0,0,0,0 }, { 200,200,200,255 });
	AddElementToCanvas(combatBg);

	////CreateCombatLayout
	SDL_Texture* layout_texture = Engine::Instance().m_assetsDB->GetTexture("uiBox3");
	combatLayout = new UIImage(*layout_texture, { 0, LOGIC_SCREEN_HEIGHT - 89 }, {640,89});
	combatLayout->SetParent(combatBg);

	//// Create AttackButtons
	for (int i = 0; i < 4; i++)
	{
		Vector2Int btn_position = { 8 + 86 * (i / 2) + 1 * (i / 2), 9 + 35 * (i % 2) + 1 * (i % 2) };
		Attack* attackValue = nullptr;
		attackButtons.emplace_back(CreateUIAttackButton(i, btn_position));

		attackButtons[i].btn->SetParent(combatLayout);
	}

	/// CreateAttackInformation
	CreateUIAttackInformation();
	HideAttackInformation(-1);
	attackInfo.name->SetParent(combatLayout);

	////CreateCharactersUI

	for (int i = 0; i < charactersSlot.size(); i++)
	{
		RemoveElementFromCanvas(charactersSlot[i].characterClick);
		delete charactersSlot[i].characterClick;
	}
	charactersSlot.clear();

	for (const auto& characterTeam : combat->GetCharactersInCombat()) {
		int index = 0;
		for (auto& character : characterTeam.second)
		{
			Vector2Int position= GetSlotPosition(character.team, index, (int)characterTeam.second.size());
			UICharacterSlot& reference = charactersSlot.emplace_back(CreateUICharacterSlot((CombatSystem::CharacterReference*)&character, position));
			reference.characterClick->onMouseClick.Subscribe([this, reference]() {SelectCharacter((UICharacterSlot&)reference); });
			index++;
		}
	}


	//// Others
	CreateUIExtras();
}

void CombatCG::UnloadCanvas()
{
	charactersSlot.clear();
	if (alert != nullptr) {
		delete alert;
		alert = nullptr;
	}
	ClearCanvas();
}

CombatCG::UIAttackButton CombatCG::CreateUIAttackButton(int attackIndex, Vector2 btn_position)
{
	SDL_Texture* btn_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex1");
	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");

	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	Vector2Int btn_size = { 86,35 };

	int font_size = 16;
	SDL_Color font_color = { 184,132,78,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Middle;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Middle;

	UIButton* button = new UIButton(*btn_texture, btn_position, btn_size, {0,0,btn_size.x,btn_size.y});
	button->AddRect(UIButton::ButtonStates::HOVER, { btn_size.x,0,btn_size.x,btn_size.y });
	UITextBox* attackName = new UITextBox("", *btn_font, font_size, font_color, btn_size / 2 + Vector2Int{0,3}, btn_size, { 0.5f,0.5f }, hAligment, vAligment);
	
	button->onMouseEnter.Subscribe([this, attackIndex]() {ShowAttackInformation(attackIndex);});
	button->onMouseExit.Subscribe([this, attackIndex]() {HideAttackInformation(attackIndex);});
	button->onMouseClick.Subscribe([this, attackIndex]() {SelectAttack(attackIndex); });
	
	button->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	button->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });

	attackName->SetParent(button);
	button->isEnabled = false;
	UIAttackButton attackButtonGroup = { button, attackName, nullptr };

	return move(attackButtonGroup);
}

void CombatCG::CreateUIAttackInformation()
{
	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");
	SDL_Texture* icons_16_texture = Engine::Instance().m_assetsDB->GetTexture("icons_16");

	int font_size = 16;
	SDL_Color font_color = { 184,132,78,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Left;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Top;

	UITextBox* attackName = new UITextBox("Name", *btn_font, font_size, font_color, {199,10}, { 143,16 }, { 0,0 }, hAligment, vAligment);
	UITextBox* attackDescription = new UITextBox("Description", *btn_font, font_size, font_color, {0,24}, { 431,46 }, { 0,0 }, hAligment, vAligment);
	
	//hAligment = UITextBox::HorizontalAlignment::Right;
	UIImage* attackPowerImg = new UIImage(*icons_16_texture, { 240,0 }, { 16,16 }, { 0,0 }, true, { 0,0,16,16 });
	UITextBox* attackPower = new UITextBox("Pow: 30", *btn_font, font_size, font_color, {24,0}, { 72,16 }, { 0,0 }, hAligment, vAligment,false);
	attackPower->SetParent(attackPowerImg);

	UIImage* attackAccuracyImg = new UIImage(*icons_16_texture, { 341,0 }, { 16,16 }, { 0,0 }, true, { 16,0,16,16 });
	UITextBox* attackAccuracy = new UITextBox("Acc: 100%", *btn_font, font_size, font_color, { 24,0 }, { 72,16 }, {0,0}, hAligment, vAligment,false);
	attackAccuracy->SetParent(attackAccuracyImg);

	attackDescription->SetParent(attackName);
	attackPowerImg->SetParent(attackName);
	attackAccuracyImg->SetParent(attackName);


	attackInfo = { attackName,attackDescription,attackPowerImg,attackPower,attackAccuracyImg,attackAccuracy };
}

CombatCG::UICharacterSlot CombatCG::CreateUICharacterSlot(CombatSystem::CharacterReference* value, Vector2 slot_position)
{
	SDL_Texture* overlay_texture = Engine::Instance().m_assetsDB->GetTexture("combat_characterUI");
	SDL_Texture* effects_texture = Engine::Instance().m_assetsDB->GetTexture("effects_toggle");
	SDL_Texture* charSelect_texture = Engine::Instance().m_assetsDB->GetTexture("arrow_tex1");
	SDL_Texture* charAttackDone_texture = Engine::Instance().m_assetsDB->GetTexture("tick_tex1");
	SDL_Texture* icons_16_texture = Engine::Instance().m_assetsDB->GetTexture("icons_16");

	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");

	CharacterDatabase::CharacterDefinition& charactedData = CharacterDatabase::Instance().GetCharacterDefinition(value->id);

	int font_size = 16;
	SDL_Color font_color = { 184,132,78,255 };
	UITextBox::HorizontalAlignment hAligment = UITextBox::HorizontalAlignment::Middle;
	UITextBox::VerticalAlignment vAligment = UITextBox::VerticalAlignment::Middle;

	UIImage* overlay = new UIImage(*overlay_texture, slot_position, { 69,11 }, { 0,0 }, true, { 0,0,69,11 });

	UITextBox* slotLvl = new UITextBox(to_string(charactedData.level), *btn_font, font_size, font_color, { 2,2 }, { 18,18 }, { 0,0 }, hAligment, vAligment);
	slotLvl->SetLocalScale(0.5f);

	hAligment = UITextBox::HorizontalAlignment::Left;
	vAligment = UITextBox::VerticalAlignment::Top;
	UITextBox* slotName = new UITextBox(charactedData.name, *btn_font, font_size, {255,255,255,255}, { 0,0 }, { 69,18 }, { 0,1 }, hAligment, vAligment, false);
	slotName->SetLocalScale(0.5f);
	
	UIToggle* poisonToggle = new UIToggle(*effects_texture, { 54,2 }, { 8,8 }, { 0,0,8,8 }, { 8,0,8,8 }, {0,0},false);
	poisonToggle->interactable = false;
	UIToggle* burnToggle = new UIToggle(*effects_texture, {64,2}, { 8,8 },{ 0,8,8,8 }, { 8,0,8,8 }, { 0,0 }, false);
	burnToggle->interactable = false;
	UIToggle* regenerationToggle = new UIToggle(*effects_texture, {74,2}, {8,8}, { 0,16,8,8 }, { 8,0,8,8 }, { 0,0 }, false);
	regenerationToggle->interactable = false;


	UIImage* hpBar = new UIImage({ 11,4 }, { 41,3 }, { 0,0 }, false, { 0,0,0,0 }, {255,0,0,255});
	int hpBarMaxWidth = (int)hpBar->size.x;

	UIImage* attackDone = new UIImage(*charAttackDone_texture, { 60,1 }, { 19,19 }, { 0,1 });
	attackDone->localVisible = false;

	UIImage* selectedCharacterIndicator = new UIImage(*charSelect_texture, { -1,5 }, {19,19}, { 1,0.5f });
	selectedCharacterIndicator->localVisible = false;
	
	UIImage* selectedCharacterTarget = new UIImage(*icons_16_texture, { -8,-80 }, { 16,16 }, { 0,0 }, true, { 0,16,16,16 });
	selectedCharacterTarget->localVisible = false;

	UIButton* characterBtn = new UIButton({35,-7}, { 32, 62 }, {0,0,0,0}, { 0.5f,1 }, { 255,255,255,0 });
	characterBtn->onMouseEnter.Subscribe([selectedCharacterTarget]() {selectedCharacterTarget->SetRect({ 0,32,16,16 }); });
	characterBtn->onMouseExit.Subscribe([selectedCharacterTarget]() {selectedCharacterTarget->SetRect({ 0,16,16,16 }); });


	SDL_Texture* characterTexture = Engine::Instance().m_assetsDB->GetTexture(charactedData.charTemplate->textureId);

	UIAnimatedImage* characterImage = new UIAnimatedImage({ 0,-30 }, { 64,64 }, { 0.5f,0.5f });
	characterImage->SetLocalScale(1.7f);
	characterImage->flip = value->team == CombatSystem::Enemy;


	int spriteSize = 64;
	characterImage->GetAnimator()->AddAnimationClip(AnimationClip("combat-idle", true, false, 0.1f,
		{
			Sprite(characterTexture, {0 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {1 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {2 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {3 * spriteSize,1 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
		}, nullptr, nullptr));

	characterImage->GetAnimator()->AddAnimationClip(AnimationClip("physic-attack", true, false, 0.1f,
		{
			Sprite(characterTexture, {0 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {1 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {2 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {3 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {4 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {5 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {6 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {7 * spriteSize,11 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
		}, nullptr, nullptr));

	characterImage->GetAnimator()->AddAnimationClip(AnimationClip("special-attack", true, false, 0.1f,
		{
			Sprite(characterTexture, {0 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {1 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {2 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {3 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {4 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {5 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {6 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {7 * spriteSize,9 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
		}, nullptr, nullptr));

	characterImage->GetAnimator()->AddAnimationClip(AnimationClip("hurt", true, false, 0.1f,
		{
			Sprite(characterTexture, {0 * spriteSize, 12 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {1 * spriteSize, 12 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {2 * spriteSize, 12 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {3 * spriteSize, 12 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
		}, nullptr, nullptr));

	characterImage->GetAnimator()->AddAnimationClip(AnimationClip("die", false, false, 0.1f,
		{
			Sprite(characterTexture, {0 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {1 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {2 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {3 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {4 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {5 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {6 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f }),
			Sprite(characterTexture, {7 * spriteSize,10 * spriteSize,spriteSize,spriteSize},{0.5f,0.5f })
		}, nullptr, nullptr));

	characterImage->GetAnimator()->GetAnimationClip("special-attack")->onAnimationFinished.Subscribe([this, characterImage]() {FinishAttackVisuals(characterImage); });
 	characterImage->GetAnimator()->GetAnimationClip("physic-attack")->onAnimationFinished.Subscribe([this, characterImage]() {FinishAttackVisuals(characterImage); });
 	characterImage->GetAnimator()->GetAnimationClip("hurt")->onAnimationFinished.Subscribe([this, characterImage, value]() {FinishHurtVisuals(characterImage, value); });
 	
	UIAnimatedImage* combatEffect = new UIAnimatedImage({ 0,-30 }, { 64,64 }, { 0.5f,0.5f }, {255,255,255,0});
	combatEffect->SetLocalScale(1.7f);
	combatEffect->flip = value->team == CombatSystem::Enemy;

	selectedCharacterTarget->SetParent(characterBtn);
	characterImage->SetParent(characterBtn);
	combatEffect->SetParent(characterBtn);
	characterBtn->SetParent(overlay);
	slotLvl->SetParent(overlay);
	slotName->SetParent(overlay);
	hpBar->SetParent(overlay);

	poisonToggle->SetParent(overlay);
	burnToggle->SetParent(overlay);
	regenerationToggle->SetParent(overlay);

	attackDone->SetParent(overlay);

	selectedCharacterIndicator->SetParent(overlay);


	AddElementToCanvas(overlay);

	return { characterBtn,characterImage, value,slotLvl,slotName, poisonToggle, burnToggle,regenerationToggle, hpBar,hpBarMaxWidth, overlay, attackDone,selectedCharacterIndicator, selectedCharacterTarget, combatEffect};
}

void CombatCG::CreateUIExtras()
{
	TTF_Font* btn_font = Engine::Instance().m_assetsDB->GetFont("alagard");
	SDL_Texture* controlBtns_texture = Engine::Instance().m_assetsDB->GetTexture("btn_tex3");


	Mix_Chunk* btn_enter = Engine::Instance().m_assetsDB->GetAudio("btn_enter");
	Mix_Chunk* btn_click = Engine::Instance().m_assetsDB->GetAudio("btn_click");

	Vector2Int btn_size = { 108, 19 };
	int font_size = 16;
	SDL_Color font_color = { 184,132,78,255 };

	cancelAttack = new UIButton(*controlBtns_texture, { 80,0 }, btn_size, { 0,0,btn_size.x,btn_size.y }, { 0,1 });
	UITextBox* cancelAttackText = new UITextBox("Cancel", *btn_font, font_size, font_color, { 6,2 }, { 78,19 }, { 0,1 }, UITextBox::HorizontalAlignment::Right, UITextBox::VerticalAlignment::Middle, false);
	cancelAttackText->SetParent(cancelAttack);
	cancelAttack->AddRect(UIButton::ButtonStates::HOVER, { btn_size.x,0,btn_size.x,btn_size.y });
	cancelAttack->onMouseClick.Subscribe([this]() {SetTargetSelectionMode(false); });
	cancelAttack->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	cancelAttack->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });


	confirmAttack = new UIButton(*controlBtns_texture, { 190,0 }, btn_size, { 0,btn_size.y,btn_size.x,btn_size.y }, { 0,1 });
	UITextBox* confirmAttackText = new UITextBox("Confirm", *btn_font, font_size, font_color, { 23,2 }, { 78,19 }, { 0,1 }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle, false);
	confirmAttackText->SetParent(confirmAttack);
	confirmAttack->AddRect(UIButton::ButtonStates::HOVER, { btn_size.x,btn_size.y * 1,btn_size.x,btn_size.y });
	confirmAttack->onMouseClick.Subscribe([this]() {ConfirmAttack(); });
	confirmAttack->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	confirmAttack->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });


	passTurn = new UIButton(*controlBtns_texture, { 532,0 }, btn_size, { 0,btn_size.y * 2,btn_size.x,btn_size.y }, { 0,1 });
	UITextBox* passTurnText = new UITextBox("Next Turn", *btn_font, font_size, font_color, { 6,2 }, { 78,19 }, { 0,1 }, UITextBox::HorizontalAlignment::Right, UITextBox::VerticalAlignment::Middle, false);
	passTurnText->SetParent(passTurn);
	passTurn->AddRect(UIButton::ButtonStates::HOVER, { btn_size.x,btn_size.y * 2,btn_size.x,btn_size.y });
	passTurn->onMouseClick.Subscribe([this]() {EndTurn(); });
	passTurn->onMouseClick.Subscribe([this, btn_click]() {Engine::Instance().m_audio->PlaySFX(btn_click); });
	passTurn->onMouseEnter.Subscribe([this, btn_enter]() {Engine::Instance().m_audio->PlaySFX(btn_enter); });
	
	debug_immortalEnabled = new UITextBox("GOD mode enabled", *btn_font, 16, font_color, { 10, 10 }, { LOGIC_SCREEN_WIDTH, 30 }, { 0,00 }, UITextBox::HorizontalAlignment::Left, UITextBox::VerticalAlignment::Middle, false);
	debug_immortalEnabled->localVisible = false;
	debug_immortalEnabled->SetParent(combatBg);

	confirmAttack->localVisible = false;
	confirmAttack->interactable = false;
	cancelAttack->localVisible = false;
	cancelAttack->interactable = false;


	cancelAttack->SetParent(combatLayout);
	confirmAttack->SetParent(combatLayout);
	passTurn->SetParent(combatLayout);
}

void CombatCG::CreateEndScreen()
{

}

void CombatCG::SetEndScreen()
{
	
}

void CombatCG::ShowAttackInformation(int attackIndex)
{
	Attack* attackSelected = attackButtons[attackIndex].attack;
	attackInfo.name->localVisible = true;

	attackInfo.currentAttack = attackSelected;

	attackInfo.name->SetText(attackSelected->name);
	attackInfo.description->SetText(attackSelected->description);
	attackInfo.power->SetText("Pow: "+to_string(attackSelected->power));
	attackInfo.accuracy->SetText("Acc: " + to_string(attackSelected->accuracy) + "%");
}

void CombatCG::HideAttackInformation(int attackIndex)
{
	Attack* attackDeselected = nullptr;
	if (attackIndex < 0 || attackIndex >= attackButtons.size()) {

	}else
		attackDeselected = attackButtons[attackIndex].attack;

	if (attackInfo.currentAttack != attackDeselected)
		return;
	HideAttackInformation();
	
}

void CombatCG::HideAttackInformation()
{
	attackInfo.name->localVisible = false;
	attackInfo.currentAttack = nullptr;
}

void CombatCG::SelectCharacter(UICharacterSlot& character)
{

	if (combat->GetCombatState() != CombatSystem::CombatState::PLAYER_TURN)
		return;

	if (character.characterRef->stats.currentHp <= 0.f)
		return;
	if(selectingTargets)
	{
		SelectTarget(character);
		return;
	}

	if (character.characterRef->team != CombatSystem::Ally)
		return;

	if (selectedCharacter != nullptr)
		selectedCharacter->selectedCharacterIndicator->localVisible = false;


	selectedCharacter = &character;
	selectedCharacter->selectedCharacterIndicator->localVisible = true;
	CharacterDatabase::CharacterDefinition& charactedData = CharacterDatabase::Instance().GetCharacterDefinition(selectedCharacter->characterRef->id);
	for (size_t i = 0; i < attackButtons.size(); i++)
	{
		if (i< charactedData.attacks.size()) {
			Attack* attack = AttackList::Instance().GetAttack(charactedData.attacks[i]);
			attackButtons[i].attack = attack;
			attackButtons[i].text->SetText(attack->name);
			attackButtons[i].btn->isEnabled = true;
		}
		else {
			attackButtons[i].text->SetText("");
			attackButtons[i].btn->isEnabled = false;
		}
	}
}

void CombatCG::DeselectChatacter()
{
	if (selectedCharacter == nullptr)
		return;
	selectedCharacter->selectedCharacterIndicator->localVisible = false;
	selectedCharacter = nullptr;
	for (size_t i = 0; i < attackButtons.size(); i++)
	{
		attackButtons[i].text->SetText("");
		attackButtons[i].btn->isEnabled = false;
	}
}

void CombatCG::ShowAllPossibleTargets()
{
	if (selectedAttack->attack->targetType == CombatSystem::Self) {
		selectedCharacter->selectedCharacterTarget->SetColor({ 255,255,255,255 });
		selectedCharacter->selectedCharacterTarget->localVisible = true;
	}
	for (size_t i = 0; i < charactersSlot.size(); i++)
	{
		if (charactersSlot[i].characterRef->stats.currentHp <= 0.f)
			continue;
		bool isSameType = (charactersSlot[i].characterRef->team == selectedAttack->attack->targetType || selectedAttack->attack->targetType == CombatSystem::Both);

		if (isSameType) {
			charactersSlot[i].selectedCharacterTarget->SetColor({ 255,255,255,255 });
			charactersSlot[i].selectedCharacterTarget->localVisible = true;
		}
	}
}

void CombatCG::HideAllPossibleTargets()
{
	for (size_t i = 0; i < charactersSlot.size(); i++)
	{
		charactersSlot[i].selectedCharacterTarget->SetColor({ 255,255,255,255 });
		charactersSlot[i].selectedCharacterTarget->localVisible = false;
	}
}

vector<CombatCG::UICharacterSlot*> CombatCG::GetPossibleTargets()
{
	vector<UICharacterSlot*> possibleTargets;
	vector<CombatSystem::CharacterReference*> referenceTargets = combat->GetPosibleTargets(selectedCharacter->characterRef,selectedAttack->attack);
	
	for (size_t i = 0; i < referenceTargets.size(); i++)
	{
		for (size_t j = 0; j < charactersSlot.size(); j++)
		{
			if (charactersSlot[j].characterRef == referenceTargets[i]) {
				possibleTargets.emplace_back(&charactersSlot[j]);
				break;
			}
		}
	}
	return possibleTargets;
}

int CombatCG::GetPossibleTargetsAmount()
{
	if (selectedAttack->attack->targetType == CombatSystem::Self) {
		return 1;
	}

	int amount = 0;
	for (size_t i = 0; i < charactersSlot.size(); i++)
	{
		if (charactersSlot[i].characterRef->stats.currentHp <= 0.f)
			continue;
		if (charactersSlot[i].characterRef->team == selectedAttack->attack->targetType || selectedAttack->attack->targetType == CombatSystem::Both) {
			amount++;
		}
	}
	return amount;
}

void CombatCG::SelectTarget(UICharacterSlot& character)
{
	/// Check AutoSelect by numbers of enemies and requiered targets

	if (combat->GetCombatState() != CombatSystem::CombatState::PLAYER_TURN)
		return;

	bool isSameType = (character.characterRef->team == selectedAttack->attack->targetType || selectedAttack->attack->targetType == CombatSystem::Both);
	bool isSelfAttack = (selectedAttack->attack->targetType == CombatSystem::Self && &character == selectedCharacter);

	if (isSameType || isSelfAttack) {
		auto it = std::find(targetCharacters.begin(), targetCharacters.end(), &character);
		if (it != targetCharacters.end()) { /// Remove
			targetCharacters.erase(it);
			character.selectedCharacterTarget->SetColor({255,255,255,255});
		}
		else { /// Addd
			if (selectedAttack->attack->targetAmount <= targetCharacters.size())
				return;

			targetCharacters.emplace_back(&character);
			character.selectedCharacterTarget->SetColor({ 255,255,0,255 });
		}
	}
}

void CombatCG::RemoveAllTargets()
{
	HideAllPossibleTargets();

	for (size_t i = 0; i < targetCharacters.size(); i++)
	{
		targetCharacters[i]->selectedCharacterTarget->SetColor({ 255,255,255,255 });
	}
	targetCharacters.clear();
}

void CombatCG::SelectAttack(int attackIndex)
{
	if (selectingTargets)
		return;

	RemoveAllTargets();
	selectedAttack = &attackButtons[attackIndex];
	SetTargetSelectionMode(true);
	ShowAllPossibleTargets();

	vector<UICharacterSlot*> possibleTargets = GetPossibleTargets();
	if (selectedAttack->attack->targetAmount >= possibleTargets.size()) {
		for (size_t i = 0; i < possibleTargets.size(); i++)
		{
			possibleTargets[i]->characterClick->onMouseClick.Trigger();
		}
	}

}

void CombatCG::ConfirmAttack()
{
	if (selectedAttack->attack->targetAmount < GetPossibleTargetsAmount()) {
		if (selectedAttack->attack->targetAmount != targetCharacters.size()) {
			//// Change default message
			alert->SetAlertData("Not enough targets selected");
			alert->OpenAlert();
			return;
		}
	}
	else if(GetPossibleTargetsAmount() != targetCharacters.size()){
		alert->SetAlertData("Not enough targets selected");
		alert->OpenAlert();
		return;
	}

	
	selectedCharacter->attackDone->localVisible = true;

	vector<CombatSystem::CharacterReference*> referenceTargets;
	for (size_t i = 0; i < targetCharacters.size(); i++)
	{
		referenceTargets.emplace_back(targetCharacters[i]->characterRef);
	}
	attacksToExecute[selectedCharacter->characterRef] = { selectedAttack->attack,referenceTargets };
	
	HideAttackInformation();

	SetTargetSelectionMode(false);
	DeselectChatacter();
}

void CombatCG::EndTurn()
{
	for (auto& attackData : attacksToExecute)
	{
		combat->AddAttack(attackData.second.first, *attackData.first, attackData.second.second);
	}
	
	selectedAttack = nullptr;

	DeselectChatacter();
	RemoveAllTargets();

	combat->ChangeState(CombatSystem::CombatState::ENEMY_TURN);

	passTurn->localVisible = false;
	passTurn->interactable = false;

}

void CombatCG::OnCombatStateChanged()
{
	switch (combat->GetCombatState())
	{
		case CombatSystem::CombatState::PLAYER_TURN:
		{

			if (passTurn!=nullptr) {
				passTurn->localVisible = true;
				passTurn->interactable = true;
			}

			attacksToExecute.clear();
			for (size_t i = 0; i < charactersSlot.size(); i++)
			{
				charactersSlot[i].attackDone->localVisible = false;
			}
			break;
		}
	}
}

void CombatCG::SetTargetSelectionMode(bool mode)
{
	selectingTargets = mode;


	confirmAttack->localVisible = mode;
	confirmAttack->interactable = mode;
	cancelAttack->localVisible = mode;
	cancelAttack->interactable = mode;

	if (selectingTargets) {
		passTurn->localVisible = false;
		passTurn->interactable = false;
		for (size_t i = 0; i < attackButtons.size(); i++)
		{
			attackButtons[i].btn->isEnabled = false;
		}
	}
	else {
		passTurn->localVisible = true;
		passTurn->interactable = true;
		RemoveAllTargets();
		SelectCharacter(*selectedCharacter);
	}

}

CombatCG::UICharacterSlot* CombatCG::GetSlotByCharacter(CombatSystem::CharacterReference* reference)
{
	for (size_t i = 0; i < charactersSlot.size(); i++)
	{
		CombatCG::UICharacterSlot* slot = &charactersSlot[i];
		if (slot->characterRef == reference)
			return slot;
	}
	return nullptr;
}

Vector2Int CombatCG::GetSlotPosition(CombatSystem::CharacterType team, int teamMemberIndex, int teamMembersAmount)
{
	Vector2 position = { 0,LOGIC_SCREEN_HEIGHT / 2 - 20 };
	Vector2 offset = { 0,0 };
	Vector2 displacement = { 90,55 };

	if (team == CombatSystem::Ally)
		position.x = LOGIC_SCREEN_WIDTH / 5;
	else
		position.x = LOGIC_SCREEN_WIDTH / 4 * 3;

	if (teamMembersAmount > 1)
	{
		offset.x = (float)std::cos((360 / teamMembersAmount) * teamMemberIndex * M_PI / 180);
		if (team == CombatSystem::Enemy)
			offset.x *= -1;
		offset.y = (float)std::sin((360 / teamMembersAmount) * teamMemberIndex * M_PI / 180);
	}

	return position + Vector2{ offset.x * displacement.x,offset.y * displacement.y };
}

void CombatCG::FinishAttackVisuals(UIAnimatedImage* characterImage)
{
	animationEffect.first = true;
	characterImage->GetAnimator()->Animate("combat-idle");
}

void CombatCG::FinishHurtVisuals(UIAnimatedImage* characterImage, CombatSystem::CharacterReference* ref)
{

	targetVisualsCompleted.first++;
	if (targetVisualsCompleted.first == targetVisualsRequest.first)
		animationEffect.first = true;
	if(ref->stats.currentHp > 0)
		characterImage->GetAnimator()->Animate("combat-idle");
	else {
		characterImage->GetAnimator()->Animate("die");
		Engine::Instance().m_audio->PlaySFX(combat_dead);
	}
}

void CombatCG::FinishEffectVisuals(UIAnimatedImage* combatEffect)
{
	targetVisualsCompleted.second++;
	if (targetVisualsCompleted.second == targetVisualsRequest.second)
		animationEffect.second = true;
	combatEffect->GetJsonAnimator()->CleanUp();
}