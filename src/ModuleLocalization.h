#pragma once

#include "Module.h"
#include <string>
#include <unordered_map>
#include <vector>

enum Language {
	ENGLISH,
	SPANISH,
	CATALAN
};
class ModuleLocalization : public Module
{
public:
	ModuleLocalization(bool start_active = true);
	~ModuleLocalization();

	bool Start() override;
	bool CleanUp() override;

	void ChangeLanguage(Language language);
	Language GetLanguage();

	std::string GetString(const std::string& key);
	std::string FormatNumber(float number, int precision);
	
public:

private:
	void ExtractData();

private:
	Language currentLanguage= Language::ENGLISH;
	std::unordered_map<std::string, std::vector<std::string>> localizedStrings;
};

