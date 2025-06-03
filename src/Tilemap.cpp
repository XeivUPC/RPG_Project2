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
#include "Animator.h"
#include "AnimationClip.h"

#include "SimpleTilemapChanger.h"
#include "SimpleMapObject.h"
#include "OverworldItem.h"
#include "NpcCharacter.h"
#include "BirdFlock.h"
#include "ButtonPuzzleElement.h"
#include "BlockingPuzzleElement.h"
#include "TriggerPuzzleElement.h"
#include "DialogueActivatorPuzzleElement.h"

#include "ItemList.h"


#include <sstream>

Tilemap::Tilemap(const  path& tmxPath, float _scale)
{ 
    LoadMapFromXML(tmxPath, _scale);
    Engine::Instance().m_render->AddToRenderQueue(*this);
    renderLayer = 1;
    
}

Tilemap::~Tilemap()
{
    
}


void Tilemap::LoadMapFromXML(const  path& tmxPath)
{
    //// Clean Animator;
    animations.clear();
    spawnPoint = { 0,0 };
    currentMap = TiledMap{};
	currentMap.path = tmxPath.string();
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

    onTilemapLoad.Trigger();
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

		shared_ptr<SimpleMapObject> collisionLayerContainer = nullptr;
        if (layer->name == "Collisions") {
            collisionLayerContainer = Pooling::Instance().AcquireObject<SimpleMapObject>();
			collisionLayerContainer->SetData({ 0,0 }, scale);
        }

        for (const auto& pairData : layer->objects)
        {
            const MapObject* object = &pairData.second;
            const Tileset* tileset = GetTileset(object->gid);
            int local_gid = 0; 
            if(tileset!=nullptr)
                local_gid = object->gid - tileset->firstGid;
            string type = "";
            if(object->properties.count("Type"))
                type = object->properties.at("Type").value;

            if (collisionLayerContainer != nullptr) {
                Vector2 position = { 0,0 };
                vector<Vector2> fixedPoints;
                if (object->shapes.size() != 0) {
                    for (size_t j = 0; j < object->shapes.size(); j++)
                    {
						switch (object->shapes[j].type)
						{
						    case ObjectShape::Type::Polyline:
							case ObjectShape::Type::Polygon:
								
                                for (size_t x = 0; x < object->shapes[j].points.size(); x++)
                                {
                                    fixedPoints.emplace_back(Vector2{ PIXEL_TO_METERS(object->shapes[j].points[x].x),PIXEL_TO_METERS(object->shapes[j].points[x].y) });
                                }
							    collisionLayerContainer->AddChainCollision({ PIXEL_TO_METERS(position.x + object->width / 2 + object->x),PIXEL_TO_METERS(position.y + object->height / 2 + object->y) }, fixedPoints);
						        break;
						    case ObjectShape::Type::Ellipse:
							    collisionLayerContainer->AddCircleCollision({ PIXEL_TO_METERS(position.x + object->width / 2 + object->x),PIXEL_TO_METERS(position.y + object->height / 2 + object->y) }, PIXEL_TO_METERS(object->width / 2));
							    break;
                            default:
                                break;
                        }
                    }
                }else
                    collisionLayerContainer->AddBoxCollision({ PIXEL_TO_METERS(position.x + object->width / 2 + object->x),PIXEL_TO_METERS(position.y + object->height / 2 + object->y) }, { PIXEL_TO_METERS(object->width),PIXEL_TO_METERS(object->height) },false);
            }

            if (type == "simpleObject") {
                Vector2 position = { object->x + object->width / 2 ,object->y };
                Vector2 cornerPosition = { object->x ,object->y - object->height };

                auto simpleObject = Pooling::Instance().AcquireObject<SimpleMapObject>();
                const TileData* tileData = &tileset->tiles.at(local_gid);
                simpleObject->SetData(tileset->name, tileData->textureId, position,scale);

                if (tileData->objects.count("collision")) {
                    for (size_t i = 0; i < tileData->objects.at("collision").size(); i++)
                    {
                        const TileObject* tileObject = &tileData->objects.at("collision")[i];
                        simpleObject->AddBoxCollision({ PIXEL_TO_METERS(cornerPosition.x + tileObject->width / 2 + tileObject->x),PIXEL_TO_METERS(cornerPosition.y + tileObject->height / 2 + tileObject->y) }, { PIXEL_TO_METERS(tileObject->width),PIXEL_TO_METERS(tileObject->height) });
                    }
                }
            }
            else if (type == "tilemapChanger") {
                Vector2 position = { object->x ,object->y };

                auto tilemapChanger = Pooling::Instance().AcquireObject<SimpleTilemapChanger>();
                tilemapChanger->SetData(position, scale);

                if (object->properties.count("TargetPath")) {
                    tilemapChanger->SetTargetTilemapPath(object->properties.at("TargetPath").value);
                }

                if (object->properties.count("EntryPoint")) {
                    tilemapChanger->SetEntryPoint(stoi(object->properties.at("EntryPoint").value));
                }
     
                tilemapChanger->SetEntryTrigger({ PIXEL_TO_METERS(position.x + object->width / 2),PIXEL_TO_METERS(position.y + object->height / 2) }, { PIXEL_TO_METERS(object->width),PIXEL_TO_METERS(object->height) });
            }
            else if (type == "building") {
                Vector2 position = { object->x + object->width / 2 * scale ,object->y };
                Vector2 cornerPosition = { object->x ,object->y - object->height };

                auto tilemapChanger = Pooling::Instance().AcquireObject<SimpleTilemapChanger>();
                const TileData* tileData = &tileset->tiles.at(local_gid);
                tilemapChanger->SetData(tileset->name, tileData->textureId, position, scale);


				string targetPath = "";
                if (object->properties.count("TargetPath")) {
                    targetPath = object->properties.at("TargetPath").value;
                    tilemapChanger->SetTargetTilemapPath(targetPath);
                }

                if (tileData->objects.count("collision")) {
                    for (size_t i = 0; i < tileData->objects.at("collision").size(); i++)
                    {
                        const TileObject* tileObject = &tileData->objects.at("collision")[i];
                        tilemapChanger->AddBoxCollision({PIXEL_TO_METERS(cornerPosition.x+ tileObject->width/2 + tileObject->x),PIXEL_TO_METERS(cornerPosition.y + tileObject->height/2 + tileObject->y)}, {PIXEL_TO_METERS(tileObject->width),PIXEL_TO_METERS(tileObject->height)});
                    }
                }
                if (tileData->objects.count("renderPosition")) {
                    const TileObject* tileObject = &tileData->objects.at("renderPosition")[i];
                    tilemapChanger->renderOffsetSorting = { (int)(cornerPosition.x + tileObject->x) ,(int)(tileObject->y - object->height) };
                }
                if (targetPath != "") {
                    if (tileData->objects.count("entryCollider")) {
                        const TileObject* tileObject = &tileData->objects.at("entryCollider")[i];
                        tilemapChanger->SetEntryTrigger({ PIXEL_TO_METERS(cornerPosition.x + tileObject->width / 2 + tileObject->x),PIXEL_TO_METERS(cornerPosition.y + tileObject->height / 2 + tileObject->y) }, { PIXEL_TO_METERS(tileObject->width),PIXEL_TO_METERS(tileObject->height) });
                    }
                } 
                if (tileData->objects.count("exitPosition")) {
                    const TileObject* tileObject = &tileData->objects.at("exitPosition")[i];
                    tilemapChanger->SetExitPosition({ cornerPosition.x + tileObject->x ,cornerPosition.y + tileObject->y });
                }
            }
            else if (type == "npc") {
                auto npc = Pooling::Instance().AcquireObject<NpcCharacter>();

                Vector2 position = { object->x ,object->y };
                npc->SetPosition(position);  

                if (object->properties.count("NpcId"))
                {
                    npc->SetCharacterId((object->properties.at("NpcId").value));
                }else
                    npc->SetCharacterId("character;test");

                if (object->properties.count("DialoguePath"))
                {
					npc->SetDialoguePath(object->properties.at("DialoguePath").value);
                }

                if (object->properties.count("Path"))
                {
                    int pathId = stoi(object->properties.at("Path").value);
                    
                    const MapObject* pathObject = &layer->objects.at(pathId);
                    vector<Vector2> pathData = pathObject->shapes[0].points;

                    for (size_t i = 0; i < pathData.size(); i++)
                    {
                        pathData[i] += {pathObject->x, pathObject->y};
                    }

                    if (object->properties.count("PathMovementType")) {
                        int pathMoveId = stoi(object->properties.at("PathMovementType").value);
                        npc->SetNpcPath(move(pathData), (NpcCharacter::MovementType)pathMoveId);
                    }else
                        npc->SetNpcPath(move(pathData));
                    printf("");
                }
            }
            else if (type == "birdFlock") {
                Vector2 position = { object->x+ object->width / 2 ,object->y + object->width / 2 };
				auto flock = Pooling::Instance().AcquireObject<BirdFlock>();
                flock->Initialize(position, (object->width/2));
                flock->SpawnBirds();
            }
            else if (type == "buttonPuzzle") {

                auto button = Pooling::Instance().AcquireObject<ButtonPuzzleElement>();

                Vector2 position = { object->x ,object->y };
				string puzzleId = object->properties.at("PuzzleId").value;
                bool puzzleValue = object->properties.at("Value").value == "true";

                string targetsText = object->properties.at("Targets").value;
				vector<string> targets;

                targetsText.erase(remove(targetsText.begin(), targetsText.end(), ' '));
                stringstream ss(targetsText);
                string temp;

                while (getline(ss, temp, ','))
                {
                    targets.emplace_back(temp);
                }

                button->Initialize(puzzleId, position, puzzleValue, targets);
            }
            else if (type == "blockingPuzzle") {

                auto blocking = Pooling::Instance().AcquireObject<BlockingPuzzleElement>();

                Vector2 position = { object->x + object->width / 2 * scale ,object->y + +object->height/2 };
                Vector2 size = { PIXEL_TO_METERS(object->width) ,PIXEL_TO_METERS(object->height) };
                string blockingId = object->properties.at("PuzzleId").value;
                bool blockingState = object->properties.at("IsBlocking").value == "true";

                string colorHtml = "";
				SDL_Color color = { 255,255,255,255 };

                if (object->properties.count("Color") != 0) {
                    colorHtml = object->properties.at("Color").value;

                    vector<uint8> rgba;
                    for (int i = 1; i < 9; i += 2) {
                        std::string component = colorHtml.substr(i, 2);
                        int value = std::stoi(component, nullptr, 16);
                        rgba.push_back(value);
                    }
                    SDL_Color color2 = { rgba[1],rgba[2],rgba[3],rgba[0] };
					color = color2;
                }

               
                blocking->Initialize(blockingId, position, size, blockingState, color);
             }
            else if (type == "triggerPuzzle") {

                auto trigger = Pooling::Instance().AcquireObject<TriggerPuzzleElement>();

                Vector2 position = { object->x + object->width / 2 * scale ,object->y + +object->height / 2 };
                float size = PIXEL_TO_METERS(object->width/2);
                string puzzleId = object->properties.at("PuzzleId").value;

                string targetsText = "";
                vector<string> targets;
                if (object->properties.count("Targets") != 0) {
                    object->properties.at("Targets").value;

                    targetsText.erase(remove(targetsText.begin(), targetsText.end(), ' '));
                    stringstream ss(targetsText);
                    string temp;

                    while (getline(ss, temp, ','))
                    {
                        targets.emplace_back(temp);
                    }
                }

				bool callOnEnter = false;
				callOnEnter = object->properties.at("OnEnterCall").value == "true";
				bool callOnExit = false;
                callOnExit = object->properties.at("OnExitCall").value == "true";

                trigger->Initialize(puzzleId, position, size, targets, callOnEnter, callOnExit);
            }
            else if (type == "dialogueActivatorPuzzle") {
                auto trigger = Pooling::Instance().AcquireObject<DialogueActivatorPuzzleElement>();

                Vector2 position = { object->x + object->width / 2 * scale ,object->y + +object->height / 2 };
                Vector2 size = { PIXEL_TO_METERS(object->width / 2), PIXEL_TO_METERS(object->height / 2) };
                string puzzleId = object->properties.at("PuzzleId").value;

                string targetsText = "";
                vector<string> targets;

                if (object->properties.count("Targets") != 0) {
                    object->properties.at("Targets").value;

                    targetsText.erase(remove(targetsText.begin(), targetsText.end(), ' '));
                    stringstream ss(targetsText);
                    string temp;

                    while (getline(ss, temp, ','))
                    {
                        targets.emplace_back(temp);
                    }
                }
				bool enabled = true;
                enabled = object->properties.at("IsEnabled").value == "true";
                string path = "";
                path = object->properties.at("DialoguePath").value;
                int state = 0;
                state = stoi(object->properties.at("DialogueState").value);

                trigger->Initialize(puzzleId, position, size, targets, enabled, path, state);
            }
            else if (type == "item") {
                if (object->properties.count("ItemId")) {
				    auto item = Pooling::Instance().AcquireObject<OverworldItem>();
				    Vector2 position = { object->x + object->width / 2 ,object->y + object->height / 2 };
				    int amount = 1;
				    if (object->properties.count("Amount")) {
                       amount = stoi(object->properties.at("Amount").value);
				    }
				    item->Initialize(ItemList::Instance().ItemByID(object->properties.at("ItemId").value), amount, position);
			    }
            }
            else if (type == "spawnPoint") {
                if (!spawnPointSaved) {
                    spawnPointSaved = true;
                    spawnPoint = { object->x ,object->y };
                }
            }
            else if (type == "entryPoint") {

				entryPoints.emplace(stoi(object->properties.at("Value").value), Vector2{ object->x ,object->y });
            }
        }
    }
}


