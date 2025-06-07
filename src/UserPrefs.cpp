#include "UserPrefs.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;

bool UserPrefs::Exists(string key)
{
    return prefs.count(key)>0;
}

void UserPrefs::SaveInt(string key, int value)
{
    prefs[key] = to_string(value);
}

void UserPrefs::SaveBool(string key, bool value)
{
    prefs[key] = to_string((value ? 1 : 0 ));
}

void UserPrefs::SaveFloat(string key, float value)
{
    prefs[key] = to_string(value);
}

void UserPrefs::SaveString(string key, string value)
{
    prefs[key] = value;
}

int UserPrefs::GetInt(string key, int defaultValue)
{
    if (!Exists(key)) {
        LOG("Key: %s --> UserPref Does Not Exist, Returning Default Value %d", key.c_str(), defaultValue);
        return defaultValue;
    }
    return std::stoi(prefs[key]);
}

bool UserPrefs::GetBool(string key, bool defaultValue)
{
    if (!Exists(key)) {
        LOG("Key: %s --> UserPref Does Not Exist, Returning Default Value %d", key.c_str(), defaultValue);
        return defaultValue;
    }
    return std::stoi(prefs[key]) == 1;
}

float UserPrefs::GetFloat(string key, float defaultValue)
{
    if (!Exists(key)) {
        LOG("Key: %s --> UserPref Does Not Exist, Returning Default Value %f", key.c_str(), defaultValue);
        return defaultValue;
    }
    return std::stof(prefs[key]);
}

string UserPrefs::GetString(string key, string defaultValue)
{
    if (!Exists(key)) {
        LOG("Key: %s --> UserPref Does Not Exist, Returning Default Value %s", key.c_str(), defaultValue.c_str());
        return defaultValue;
    }
    return prefs[key];
}

void UserPrefs::RemoveEntry(string key)
{
    if (Exists(key)) {
        prefs.erase(key);
    }
}

UserPrefs::UserPrefs()
{
    LoadPrefs();
}

UserPrefs::~UserPrefs()
{
    SavePrefs();
    prefs.clear();
}

void UserPrefs::SavePrefs()
{
    LOG("Saving");

    xml_document file;
    pugi::xml_parse_result result = file.load_file(pathToUserPrefs.c_str());
    if (result != NULL)
    {
        xml_node rootNode = file.child("UserPrefs");
        rootNode.remove_children();
        for (const auto& [key, value] : prefs) {
            xml_node entry = rootNode.find_child_by_attribute("Key", key.c_str());
            if (entry) {
                entry.attribute("Value").set_value(value.c_str());
            }
            else {
                xml_node newEntry = rootNode.append_child("Entry");
                newEntry.append_attribute("Key") = key.c_str();
                newEntry.append_attribute("Value") = value.c_str();
            }
        }

        file.save_file(pathToUserPrefs.c_str());

    }
    else {
        LOG("UserPrefs couldn't be saved");
    }
}

void UserPrefs::LoadPrefs()
{
    LOG("Loading");

    LOG("Loading preferences from XML");

    xml_document file;
    xml_parse_result result = file.load_file(pathToUserPrefs.c_str());

    if (result != NULL) {
        xml_node rootNode = file.child("UserPrefs");

        for (xml_node entry : rootNode.children("Entry")) {
            string key = entry.attribute("Key").value();
            string value = entry.attribute("Value").value();

            prefs[key] = value;
        }
    }
    else {
        LOG("UserPrefs couldn't be loaded");
    }
}
