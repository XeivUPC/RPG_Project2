#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class CharacterDatabase {
	public:

		struct CharacterData {
			int id;
			string name;
			int state;
			string textureId;
			string dialoguePath;
		};

		static CharacterDatabase& Instance() {
			static CharacterDatabase instance;
			return instance;
		}

		CharacterData& GetCharacterData(int id);
		const unordered_map<int, CharacterData>& GetCharacters();

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