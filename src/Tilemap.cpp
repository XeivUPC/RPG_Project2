#include "Tilemap.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "Camera.h"
#include "ModuleAssetDatabase.h"
#include "DrawingTools.h"
#include "LOG.h"

#include <sstream>

Tilemap::Tilemap(string filename, float _scale)
{
    LoadFromXML(move(filename), _scale);
}

Tilemap::~Tilemap()
{
}

bool Tilemap::LoadFromXML(string filename, float _scale)
{
    scale = _scale;
    xml_document doc;
    xml_parse_result result = doc.load_file(filename.c_str());

    if (result != NULL)
    {
        const path mapPath(filename);
        const string mapDir = mapPath.parent_path().string();

        xml_node mapNode = doc.child("map");

        tileSize = {
            mapNode.attribute("tilewidth").as_int(),
            mapNode.attribute("tileheight").as_int()
        };

        for (xml_node tsNode : mapNode.children("tileset")) {
            ParseTileset(tsNode, mapPath.parent_path());
        }



        for (xml_node layerNode : mapNode.children("layer")) {
            ParseLayer(layerNode);
        }
        for (xml_node objGroupNode : mapNode.children("objectgroup")) {
            ParseObjectLayer(objGroupNode);
        }

        return true;

    }
    else {
        LOG("Tilemap couldn't be loaded");
        return false;
    }   
}

void Tilemap::UpdateTilemap()
{
}


void Tilemap::RenderTilemap()
{
    ModuleRender& renderer = *Engine::Instance().m_render;
    const DrawingTools& painter = renderer.painter();
    Vector2Int drawingPos = { 0,0 };
    SDL_Rect rect = { 0,0,0,0 };
    SDL_Rect cameraRect = renderer.GetCamera().GetRect();

    Tileset* tileset = nullptr;

    int startX = (int)((cameraRect.x - position.x) / (tileSize.x * scale));
    int startY = (int)((cameraRect.y - position.y) / (tileSize.y * scale));
    int endX = (int)(((cameraRect.x + cameraRect.w) - position.x) / (tileSize.x * scale) + 1);
    int endY = (int)(((cameraRect.y + cameraRect.h) - position.y) / (tileSize.y * scale) + 1);

    startX = std::max(0, startX);
    startY = std::max(0, startY);

    for (const auto& layer : layers) {
        if (!layer.visible) continue;

        endX = min(layer.layerSize.x, endX);
        endY = min(layer.layerSize.y, endY);

        for (int y = startY; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                const int index = GetTileRelativeIndex(x, y, layer);
                const int gid = layer.tiles[index];

                if (gid == 0)
                    continue;

                tileset = nullptr;
                if (lastTilesetUsed && gid >= lastTilesetUsed->firstGid) {
                    int nextGid = lastTilesetUsed->firstGid + lastTilesetUsed->tileCount;
                    if (gid < nextGid) {
                        tileset = lastTilesetUsed;
                    } 
                }
                if(tileset==nullptr)
                    tileset = GetTileset(gid);
                if (!tileset || !tileset->texture) continue;

                const int tileId = gid - tileset->firstGid;
                GetTileRect(tileset, tileId, rect);

                drawingPos.x = (int)(x * tileSize.x * scale);
                drawingPos.y = (int)(y * tileSize.y * scale);

                painter.RenderTexture(*tileset->texture, drawingPos + position, &rect, { scale,scale });
            }
        }
    }

}


