#include "CharacterDatabase.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;

CharacterDatabase::CharacterData& CharacterDatabase::GetCharacterData(int id)
{
    return data[id];
}

const unordered_map<int, CharacterDatabase::CharacterData>& CharacterDatabase::GetCharacters()
{
    return data;
}

bool CharacterDatabase::Exists(int id)
{
    return data.count(id) > 0;
}

CharacterDatabase::CharacterDatabase()
{
    LoadDatabase();
}

CharacterDatabase::~CharacterDatabase()
{
}

void CharacterDatabase::SaveDatabase()
{
}

void CharacterDatabase::LoadDatabase()
{
    LOG("Loading");
    LOG("Loading characters data from XML");
    data.clear();
    xml_document file;
    xml_parse_result result = file.load_file(pathToCharacterData.c_str());

    if (result != NULL) {
        xml_node rootNode = file.child("Characters");

        for (xml_node character : rootNode.children("Character")) {
            CharacterData charData;
            int charId = character.attribute("id").as_int();
            string charName = character.attribute("name").as_string();
            xml_node charDataNode = character.child("Data");


            charData.id = charId;
            charData.name = charName;
            charData.state = charDataNode.attribute("state").as_int();
            charData.textureId = charDataNode.attribute("textureId").as_string();
            charData.dialoguePath = charDataNode.attribute("dialoguePath").as_string();

            data[charId] = charData;
        }
    }
    else {
        LOG("UserPrefs couldn't be loaded");
    }
}
