#pragma once
#include <unordered_map>
#include <string>

using namespace std;
class CharacterDatabase {
	public:

		struct CharacterTemplate {
			string id;
			string textureId;
			string faceId;
		};

		struct CharacterDefinition {
			string id = "character;test";
			string name = "";

			CharacterTemplate* charTemplate;

			/// Party
			bool recruitable = false;

			/// Dialogue
			string dialogue = "";
			int state = 0;

			/// Combat Data
			vector<int> attacks;
			// Stats
			int level = 2;
			int health = 0;
			int attack = 0;
			int defense = 0;
			int speed = 0;
		};

		static CharacterDatabase& Instance() {
			static CharacterDatabase instance;
			return instance;
		}

		CharacterDefinition& GetCharacterDefinition(string id);
		const unordered_map<string, CharacterDefinition>& GetCharacters();

		bool Exists(string id);

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
		string pathToCharacterDefinitions = "Assets/Data/Characters/SaveData/CharacterDefinitions.xml";
		string pathToCharacterDefinitionsDefault = "Assets/Data/Characters/CharacterDefinitions_Default.xml";

		string pathToCharacterTemplate = "Assets/Data/Characters/CharacterTemplate.xml";
		unordered_map<string, CharacterDefinition> definitions;
		unordered_map<string, CharacterTemplate> templates;
	protected:
	protected:
};