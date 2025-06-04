#include "PuzzleManager.h"
#include "Log.h"
#include "pugixml.hpp"
#include "PuzzleElement.h"

using namespace pugi;

PuzzleManager::PuzzleManager()
{
}

PuzzleManager::~PuzzleManager()
{
}

bool PuzzleManager::HasPuzzle(const string& id) const
{
    if (puzzlesData.count(id) != 0)
        return true;
    return false;
}

bool PuzzleManager::HasPuzzleProperty(const string& id, const string& propertyId) const
{
    if (HasPuzzle(id)) {
        if (puzzlesData.at(id).count(propertyId) != 0)
            return true;
    }
    return false;
}

string PuzzleManager::GetValueFromPuzzle(const string& id, const string& propertyId)
{
    if (HasPuzzleProperty(id, propertyId)) {
		return puzzlesData.at(id).at(propertyId);
    }
    return "";
}

void PuzzleManager::SetValueFromPuzzle(const string& id, const string& propertyId, const string& value)
{
	puzzlesData[id][propertyId] = value;
}

void PuzzleManager::ClearData()
{
    puzzlesData.clear();
}

void PuzzleManager::SaveAllData()
{
    LOG("Saving");
    LOG("Saving puzzles data to xml");

    xml_document file;
    pugi::xml_parse_result result = file.load_file(pathToSaveFile.c_str());
    if (result != NULL)
    {
        xml_node rootNode = file.child("puzzles");

		rootNode.remove_children();

		for (const auto& puzzle : puzzlesData)
		{
			xml_node puzzleNode = rootNode.append_child("puzzle");
			puzzleNode.append_attribute("id") = puzzle.first.c_str();
			for (const auto& property : puzzle.second)
			{
				xml_node propertyNode = puzzleNode.append_child("property");
				propertyNode.append_attribute("key") = property.first.c_str();
				propertyNode.append_attribute("value") = property.second.c_str();
			}
		}  

        file.save_file(pathToSaveFile.c_str());

    }
    else {
        LOG("UserPrefs couldn't be saved");
    }
}

void PuzzleManager::LoadAllData()
{
    LOG("Loading");
    LOG("Loading puzzles data from XML");
    puzzlesData.clear();
    xml_document file;
    xml_parse_result result = file.load_file(pathToSaveFile.c_str());
    if (result == NULL)
    {
        LOG("Puzzles couldn't be loaded");
        return;
    }
    xml_node rootNode = file.child("puzzles");
    for (xml_node puzzle_node : rootNode.children("puzzle"))
    {
		string id = puzzle_node.attribute("id").as_string();
		unordered_map<string, string> properties;

		for (xml_node property_node : puzzle_node.children("property"))
		{
			string propertyId = property_node.attribute("key").as_string();
			string value = property_node.attribute("value").as_string();
			properties[propertyId] = value;
		}
		puzzlesData[id] = properties;
    }
}

void PuzzleManager::AddPuzzleElement(const string& id, PuzzleElement& element)
{
	puzzles[id] = &element;
}

void PuzzleManager::RemovePuzzleElement(const PuzzleElement& element)
{
	for (auto it = puzzles.begin(); it != puzzles.end(); ++it) {
		if (it->second == &element) {
			puzzles.erase(it);
			return;
		}
	}
}

PuzzleElement* PuzzleManager::GetPuzzleElement(const string& id)
{
    if (puzzles.count(id) == 0) {
        return nullptr;
    }
	return puzzles.at(id);
}

