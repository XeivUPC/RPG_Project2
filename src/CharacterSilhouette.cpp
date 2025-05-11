#include "CharacterSilhouette.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "Animator.h"
#include "Character.h"

CharacterSilhouette::CharacterSilhouette()
{
	Engine::Instance().m_render->AddToRenderQueue(*this);
}

CharacterSilhouette::~CharacterSilhouette()
{
	Engine::Instance().m_render->RemoveFromRenderQueue(*this);
}

void CharacterSilhouette::SetCharacter(Character* _character)
{
	character = _character;
}

void CharacterSilhouette::Render()
{
	if (character != nullptr) {
		character->GetAnimator().clip()->RenderClip({ 0,0,0,(Uint8)opacity });
	}
}
