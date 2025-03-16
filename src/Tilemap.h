#pragma once
#include "Vector2Int.h"
#include "IRendereable.h"
#include <vector>
#include <string>
#include <pugixml.hpp>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace pugi;
using namespace fs;
using namespace std;


struct SDL_Texture;
struct SDL_Rect;


struct TileProperties {
    unordered_map<string, unordered_map<string, string>> properties;
};
struct Tileset {
    int firstGid=0;
    int tileCount=0;
    std::string name="";
    Vector2Int tileSize = {0,0};
    int spacing=0;
    int margin=0;
    int columns=0;
    SDL_Texture* tilesetImage=nullptr;
    unordered_map < int, TileProperties> tileProperties;
    unordered_map<int, vector<SDL_Rect>> animations;
    vector<int> ignoredTiles;
};

struct TileLayer {
    string name="";
    Vector2Int layerSize = {0,0};
    vector<int> tiles;
    bool visible=false;
    unordered_map<string, string> properties;
};

struct ObjectLayer {
    string name = "";
    unordered_map<string, string> properties;
};

class Tilemap : public IRendereable{

    public:
        Tilemap(string filename, float _scale = 1);
        ~Tilemap();

        bool LoadFromXML(string filename, float _scale = 1);
        void UpdateTilemap();
        // Inherited via IRendereable
        void Render() override;

    public:
        float scale = 1;
        Vector2Int position = {0,0};

    private:
        void ParseTileset(xml_node tsNode, const path& basePath);
        void ParseLayer(xml_node layerNode);
        void ParseObjectLayer(xml_node objectGroupNode);
        void GetTileRect(Tileset* tileset, int tileId, SDL_Rect& rect);
        int GetTileRelativeIndex(int x, int y, const TileLayer& layer) const;
        Tileset* GetTileset(int gid);

    private:
        Vector2Int tileSize = {0,0};

        vector<Tileset> tilesets;
        vector<TileLayer> layers;
        vector<ObjectLayer> objectLayers;

        Tileset* lastTilesetUsed = nullptr;

        
};