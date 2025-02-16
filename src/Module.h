#pragma once
#include "IUpdateable.h"
#include "ICleanable.h"
#include "IInitializable.h"
#include "IActivable.h"

#pragma warning(push)
#pragma warning(disable : 26819)
#include "SDL2/SDL.h"
#pragma warning(pop)

using namespace std;

class Module : public IInitializable, public IUpdateable, public IActivable, public ICleanable {
	friend class Engine;

	public:
		Module(bool start_active = true);
		~Module();
	public:
		// Inherited via IActivable
		void Activate() override;
		// Inherited via IActivable
		void Desactivate() override;
		// Inherited via IActivable
		bool IsActive() override;
	private:
		// Inherited via IInitializable
		bool Init() override;
		// Inherited via IInitializable
		bool Start() override;
		// Inherited via IUpdateable
		bool PreUpdate() override;
		// Inherited via IUpdateable
		bool Update() override;
		// Inherited via IUpdateable
		bool PostUpdate() override;
		// Inherited via ICleanable
		bool CleanUp() override;
	
	private:
		bool isEnabled = true;

};