Vector2 Tilemap::GetSpawnPoint()
{
    return spawnPoint;
}

void Tilemap::SetSpawnPoint(Vector2 _spawnPoint)
{
    spawnPoint = _spawnPoint;
    spawnPointSaved = true;
}


Vector2 Tilemap::GetEntryPoint(int id)
{
    if(entryPoints.count(id) == 0)
		return { 0,0 };
    return entryPoints[id];
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
        vector<Sprite> sprites;
        for (xml_node frameNode : animationNode.children("frame")) {
            TileAnimationFrame data = {
                frameNode.attribute("tileid").as_int(),
                frameNode.attribute("duration").as_int()
            };
            tileData.animation.push_back(move(data));
            SDL_Rect rect = { 0,0,0,0 };
            GetTileRect(tileset, data.tileId, rect);
            sprites.emplace_back(Sprite{ tileset.texture,rect,{0,0},{0,0} });
        }
        AnimationClip clip = AnimationClip{to_string((tileset.firstGid + tileData.id)),true,false,0.1f,move(sprites),nullptr, nullptr};
        animations[tileset.firstGid + tileData.id] = clip;
    }

    if (xml_node objectGroup = tileNode.child("objectgroup")) {
        for (xml_node objNode : objectGroup.children("object")) {
            TileObject obj;
            ParseObject(objNode, obj, baseDir);
            tileData.objects[obj.name].emplace_back(obj);
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
            objLayer.objects[obj.id] = obj;
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
            prop.value = value;

            props[propNode.attribute("name").as_string()] = prop;
        }
    }
}


