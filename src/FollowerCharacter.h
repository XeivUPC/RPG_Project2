#pragma once
#include "Character.h"
#include "IPooleable.h"
#include "IInteractuable.h"


#include <vector>


class FollowerCharacter : public Character{

public:

	FollowerCharacter(Character* _characterToFollow, float _delayDistance, int _npcId=0);
	~FollowerCharacter();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetCharacterToFollow(Character* _characterToFollow);

public:

private:
	void SearchPath();
private:
	int pathPosition = 0;

	Character* characterToFollow = nullptr;
	float delayDistance = 0;

protected:
	// Inherited via Character
	void Move() override;
	// Inherited via Character
	void Animate() override;
protected:

};

/*
Character has array of follow charas

{ Solo funciona si el array de followers no es 0
variable smooth >> cada cuanto crea un punto de referencia
npcs siguen los puntos de referencia
variable cuanto puede llegar a medir el path (queue)
solo se crean muntos si el player se ha movido, si esta en estatico no
}

Follower, contiene un referencia a un character, y un delay/ distancia al player (para calcular la distancia, medimo de punto a punto)
se mueve a la misma speed del char referente (basespeed* speedModifier)
renderiza por ahora la textura de un npc


*/