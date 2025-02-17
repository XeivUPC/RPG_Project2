#pragma once
#include <unordered_map>
#include <string>

using namespace std;

class UserPrefs
{
public:

	static UserPrefs& Instance() {
		static UserPrefs instance;
		return instance;
	}

	bool Exists(string key);

	void SaveInt(string key, int value);
	void SaveBool(string key, bool value);
	void SaveFloat(string key, float value);
	void SaveString(string key, string value);

	int GetInt(string key, int defaultValue = 0);
	bool GetBool(string key, bool defaultValue = false);
	float GetFloat(string key, float defaultValue = 0);
	string GetString(string key, string defaultValue = "");

	void RemoveEntry(string key);

private:
	UserPrefs();
	~UserPrefs();

	UserPrefs(const UserPrefs&) = delete;
	UserPrefs& operator=(const UserPrefs&) = delete;

	void SavePrefs();
	void LoadPrefs();
private:
	string pathToUserPrefs = "Assets/Data/UserPrefs.xml";
	unordered_map<string, string> prefs;
};
