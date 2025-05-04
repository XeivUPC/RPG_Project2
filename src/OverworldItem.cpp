#include "OverworldItem.h"
#include "ModuleAssetDatabase.h"
#include "Item.h"

OverworldItem::OverworldItem()
{
	
}

OverworldItem::~OverworldItem()
{
	texture = nullptr;
}

void OverworldItem::Initialize(Item item, int q, Vector2 position)
{
	/*id = item.id;
	texture = 
	quantity = q;*/
	//Spawn in position
}

void OverworldItem::InitPoolObject()
{
}

void OverworldItem::ResetPoolObject()
{
	Reset();
}
