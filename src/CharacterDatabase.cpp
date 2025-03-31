#include "CharacterDatabase.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;

CharacterDatabase::CharacterData& CharacterDatabase::GetCharacterData(int id)
{
    return data[id];
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
            xml_node charStatsNode = character.child("Stats");
            xml_node charAttacksNode = character.child("Attacks");


            charData.id = charId;
            charData.name = charName;
            charData.textureId = charDataNode.attribute("textureId").as_string();
            charData.dialoguePath = charDataNode.attribute("dialoguePath").as_string();

            charData.health = charStatsNode.attribute("hp").as_int();
            charData.defense = charStatsNode.attribute("defense").as_int();
            charData.attack = charStatsNode.attribute("attack").as_int();
            charData.speed = charStatsNode.attribute("speed").as_int();

            for (const auto& attack : charAttacksNode.attributes())
            {
                charData.attacks.emplace_back(attack.as_int());
            }

            data[charId] = charData;
        }
    }
    else {
        LOG("UserPrefs couldn't be loaded");
    }
}
