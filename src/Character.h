#pragma once
#include "Entity.h"
#include "CharacterDatabase.h"
#include <deque>
#include "StepTimer.h"
#include <vector>

class PhysBody;
class Animator;
class FollowerCharacter;

class Character : public Entity {

	friend class FollowerCharacter;
public:

	Character();
	~Character();

	bool Update() override;
	void Render() override;

	bool CleanUp() override;

	void SetPosition(Vector2 newPosition) override;
	virtual bool SetCharacterId(string _charId);
	virtual string GetCharacterId() const;

	void ClearFollowerPath();

	Animator& GetAnimator() const;

public:
private:

private:	

protected:
	virtual void Animate() = 0;
	virtual void Move() = 0;
protected:
	string characterId = "";
	CharacterDatabase::CharacterDefinition* characterData = nullptr;

	SDL_Texture* texture = nullptr;
	Animator* animator;
	PhysBody* body = nullptr;


	Vector2 moveDirection = { 0,0 };
	Vector2 lastDirection = { 0,1 };

	float baseSpeed = 5;
	float speedModifier = 1;
	float runSpeedModifier = 1.5f;

	Vector2 previousPhysicsPosition = { 0,0 };

	
	bool AddFollower(string _charId, float distance);
	bool RemoveFollowerById(string _charId);
	bool RemoveFollowerByIndex(int _charIndex);
	bool EditFollower(string _charId, int _charIndex);
	bool GetFollowers() const;
	void SetFollowers(vector<string> ids,float distance);

	vector<FollowerCharacter*> followers;
	deque<Vector2> pathFollowersData;
	StepTimer updateFollowerPathTimer;
	/// <summary>
	/// The time it takes to add a new point to the follower's path. The lower the smoother.
	/// </summary>
	float followerPathSmoothing = 0.2f;
	float maxFollowersPathDistance = 30;
	int maxFollowers = 3;
	int currentFollowers = 0;
	float totalFollowerDistance = 0;
};