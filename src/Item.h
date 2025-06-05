#pragma once
#include <string>
#include <unordered_map>
using namespace std;

class Item
{
public:
	string id = "";
	string name = "";
	string description = "";

	int stackable_quantity = 0;

	unordered_map<string, string> properties;

	Item();
	~Item();
};