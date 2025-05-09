#pragma once
#include "Vector2Int.h"
#include "IRendereable.h"
#include "ITransformable.h"
#include "ICleanable.h"
#include "AnimationClip.h"
#include "SystemEvent.h"

#include <pugixml.hpp>
#include <variant>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace fs;
using namespace pugi;
using namespace std;


struct SDL_Texture;
struct SDL_Rect;


struct Property {
    string value;
};

struct TileAnimationFrame {
    int tileId;
    int duration;
};


struct ObjectShape {
    enum class Type { Rectangle, Ellipse, Polygon, Polyline };
    Type type;
    std::vector<Vector2> points;
};

struct TileObject {
    std::string name;
    std::string type;
    float x, y;
    float width, height;
    float rotation;
    std::vector<ObjectShape> shapes;
    std::unordered_map<std::string, Property> properties;
};

struct TileData {
    int id;
    std::unordered_map<std::string, Property> properties;
    std::vector<TileAnimationFrame> animation;
    std::unordered_map<std::string,vector<TileObject>> objects;
    std::string textureId;
};

struct Tileset {
    std::string name;
    int firstGid;
    int tileCount;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int columns;
    std::string textureId;
    SDL_Texture* texture;
    std::unordered_map<int, TileData> tiles;
};

struct LayerTile {
    int gid;
};

struct TileLayer {
    std::string name;
    int width;
    int height;
    vector<LayerTile> data;
    float opacity;
    bool visible;
    std::unordered_map<std::string, Property> properties;
};

struct MapObject {
    int id;
    int gid;
    std::string name;
    std::string type;
    float x, y;
    float width, height;
    float rotation;
    bool visible;
    std::vector<ObjectShape> shapes;
    std::unordered_map<std::string, Property> properties;
};

struct ObjectGroupLayer {
    std::string name;
    std::unordered_map<int, MapObject> objects;
    float opacity;
    bool visible;
    std::unordered_map<std::string, Property> properties;
};

struct TiledMap {
    string path;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<Tileset> tilesets;
    std::vector<TileLayer> tileLayers;
    std::vector<ObjectGroupLayer> objectLayers;
    std::unordered_map<std::string, Property> properties;
};

class Tilemap : public IRendereable, public ITransformable, public ICleanable {
public:
    Tilemap(const fs::path& tmxPath, float _scale = 1);
    ~Tilemap();

    void LoadMapFromXML(const fs::path& tmxPath);
    void LoadMapFromXML(const fs::path& tmxPath, float _scale);

    void CreateObjects();

    void UpdateTilemap();

    // Inherited via IRendereable
    void Render() override;
    // Inherited via ICleanable
    bool CleanUp() override;
    // Inherited via ITransformable
    void SetPosition(Vector2 newPosition) override;
    // Inherited via ITransformable
    void SetAngle(double newAngle) override;
    // Inherited via ITransformable
    void SetScale(float newScale) override;
    // Inherited via ITransformable
    void SetAnchor(Vector2 newAnchor) override;
    // Inherited via ITransformable
    Vector2 GetPosition() override;
    // Inherited via ITransformable
    double GetAngle() override;
    // Inherited via ITransformable
    float GetScale() override;
    // Inherited via ITransformable
    Vector2 GetAnchor() override;

    const TiledMap& GetTilemap() const;


    Vector2 GetTilemapSize();
    Vector2 GetSpawnPoint();
    void SetSpawnPoint(Vector2 _spawnPoint);

    Vector2 GetEntryPoint(int id);

public:
    SystemEvent<> onTilemapLoad;

private:
    void ParseTileset(const pugi::xml_node& tsNode, const fs::path& baseDir);
    void ParseLayer(const pugi::xml_node& layerNode, const fs::path& baseDir);
    void ParseTileData(pugi::xml_node& tileNode, TileData& tileData, const Tileset& tileset, const fs::path& baseDir);
    void ParseObject(const pugi::xml_node& objNode, MapObject& object, const fs::path& baseDir);
    void ParseObject(const pugi::xml_node& objNode, TileObject& object, const fs::path& baseDir);
    void ParseProperties(const pugi::xml_node& node, std::unordered_map<std::string, Property>& props);


    void GetTileRect(const Tileset& tileset, int tileId, SDL_Rect& rect);
    int GetTileRelativeIndex(int x, int y, const TileLayer& layer) const;
    Tileset* GetTileset(int gid);

    TiledMap currentMap;
    fs::path currentMapPath;

    Tileset* lastTilesetUsed;

    unordered_map<int, AnimationClip> animations;

    bool spawnPointSaved = false;
    Vector2 spawnPoint = { 0,0 };
	unordered_map<int, Vector2> entryPoints;


};