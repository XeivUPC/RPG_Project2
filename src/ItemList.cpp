#include "ItemList.h"
#include "Log.h"
#include "pugixml.hpp"
#include "Item.h"
using namespace pugi;

Item* ItemList::ItemByID(string id)
{
	return &itemList[id];
}

ItemList::ItemList()
{
	LoadItems();
}

ItemList::~ItemList()
{
	itemList.clear();
}

void ItemList::LoadItems()
{
	LOG("Loading");
	LOG("Loading items data from XML");
	itemList.clear();
	xml_document file;
	xml_parse_result result = file.load_file(path.c_str());
	if (result == NULL)
	{
		LOG("Items couldn't be loaded");
		return;
	}
	xml_node rootNode = file.child("itemList");
	for (xml_node item_node : rootNode.children("item"))
	{
		Item newItem = Item();
		newItem.id = item_node.child("id").text().as_string();
		newItem.name = item_node.child("name").text().as_string();
		newItem.description = item_node.child("description").text().as_int();
		newItem.stackable_quantity = item_node.child("stackable-quantity").text().as_int();

		xml_node properties = item_node.child("properties");
		for (xml_node property : properties.children())
		{
			newItem.properties.emplace(property.name(),property.text().as_string());
		}
		itemList.emplace(newItem.id, newItem);
	}
}