void Tilemap::UpdateTilemap()
{
    for (auto& anim : animations)
    {
        anim.second.UpdateClip();
    }
}

void Tilemap::Render()
{
    ModuleRender& renderer = *Engine::Instance().m_render;
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

                drawingPos.x = (int)(x * currentMap.tileWidth * scale);
                drawingPos.y = (int)(y * currentMap.tileHeight * scale);

                const TileData& tileData = tileset->tiles[tileId];
                if (tileData.animation.size() == 0) {
                    GetTileRect(*tileset, tileId, rect);
                    painter.RenderTexture(*tileset->texture, drawingPos + position, &rect, { scale,scale });
                }
                else {
                    /// Animate
                    int nonConst_gid = gid;
                    animations[nonConst_gid].RenderClip(drawingPos + position, scale);
                }
            }
        }
    }
}

bool Tilemap::CleanUp()
{
    Engine::Instance().m_render->RemoveFromRenderQueue(*this);

    entryPoints.clear();
    animations.clear();
    onTilemapLoad.UnsubscribeAll();
    spawnPointSaved = false;
    currentMap = TiledMap();
	return true;
}


void Tilemap::GetTileRect(const Tileset& tileset, int tileId, SDL_Rect& rect)
{

	const int tx = (tileId % tileset.columns) * (tileset.tileWidth + tileset.spacing) + tileset.margin;
	const int ty = (tileId / tileset.columns) * (tileset.tileHeight + tileset.spacing) + tileset.margin;
	rect = { tx, ty, tileset.tileWidth, tileset.tileHeight };
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

const TiledMap& Tilemap::GetTilemap() const
{
    return currentMap; 
}

Vector2 Tilemap::GetTilemapSize()
{
    return { currentMap.width * currentMap.tileWidth * scale, currentMap.height * currentMap.tileHeight * scale };
}

