#include "Tilemap.h"
#include "Engine.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "PhysicFactory.h"
#include "Camera.h"
#include "TextureAtlas.h"
#include "ModuleAssetDatabase.h"
#include "DrawingTools.h"
#include "Pooling.h"
#include "LOG.h"

#include "Building.h"
#include "SimpleMapObject.h"

#include <sstream>

Tilemap::Tilemap(const  path& tmxPath, float _scale)
{ 
    LoadMapFromXML(tmxPath, _scale);
    Engine::Instance().m_render->AddToRenderQueue(*this);
    renderLayer = 2;
  
}

Tilemap::~Tilemap()
{
    Engine::Instance().m_render->RemoveFomRenderQueue(*this);
}


void Tilemap::LoadMapFromXML(const  path& tmxPath)
{
    currentMap = TiledMap{};
    currentMapPath = tmxPath.parent_path();

    xml_document doc;
    if (!doc.load_file(tmxPath.c_str())) throw runtime_error("Failed to load TMX file");

    xml_node mapNode = doc.child("map");
    currentMap.width = mapNode.attribute("width").as_int();
    currentMap.height = mapNode.attribute("height").as_int();
    currentMap.tileWidth = mapNode.attribute("tilewidth").as_int();
    currentMap.tileHeight = mapNode.attribute("tileheight").as_int();

    ParseProperties(mapNode, currentMap.properties);

    // Parse tilesets
    for (xml_node tsNode : mapNode.children("tileset")) {
        ParseTileset(tsNode, currentMapPath);
    }

    // Parse layers
    for (xml_node layerNode : mapNode.children()) {
        ParseLayer(layerNode, currentMapPath);
    }
}

void Tilemap::LoadMapFromXML(const  path& tmxPath, float _scale)
{
    SetScale(_scale);
    LoadMapFromXML(tmxPath);
}


void Tilemap::CreateObjects()
{
    for (size_t i = 0; i < currentMap.objectLayers.size(); i++)
    {
        const ObjectGroupLayer* layer = &currentMap.objectLayers[i];
        for (size_t j = 0; j < layer->objects.size(); j++)
        {
            const MapObject* object = &layer->objects[j];
            const Tileset* tileset = GetTileset(object->gid);
            int local_gid = 0; 
            if(tileset!=nullptr)
                local_gid = object->gid - tileset->firstGid;
            string type = "";
            if(object->properties.count("Type"))
                type = object->properties.at("Type").get<string>();

            if (type == "simpleObject") {
                Vector2 position = { object->x + object->width / 2 * scale ,object->y };

                auto simpleObject = Pooling::Instance().AcquireObject<SimpleMapObject>();
                simpleObject->SetData(tileset->name, tileset->tiles.at(local_gid).textureId, position,scale);
            }
            else if(type == "building"){
                Vector2 position = { object->x + object->width / 2 * scale ,object->y };

                auto building = Pooling::Instance().AcquireObject<Building>();
                const TileData* tileData = &tileset->tiles.at(local_gid);
                building->SetData(tileset->name, tileData->textureId, position, scale);

                const TileObject* tileObject = &tileData->objects.at("collision");
                building->AddCollision({PIXEL_TO_METERS(position.x+tileObject->x),PIXEL_TO_METERS(position.y-tileObject->y)}, { PIXEL_TO_METERS(tileObject->width),PIXEL_TO_METERS(tileObject->height)});
            }
        }
    }
}


void Tilemap::ParseTileset(const xml_node& tsNode, const  path& baseDir) {
    Tileset ts;
    ts.firstGid = tsNode.attribute("firstgid").as_int();

    if (tsNode.attribute("source")) {
         path tsxPath = baseDir / tsNode.attribute("source").as_string();
        xml_document tsxDoc;
        if (!tsxDoc.load_file(tsxPath.c_str())) throw runtime_error("Failed to load TSX file");

        xml_node tilesetNode = tsxDoc.child("tileset");
        ts.name = tilesetNode.attribute("name").as_string();
        ts.tileWidth = tilesetNode.attribute("tilewidth").as_int();
        ts.tileHeight = tilesetNode.attribute("tileheight").as_int();
        ts.tileCount = tilesetNode.attribute("tilecount").as_int();
        ts.spacing = tilesetNode.attribute("spacing").as_int(0);
        ts.margin = tilesetNode.attribute("margin").as_int(0);
        ts.columns = tilesetNode.attribute("columns").as_int(0);

        if (xml_node imageNode = tilesetNode.child("image")) {
             path sourcePath = imageNode.attribute("source").as_string();
            ts.texture = Engine::Instance().m_assetsDB->GetTexture(ts.name);
        }

        for (xml_node tileNode : tilesetNode.children("tile")) {
            TileData tileData;
            tileData.id = tileNode.attribute("id").as_int();
            ParseTileData(tileNode, tileData, ts, tsxPath.parent_path());
            ts.tiles[tileData.id] = tileData;
        }
    }

    currentMap.tilesets.push_back(ts);
}

