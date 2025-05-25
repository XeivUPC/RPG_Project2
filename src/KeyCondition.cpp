#include "KeyCondition.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleTime.h"
#include "MissionManager.h"

KeyCondition::KeyCondition(vector<int> _keyId, vector<float> _timeToKeep, vector<int> mode)
{
	keyIds = _keyId;
	timesToKeep = _timeToKeep;
	timers = vector<StepTimer>(keyIds.size());
	for (size_t i = 0; i < mode.size(); i++)
	{
		keyMethods.emplace_back((KeyMethod)mode[i]);
		tasks.emplace_back(false);

	}
}

void KeyCondition::SetUp()
{
}

bool KeyCondition::UpdateCondition()
{
	for (size_t i = 0; i < keyMethods.size(); i++) {
		switch (keyMethods[i])
		{
		case KeyMethod::Hold:
			if (Engine::Instance().m_input->GetKey(keyIds[i]) == KEY_REPEAT) {
				timers[i].Step(ModuleTime::deltaTime);
			}
			else
				timers[i].Start();
			break;
		default:
			break;
		}
	}

	return Check();
}

bool KeyCondition::Check()
{
	if(Completed())
		return true;

	for (size_t i = 0; i < keyMethods.size(); i++) {
		if (tasks[i])
			continue;
		switch (keyMethods[i])
		{
		case KeyMethod::Press:
			tasks[i] = Engine::Instance().m_input->GetKey(keyIds[i]) == KEY_DOWN;		
			break;

		case KeyMethod::Hold:

			if (timers[i].ReadSec() >= timesToKeep[i]) {
				tasks[i] = true;
			}
			break;
		default:
			break;
		}
	}
	isDone = true;
	for (size_t i = 0; i < tasks.size(); i++)
	{
		if (!tasks[i]) {
			isDone = false;
			break;
		}
	}
	if (isDone) {
		MissionManager::Instance().UpdateMissionsStatus();
	}

	return isDone;
}
