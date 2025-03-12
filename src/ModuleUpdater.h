#pragma once
#include "Module.h" 
#include <vector>
#include <string>
#include <unordered_map>

class ModuleUpdater : public Module {
	friend class Engine;

	

	public:
		enum class UpdateMode {
			PRE_UPDATE,
			UPDATE,
			POST_UPDATE
		};

		struct UpdateGroup {
			vector<IUpdateable*> elements;
			bool isPaused = false;
		};

		ModuleUpdater(bool start_active = true);
		~ModuleUpdater();

		void AddToUpdateQueue(IUpdateable& updateableObj, UpdateMode mode, const string& groupId = "");
		void RemoveFromUpdateQueue(IUpdateable& updateableObj, UpdateMode mode, bool removeFromGroups = true);

		void AddToUpdateGroup(IUpdateable& updateableObj, string groupID);
		void RemoveFromUpdateGroup(IUpdateable& updateableObj, const string& groupID);
		void RemoveFromUpdateGroup(IUpdateable& updateableObj);

		const UpdateGroup& GetUpdateGroup(const string& groupID);
		void PauseUpdateGroup(const string& groupID);
		void ResumeUpdateGroup(const string& groupID);
		void SetStatusUpdateGroup(const string& groupID, bool status);

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

		UpdateGroup& GetModifiableUpdateGroup(const string& groupID);

	private:
		unordered_map<UpdateMode, vector<IUpdateable*>> updatesQueue;
		unordered_map<string, UpdateGroup> groups;
		bool isPaused = false;
};

