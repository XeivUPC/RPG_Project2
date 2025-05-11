#include "JsonAnimator.h"
#include "Engine.h"
#include "ModuleAssetDatabase.h"
#include "LOG.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

JsonAnimator::JsonAnimator() : Animator()
{
}

JsonAnimator::JsonAnimator(string jsonPath, float speed) : Animator()
{
    AddJsonAnimationClip(jsonPath, speed);
}

JsonAnimator::JsonAnimator(vector<AnimationClip> Animations, int current) : Animator(Animations, current)
{
}

JsonAnimator::~JsonAnimator()
{
}

void JsonAnimator::AddJsonAnimationClip(string jsonPath, float speed)
{

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        LOG("Error opening JSON file -> %s\n", jsonPath.c_str());
        return;
    }

    json data = json::parse(file);

    SDL_Texture* texture = nullptr;
	bool textureFound = false;
    for (const auto& layer : data["meta"]["layers"]) {
        if (layer["name"] == "TextureId") {
            string textureId = layer["data"];

            texture = Engine::Instance().m_assetsDB->GetTexture(textureId);
            if(texture!=nullptr)
                textureFound = true;
            break;
        }
    }
    if (!textureFound) {
        LOG("TextureId not found in JsonAnimator data -> %s\n", jsonPath.c_str());
        return;
    }

    vector<Sprite> sprites;
	string key = "";
    for (auto& [key, value] : data["frames"].items()) {
        SDL_Rect rect = { 0,0,0,0 };
        //frame.name = key;
        rect.x = value["frame"]["x"];
        rect.y = value["frame"]["y"];
        rect.w = value["frame"]["w"];
        rect.h = value["frame"]["h"];
        //frame.duration = value["duration"];
       
        sprites.emplace_back(Sprite{ texture,rect,{0,0},{0,0} });

    }
	AddAnimationClip(AnimationClip(key,true,false, speed,sprites,nullptr,nullptr));
}
