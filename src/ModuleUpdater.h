#pragma once
#include "Module.h" 
#include <vector>
#include <unordered_map>

class ModuleUpdater : public Module {
	friend class Engine;

	

	public:
		enum class UpdateMode {
			PRE_UPDATE,
			UPDATE,
			POST_UPDATE
		};

		ModuleUpdater(bool start_active = true);
		~ModuleUpdater();

		void AddToUpdateQueue(IUpdateable& updateableObj, UpdateMode mode);
		void RemoveFomUpdateQueue(IUpdateable& updateableObj, UpdateMode mode);

		void Pause();
		void Resume();
		void SetPauseStatus(bool status);
		bool IsPaused();
	public:
	

	private:
		// Inherited via IUpdateable
		bool PreUpdate() override;
		// Inherited via IUpdateable
		bool Update() override;
		// Inherited via IUpdateable
		bool PostUpdate() override;
		// Inherited via ICleanable
		bool CleanUp() override;

		bool PreUpdateAll();
		bool UpdateAll();
		bool PostUpdateAll();

	private:
		unordered_map<UpdateMode, vector<IUpdateable*>> updatesQueue;
		bool isPaused = false;
};

