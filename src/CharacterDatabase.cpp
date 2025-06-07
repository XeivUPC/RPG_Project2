#include "CharacterDatabase.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;

CharacterDatabase::CharacterDefinition& CharacterDatabase::GetCharacterDefinition(string id)
{
    return definitions[id];
}

const unordered_map<string, CharacterDatabase::CharacterDefinition>& CharacterDatabase::GetCharacters()
{
    return definitions;
}

bool CharacterDatabase::Exists(string id)
{
    return definitions.count(id) > 0;
}

void CharacterDatabase::ResetDataToDefault()
{
    pugi::xml_document sourceDoc;
    if (!sourceDoc.load_file(pathToCharacterDefinitionsDefault.c_str())) {
        LOG("Failed to load source.xml");
        return;
    }

    pugi::xml_document targetDoc;
    if (!targetDoc.load_file(pathToCharacterDefinitions.c_str())) {
        LOG("Failed to load target.xml");
        return;
    }

    targetDoc.reset();

    pugi::xml_node newRoot = targetDoc.append_copy(sourceDoc.document_element());

    if (!targetDoc.save_file(pathToCharacterDefinitions.c_str())) {
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
    definitions.clear();
}

void CharacterDatabase::SaveDatabase()
{
    LOG("Saving");

    xml_document file;
    pugi::xml_parse_result result = file.load_file(pathToCharacterDefinitions.c_str());
    if (result != NULL)
    {
        xml_node rootNode = file.child("definition");

        for (const auto& [key, value] : definitions) {
            xml_node entry = rootNode.find_child_by_attribute("id",key.c_str());
            if (entry) {
				entry.attribute("template").set_value(value.charTemplate->id.c_str());
				entry.attribute("name").set_value(value.name.c_str());

				entry.child("recruitable").text().data().set_value(value.recruitable ? "true" : "false");

				entry.child("dialogue").attribute("path").set_value(value.dialogue.c_str());
				entry.child("dialogue").attribute("state").set_value(value.state);


				entry.child("combat").child("stats").attribute("level").set_value(value.level);
				entry.child("combat").child("stats").attribute("hp").set_value(value.health);
				entry.child("combat").child("stats").attribute("defense").set_value(value.defense);
				entry.child("combat").child("stats").attribute("attack").set_value(value.attack);
				entry.child("combat").child("stats").attribute("speed").set_value(value.speed);

				xml_node attacksNode = entry.child("combat").child("attacks");
                int index = 0;
				for (auto& attackAtr : attacksNode.attributes())
				{
					attackAtr.set_value(value.attacks[index]);
                    index++;
				}
            }
            else {
                LOG("CharacterDefinitions couldn't find this id %d", key);
            }
        }

        file.save_file(pathToCharacterDefinitions.c_str());

    }
    else {
        LOG("CharacterDefinitions couldn't be saved");
    }
}

void CharacterDatabase::LoadDatabase()
{
    LOG("Loading");
    LOG("Loading characters data from XML");

    definitions.clear();
    templates.clear();
    xml_document file;
    xml_parse_result result = file.load_file(pathToCharacterTemplate.c_str());

    if (result != NULL) {
        xml_node rootNode = file.child("template");

        for (xml_node character : rootNode.children("character")) {
            CharacterTemplate charTemplate;
            charTemplate.id = character.attribute("id").as_string();
            charTemplate.textureId = character.attribute("textureId").as_string();
            charTemplate.faceId = character.attribute("faceId").as_string();

            templates[charTemplate.id] = charTemplate;
        }
    }
    else {
        LOG("CharacterTemplates couldn't be loaded");
        return;
    }



    result = file.load_file(pathToCharacterDefinitions.c_str());
    if (result != NULL) {
        xml_node rootNode = file.child("definition");

        for (xml_node characterNode : rootNode.children("character")) {
            CharacterDefinition charDefinition;

            string charId = characterNode.attribute("id").as_string();
            string charName = characterNode.attribute("name").as_string();
            string templateId = characterNode.attribute("template").as_string();
            CharacterTemplate* charTemplate = &templates[templateId];

            xml_node recruitableNode = characterNode.child("recruitable");
            xml_node dialogueNode = characterNode.child("dialogue");
            xml_node combatStatsNode = characterNode.child("combat").child("stats");
            xml_node combatAttacksNode = characterNode.child("combat").child("attacks");

            charDefinition.id = charId;
            charDefinition.name = charName;
            charDefinition.charTemplate = charTemplate;

            charDefinition.recruitable = recruitableNode.text().as_bool();

            charDefinition.state = dialogueNode.attribute("state").as_int();
            charDefinition.dialogue = dialogueNode.attribute("path").as_string();      

            charDefinition.level = combatStatsNode.attribute("level").as_int();
            charDefinition.health = combatStatsNode.attribute("hp").as_int();
            charDefinition.defense = combatStatsNode.attribute("defense").as_int();
            charDefinition.attack = combatStatsNode.attribute("attack").as_int();
            charDefinition.speed = combatStatsNode.attribute("speed").as_int();

            for (const auto& attack : combatAttacksNode.attributes())
            {
                charDefinition.attacks.emplace_back(attack.as_int());
            }
            definitions[charId] = charDefinition;
        }
    }
    else {
        LOG("CharacterDefinitions couldn't be loaded");
    }
}
