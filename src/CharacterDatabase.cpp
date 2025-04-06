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

void CharacterDatabase::ResetDataToDefault()
{
    pugi::xml_document sourceDoc;
    if (!sourceDoc.load_file(pathToCharacterDataDefault.c_str())) {
        LOG("Failed to load source.xml");
        return;
    }

    pugi::xml_document targetDoc;
    if (!targetDoc.load_file(pathToCharacterData.c_str())) {
        LOG("Failed to load target.xml");
        return;
    }

    targetDoc.reset();

    pugi::xml_node newRoot = targetDoc.append_copy(sourceDoc.document_element());

    if (!targetDoc.save_file(pathToCharacterData.c_str())) {
        LOG("Failed to save updated target.xml");
        return;
    }

    LoadDatabase();
}

CharacterDatabase::CharacterDatabase()
{
    LoadDatabase();
}

CharacterDatabase::~CharacterDatabase()
{
	SaveDatabase();
}

void CharacterDatabase::SaveDatabase()
{
    LOG("Saving");

    xml_document file;
    pugi::xml_parse_result result = file.load_file(pathToCharacterData.c_str());
    if (result != NULL)
    {
        xml_node rootNode = file.child("Characters");

        for (const auto& [key, value] : data) {
            xml_node entry = rootNode.find_child_by_attribute("id", to_string(key).c_str());
            if (entry) {
                entry.child("Data").attribute("textureId").set_value(value.textureId.c_str());
				entry.child("Data").attribute("dialoguePath").set_value(value.dialoguePath.c_str());

				entry.child("Stats").attribute("state").set_value(value.state);
				entry.child("Stats").attribute("friendship").set_value(value.friendShip);
				entry.child("Stats").attribute("love").set_value(value.love);

				entry.child("CombatStats").attribute("level").set_value(value.level);
				entry.child("CombatStats").attribute("hp").set_value(value.health);
				entry.child("CombatStats").attribute("defense").set_value(value.defense);
				entry.child("CombatStats").attribute("attack").set_value(value.attack);
				entry.child("CombatStats").attribute("speed").set_value(value.speed);

				xml_node attacksNode = entry.child("Attacks");
                int index = 0;
				for (auto& attackAtr : attacksNode.attributes())
				{
					attackAtr.set_value(value.attacks[index]);
                    index++;
				}

                entry.child("AttackRole").attribute("type").set_value(value.role);
            }
            else {
                LOG("CharacterDatabase couldn't find this id %d", key);
            }
        }

        file.save_file(pathToCharacterData.c_str());

    }
    else {
        LOG("CharacterDatabase couldn't be saved");
    }
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
            xml_node charCombatStatsNode = character.child("CombatStats");
            xml_node charAttacksNode = character.child("Attacks");
            xml_node charAttackRoleNode = character.child("AttackRole");

            ///Basic
            charData.id = charId;
            charData.name = charName;

            ///Stats
            charData.state = charStatsNode.attribute("state").as_int();
            charData.friendShip = charStatsNode.attribute("friendShip").as_int();
            charData.love = charStatsNode.attribute("love").as_int();

            ///Data
            charData.textureId = charDataNode.attribute("textureId").as_string();
            charData.dialoguePath = charDataNode.attribute("dialoguePath").as_string();

            charData.level = charCombatStatsNode.attribute("level").as_int();
            charData.health = charCombatStatsNode.attribute("hp").as_int();
            charData.defense = charCombatStatsNode.attribute("defense").as_int();
            charData.attack = charCombatStatsNode.attribute("attack").as_int();
            charData.speed = charCombatStatsNode.attribute("speed").as_int();

            for (const auto& attack : charAttacksNode.attributes())
            {
                charData.attacks.emplace_back(attack.as_int());
            }

            charData.role = (CharacterData::CharacterRole)charAttackRoleNode.attribute("type").as_int();

            data[charId] = charData;
        }
    }
    else {
        LOG("CharacterDatabase couldn't be loaded");
    }
}
