#pragma once
#include "Vector2Int.h"
#include <vector>
#include <string>
#include <pugixml.hpp>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace pugi;
using namespace fs;
using namespace std;


struct SDL_Texture;
struct SDL_Rect;

struct Tileset {
    int firstGid=0;
    int tileCount=0;
    std::string name="";
    Vector2Int tileSize = {0,0};
    int spacing=0;
    int margin=0;
    int columns=0;
    SDL_Texture* texture=nullptr;
    std::map<int, std::vector<SDL_Rect>> animations;
};

struct TileLayer {
    std::string name="";
    Vector2Int layerSize = {0,0};
    std::vector<int> tiles;
    bool visible=false;
    std::map<std::string, std::string> properties;
};

class Tilemap {

    public:
        Tilemap();
        Tilemap(string filename);
        ~Tilemap();

        bool LoadFromXML(string filename);
        void UpdateTilemap();
        void RenderTilemap();

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

        std::vector<Tileset> tilesets;
        std::vector<TileLayer> layers;
        std::vector<TileLayer> objectLayers;

        Tileset* lastTilesetUsed = nullptr;
};