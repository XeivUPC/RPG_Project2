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

TextureAtlas* AtlasFactory::CreateAtlas(SDL_Texture& texture, const string& xmlInfoPath) const
{
	TextureAtlas* atlas = new TextureAtlas(&texture);

	xml_document atlasXML;
	xml_parse_result result = atlasXML.load_file(xmlInfoPath.c_str());

	if (result)
	{
		LOG("%s parsed without errors", xmlInfoPath.c_str());

		xml_node atlasNode = atlasXML.child("AtlasTexture");
		for (const auto& spriteNode : atlasNode.children())
		{
			string id = spriteNode.attribute("nameId").as_string();

			Vector2Int origin;
			origin.x = spriteNode.attribute("originX").as_int();
			origin.y = spriteNode.attribute("originY").as_int();

			Vector2Int position;
			position.x = spriteNode.attribute("positionX").as_int();
			position.y = spriteNode.attribute("positionY").as_int();

			Vector2Int size;
			size.x = spriteNode.attribute("sourceWidth").as_int();
			size.y = spriteNode.attribute("sourceHeight").as_int();

			TextureAtlas::AtlasSprite spriteData = TextureAtlas::AtlasSprite{ move(origin), {position.x, position.y, size.x, size.y}};
			atlas->sprites.emplace(move(id), move(spriteData));
		}
	}
	else
	{
		LOG("Error loading atlasXML: %s", result.description());
	}
	return atlas;
}

void AtlasFactory::DeleteAtlas(TextureAtlas& atlas) const
{
	delete& atlas;
}