void Tilemap::ParseTileset(xml_node tsNode, const path& basePath)
{
    Tileset tileset;
    tileset.firstGid = tsNode.attribute("firstgid").as_int();
    

    if (xml_attribute source = tsNode.attribute("source")) {
        fs::path tsxPath = basePath / source.as_string();
        tsxPath = fs::canonical(tsxPath);

        xml_document tsxDoc;
        if (!tsxDoc.load_file(tsxPath.string().c_str())) return;

        xml_node tsxNode = tsxDoc.child("tileset");
        tileset.name = tsxNode.attribute("name").as_string();
        tileset.tileSize = {
            tsxNode.attribute("tilewidth").as_int(),
            tsxNode.attribute("tileheight").as_int()
        };
        tileset.tileCount = tsxNode.attribute("tilecount").as_int();
        tileset.spacing = tsxNode.attribute("spacing").as_int(0);
        tileset.margin = tsxNode.attribute("margin").as_int(0);
        tileset.columns = tsxNode.attribute("columns").as_int();

        tileset.texture = Engine::Instance().m_assetsDB->GetTexture(tileset.name);

       /* for (xml_node tileNode : tsxNode.children("tile")) {
            int tileId = tileNode.attribute("id").as_int();
            std::vector<SDL_Rect> frames;

            for (xml_node frameNode : tileNode.child("animation").children("frame")) {
                int frameId = frameNode.attribute("tileid").as_int();
                frames.push_back(GetTileRect(tileset.firstGid, frameId));
            }

            if (!frames.empty()) {
                tileset.animations[tileId] = frames;
            }
        }*/
    }

    tilesets.push_back(tileset);
}

void Tilemap::ParseLayer(xml_node layerNode)
{
    TileLayer layer;
    layer.name = layerNode.attribute("name").as_string();
    layer.layerSize = {
        layerNode.attribute("width").as_int(),
        layerNode.attribute("height").as_int()
    };
    layer.visible = layerNode.attribute("visible").as_bool(true);

    if (xml_node propertiesNode = layerNode.child("properties")) {
        for (xml_node propNode : propertiesNode.children("property")) {
            const char* name = propNode.attribute("name").as_string();
            const char* value = propNode.attribute("value").as_string();
            layer.properties[name] = value;
        }
    }

    if (xml_node dataNode = layerNode.child("data")) {
        string encoding = dataNode.attribute("encoding").as_string();
        if (encoding == "csv") {
            string csvData = dataNode.text().get();

            csvData.erase(remove(csvData.begin(), csvData.end(), '\n'), csvData.end());
            csvData.erase(remove(csvData.begin(), csvData.end(), '\r'), csvData.end());

            stringstream ss(csvData);
            string token;

            while (getline(ss, token, ',')) {
                if (!token.empty()) {
                    layer.tiles.push_back(stoi(token));
                }
            }
        }
    }

    layers.push_back(layer);
}

void Tilemap::ParseObjectLayer(xml_node objectGroupNode)
{
    ObjectLayer objectLayer;
    objectLayer.name = objectGroupNode.attribute("name").as_string();


    if (xml_node propertiesNode = objectGroupNode.child("properties")) {
        for (xml_node propNode : propertiesNode.children("property")) {
            const char* name = propNode.attribute("name").as_string();
            const char* value = propNode.attribute("value").as_string();
            objectLayer.properties[name] = value;
        }
    }

    objectLayers.push_back(objectLayer);
}

void Tilemap::GetTileRect(Tileset* tileset, int tileId, SDL_Rect& rect)
{
	if (!tileset) {
		rect = { 0, 0, 0, 0 };
		return;
	}
	const int tx = (tileId % tileset->columns) * (tileset->tileSize.x + tileset->spacing) + tileset->margin;
	const int ty = (tileId / tileset->columns) * (tileset->tileSize.y + tileset->spacing) + tileset->margin;
	rect = { tx, ty, tileset->tileSize.x, tileset->tileSize.y };
}

int Tilemap::GetTileRelativeIndex(int x, int y, const TileLayer& layer) const
{
	return y * layer.layerSize.x + x;
}


Tileset* Tilemap::GetTileset(int gid)
{
    auto it = std::upper_bound(tilesets.begin(), tilesets.end(), gid,
        [](int gid, const Tileset& ts) { return gid < ts.firstGid; });

    if (it != tilesets.begin()) {
        --it;
        lastTilesetUsed = &(*it);
        return lastTilesetUsed;
    }
    else {
        return nullptr;
    }
}