void Tilemap::ParseTileData(xml_node& tileNode, TileData& tileData, const Tileset& tileset, const  path& baseDir) {
    ParseProperties(tileNode, tileData.properties);

    if (xml_node imageNode = tileNode.child("image")) {
         path sourcePath = imageNode.attribute("source").as_string();
        tileData.textureId = sourcePath.stem().string();
    }

    if (xml_node animationNode = tileNode.child("animation")) {
        for (xml_node frameNode : animationNode.children("frame")) {
            tileData.animation.push_back({
                frameNode.attribute("tileid").as_int(),
                frameNode.attribute("duration").as_int()
                });
        }
    }

    if (xml_node objectGroup = tileNode.child("objectgroup")) {
        for (xml_node objNode : objectGroup.children("object")) {
            TileObject obj;
            ParseObject(objNode, obj, baseDir);
            tileData.objects[obj.name] = obj;
        }
    }
}

void Tilemap::ParseLayer(const xml_node& layerNode, const  path& baseDir) {
    const string nodeName = layerNode.name();

    if (nodeName == "layer") {
        TileLayer layer;
        layer.name = layerNode.attribute("name").as_string();
        layer.width = layerNode.attribute("width").as_int();
        layer.height = layerNode.attribute("height").as_int();
        layer.opacity = layerNode.attribute("opacity").as_float(1.0f);
        layer.visible = layerNode.attribute("visible").as_bool(true);
        ParseProperties(layerNode, layer.properties);

        if (xml_node dataNode = layerNode.child("data")) {
            string encoding = dataNode.attribute("encoding").as_string();
            if (encoding == "csv") {
                string csvData = dataNode.text().as_string();
                stringstream ss(csvData);
                string line;

                while (getline(ss, line)) {
                    stringstream lineSS(line);
                    string cell;

                    while (getline(lineSS, cell, ',')) {
                        LayerTile tile;
                        int gid = stoi(cell);
                        tile.gid = gid;
                        layer.data.push_back(tile);
                    }
                }
            }
        }
        currentMap.tileLayers.push_back(layer);
    }
    else if (nodeName == "objectgroup") {
        ObjectGroupLayer objLayer;
        objLayer.name = layerNode.attribute("name").as_string();
        objLayer.opacity = layerNode.attribute("opacity").as_float(1.0f);
        objLayer.visible = layerNode.attribute("visible").as_bool(true);
        ParseProperties(layerNode, objLayer.properties);

        for (xml_node objNode : layerNode.children("object")) {
            MapObject obj;
            ParseObject(objNode, obj, baseDir);
            objLayer.objects.push_back(obj);
        }
        currentMap.objectLayers.push_back(objLayer);
    }
}

void Tilemap::ParseObject(const xml_node& objNode, MapObject& object, const  path& baseDir) {
    object.id = objNode.attribute("id").as_int();
    object.gid = objNode.attribute("gid").as_int(0);
    object.name = objNode.attribute("name").as_string();
    object.type = objNode.attribute("type").as_string();
    object.x = objNode.attribute("x").as_float();
    object.y = objNode.attribute("y").as_float();
    object.width = objNode.attribute("width").as_float(0);
    object.height = objNode.attribute("height").as_float(0);
    object.rotation = objNode.attribute("rotation").as_float(0);
    object.visible = objNode.attribute("visible").as_bool(true);
    ParseProperties(objNode, object.properties);

    for (xml_node shapeNode : objNode.children()) {
        ObjectShape shape;
        const string type = shapeNode.name();

        if (type == "polygon" || type == "polyline") {
            shape.type = (type == "polygon") ?
                ObjectShape::Type::Polygon : ObjectShape::Type::Polyline;

            string pointsStr = shapeNode.attribute("points").as_string();
            stringstream ss(pointsStr);
            string point;

            while (getline(ss, point, ' ')) {
                size_t comma = point.find(',');
                float x = stof(point.substr(0, comma));
                float y = stof(point.substr(comma + 1));
                shape.points.emplace_back(x, y);
            }
        }
        else if (type == "ellipse") {
            shape.type = ObjectShape::Type::Ellipse;
            shape.points.emplace_back(object.width / 2, object.height / 2);
        }
        else {
            shape.type = ObjectShape::Type::Rectangle;
        }

        object.shapes.push_back(shape);
    }
}

void Tilemap::ParseObject(const pugi::xml_node& objNode, TileObject& object, const  path& baseDir)
{
    object.name = objNode.attribute("name").as_string();
    object.type = objNode.attribute("type").as_string();
    object.x = objNode.attribute("x").as_float();
    object.y = objNode.attribute("y").as_float();
    object.width = objNode.attribute("width").as_float(0);
    object.height = objNode.attribute("height").as_float(0);
    object.rotation = objNode.attribute("rotation").as_float(0);
    ParseProperties(objNode, object.properties);

    for (xml_node shapeNode : objNode.children()) {
        ObjectShape shape;
        const string type = shapeNode.name();

        if (type == "polygon" || type == "polyline") {
            shape.type = (type == "polygon") ?
                ObjectShape::Type::Polygon : ObjectShape::Type::Polyline;

            string pointsStr = shapeNode.attribute("points").as_string();
            stringstream ss(pointsStr);
            string point;

            while (getline(ss, point, ' ')) {
                size_t comma = point.find(',');
                float x = stof(point.substr(0, comma));
                float y = stof(point.substr(comma + 1));
                shape.points.emplace_back(x, y);
            }
        }
        else if (type == "ellipse") {
            shape.type = ObjectShape::Type::Ellipse;
            shape.points.emplace_back(object.width / 2, object.height / 2);
        }
        else { 
            shape.type = ObjectShape::Type::Rectangle;
        }

        object.shapes.push_back(shape);
    }
}

