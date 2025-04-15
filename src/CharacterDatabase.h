#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class CharacterDatabase {
	public:

		struct CharacterData {

			enum CharacterRole {
				DPS,
				TANK,
				HEALER,
				ALCHEMIST
			};

			int id = 0;
			string name = "";

			string textureId = "";
			string faceId = "";
			string dialoguePath = "";

			//// Relations
			int love = 0;
			int friendShip = 0;
			int state = 0;

			//// Stats
			int level = 2;
			int health = 0;
			int attack = 0;
			int defense = 0;
			int speed=0;

			vector<int> attacks;

			CharacterRole role = DPS;
		};

		static CharacterDatabase& Instance() {
			static CharacterDatabase instance;
			return instance;
		}

		CharacterData& GetCharacterData(int id);
		const unordered_map<int, CharacterData>& GetCharacters();

		bool Exists(int id);

		void ResetDataToDefault();

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
		string pathToCharacterDataDefault = "Assets/Data/CharactersData_Default.xml";
		unordered_map<int, CharacterData> data;
	protected:
	protected:
};