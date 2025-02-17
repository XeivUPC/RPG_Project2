#include "ModuleScene.h"

ModuleScene::ModuleScene(bool start_active) : Module(start_active)
{
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
    return true;
}

bool ModuleScene::Start()
{
    return true;
}

bool ModuleScene::PreUpdate()
{
    return true;
}

bool ModuleScene::Update()
{
    return true;
}

bool ModuleScene::PostUpdate()
{
    return true;
}

bool ModuleScene::CleanUp()
{
    return true;
}