void Tilemap::ParseProperties(const xml_node& node, unordered_map<string, Property>& props) {
    if (xml_node propertiesNode = node.child("properties")) {
        for (xml_node propNode : propertiesNode.children("property")) {
            Property prop;
            const string type = propNode.attribute("type").as_string("string");
            const string value = propNode.attribute("value").as_string();

            if (type == "int") {
                prop.value = stoi(value);
            }
            else if (type == "float") {
                prop.value = stof(value);
            }
            else if (type == "bool") {
                prop.value = (value == "true");
            }
            else if (type == "file") {
                prop.value =  path(value);
            }
            else {
                prop.value = value;
            }

            props[propNode.attribute("name").as_string()] = prop;
        }
    }
}


void Tilemap::UpdateTilemap()
{
}

void Tilemap::Render()
{
    ModuleRender& renderer = *Engine::Instance().m_render;
    renderer.SetCameraMode(true);
    const DrawingTools& painter = renderer.painter();
    SDL_Rect cameraRect = renderer.GetCamera().GetRect();

    Vector2Int drawingPos = { 0,0 };
    Tileset* tileset = nullptr;
    SDL_Rect rect = { 0,0,0,0 };

    int startX = (int)((cameraRect.x - position.x) / (currentMap.tileWidth * scale));
    int startY = (int)((cameraRect.y - position.y) / (currentMap.tileHeight * scale));
    int endX = (int)(((cameraRect.x + cameraRect.w) - position.x) / (currentMap.tileWidth * scale) + 1);
    int endY = (int)(((cameraRect.y + cameraRect.h) - position.y) / (currentMap.tileHeight * scale) + 1);


    startX = max(0, startX);
    startY = max(0, startY);

    for (const auto& layer : currentMap.tileLayers) {
        if (!layer.visible) continue;

        endX = min(layer.width, endX);
        endY = min(layer.height, endY);

        for (int y = startY; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                const int index = GetTileRelativeIndex(x, y, layer);
                const int gid = layer.data[index].gid;

                if (gid == 0)
                    continue;

                tileset = nullptr;
                if (lastTilesetUsed && gid >= lastTilesetUsed->firstGid) {
                    int nextGid = lastTilesetUsed->firstGid + lastTilesetUsed->tileCount;
                    if (gid < nextGid) {
                        tileset = lastTilesetUsed;
                    }
                }
                if (!tileset) {
                    tileset = GetTileset(gid);
                    if (!tileset)
                        continue;
                }
                if (!tileset->texture)
                    continue;

                const int tileId = gid - tileset->firstGid;
                GetTileRect(tileset, tileId, rect);

                drawingPos.x = (int)(x * currentMap.tileWidth * scale);
                drawingPos.y = (int)(y * currentMap.tileHeight * scale);

                painter.RenderTexture(*tileset->texture, drawingPos + position, &rect, { scale,scale });
            }
        }
    }
    renderer.SetCameraMode(false);
}

void Tilemap::GetTileRect(Tileset* tileset, int tileId, SDL_Rect& rect)
{
	if (!tileset) {
		rect = { 0, 0, 0, 0 };
		return;
	}
	const int tx = (tileId % tileset->columns) * (tileset->tileWidth + tileset->spacing) + tileset->margin;
	const int ty = (tileId / tileset->columns) * (tileset->tileHeight + tileset->spacing) + tileset->margin;
	rect = { tx, ty, tileset->tileWidth, tileset->tileHeight };
}

int Tilemap::GetTileRelativeIndex(int x, int y, const TileLayer& layer) const
{
	return y * layer.width + x;
}


Tileset* Tilemap::GetTileset(int gid)
{
    auto it = upper_bound(currentMap.tilesets.begin(), currentMap.tilesets.end(), gid,
        [](int gid, const Tileset& ts) { return gid < ts.firstGid; });

    if (it != currentMap.tilesets.begin()) {
        --it;
        lastTilesetUsed = &(*it);
        return lastTilesetUsed;
    }
    else {
        return nullptr;
    }
}


void Tilemap::SetPosition(Vector2 newPosition)
{
    position = newPosition;
}

void Tilemap::SetAngle(double newAngle)
{
    angle = newAngle;
}

void Tilemap::SetScale(float newScale)
{
    scale = newScale;
}

void Tilemap::SetAnchor(Vector2 newAnchor)
{
    anchor = newAnchor;
}

Vector2 Tilemap::GetPosition()
{
    return position;
}

double Tilemap::GetAngle()
{
    return angle;
}

float Tilemap::GetScale()
{
    return scale;
}

Vector2 Tilemap::GetAnchor()
{
    return anchor;
}

