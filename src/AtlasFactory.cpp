#include "AtlasFactory.h"
#include "Log.h"
#include "pugixml.hpp"

using namespace pugi;

AtlasFactory::AtlasFactory()
{
}

AtlasFactory::~AtlasFactory()
{
}

Atlas* AtlasFactory::CreateAtlas(SDL_Texture& texture, const string& xmlInfoPath) const
{
	Atlas* atlas = new Atlas(&texture);

	xml_document atlasXML;
	xml_parse_result result = atlasXML.load_file(xmlInfoPath.c_str());

	if (result)
	{
		LOG("%s parsed without errors", xmlInfoPath.c_str());

		xml_node atlasNode = atlasXML.child("AtlasTexture");
		for (const auto& spriteNode : atlasNode.children())
		{
			string id = spriteNode.attribute("nameId").as_string();

			Vector2 origin;
			origin.x = spriteNode.attribute("originX").as_float();
			origin.y = spriteNode.attribute("originY").as_float();

			Vector2 position;
			position.x = spriteNode.attribute("positionX").as_float();
			position.y = spriteNode.attribute("positionY").as_float();

			Vector2 size;
			size.x = spriteNode.attribute("sourceWidth").as_float();
			size.y = spriteNode.attribute("sourceHeight").as_float();

			Atlas::AtlasSprite spriteData = Atlas::AtlasSprite{ move(origin), move(position), move(size) };
			atlas->sprites.emplace(move(id), move(spriteData));
		}
	}
	else
	{
		LOG("Error loading atlasXML: %s", result.description());
	}
	return atlas;
}

void AtlasFactory::DeleteAtlas(Atlas& atlas) const
{
	delete& atlas;
}
