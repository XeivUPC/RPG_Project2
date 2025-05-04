#pragma once
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

class Item;
class ItemList
{
public:
	static ItemList& Instance() {
		static ItemList instance;
		return instance;
	}
private:
	string path;
	ItemList();
	~ItemList();

	void LoadItems();

	ItemList(const ItemList&) = delete;
	ItemList& operator=(const ItemList&) = delete;
	unordered_map<string, Item> itemList;
};