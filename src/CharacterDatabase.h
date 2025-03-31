#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class CharacterDatabase {
	public:

		struct CharacterData {
			int id;
			string name;
			string textureId;
			string dialoguePath;


			//// Stats
			int health;
			int attack;
			int defense;
			int speed;

		};

		static CharacterDatabase& Instance() {
			static CharacterDatabase instance;
			return instance;
		}

		CharacterData& GetCharacterData(int id);

		bool Exists(int id);

	public:
	private:
		CharacterDatabase();
		~CharacterDatabase();

		CharacterDatabase(const CharacterDatabase&) = delete;
		CharacterDatabase& operator=(const CharacterDatabase&) = delete;

		void SaveDatabase();
		void LoadDatabase();
	private:
		string pathToCharacterData = "Assets/Data/CharactersData.xml";
		unordered_map<int, CharacterData> data;
	protected:
	protected:
};