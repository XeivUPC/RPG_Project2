#include "ItemList.h"
#include "Log.h"
#include "pugixml.hpp"
#include "Item.h"
using namespace pugi;

Item ItemList::ItemByID(string id)
{
	return itemList[id];
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
	LOG("Loading attacks data from XML");
	itemList.clear();
	xml_document file;
	xml_parse_result result = file.load_file(path.c_str());
	if (result == NULL)
	{
		LOG("Items couldn't be loaded");
		return;
	}
	xml_node rootNode = file.child("itemList");
	for (xml_node item : rootNode.children("item"))
	{
		Item newItem = Item();
		newItem.id = item.attribute("id").as_string();
		newItem.name = item.attribute("name").as_string();
		newItem.description = item.attribute("description").as_string();
		newItem.stackable_quantity = item.attribute("stackable-quantity").as_int();

		newItem.texture_id = item.attribute("texture-id").as_string();
		xml_node properties = item.child("properties");
		for (xml_node property : properties.children())
		{
			newItem.properties.emplace(property.name(), std::stof(property.child_value()));
		}
		itemList.emplace(newItem.name, newItem);
	}
}
