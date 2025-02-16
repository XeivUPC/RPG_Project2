#include "Module.h"

Module::Module(bool start_active)
{
	isActive = start_active;
}

Module::~Module()
{
}

bool Module::Init()
{
	return true;
}

bool Module::Start()
{
	return true;
}

// Inherited via IUpdateable
bool Module::PreUpdate() {
	return true;
}

// Inherited via IUpdateable
bool Module::Update() {
	return true;
}

// Inherited via IUpdateable
bool Module::PostUpdate() {
	return true;
}

bool Module::CleanUp()
{
	return true;
}

void Module::Activate()
{
	isActive = true;
	Start();
}

void Module::Desactivate()
{
	isActive = false;
	CleanUp();
}

bool Module::IsActive()
{
	return isActive;
}

