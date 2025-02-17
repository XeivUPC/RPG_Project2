#pragma once
#include "IRendereable.h"
#include "IUpdateable.h"
#include "ITransformable.h"
#include "IInitializable.h"
#include "IPooleable.h"
#include "ICleanable.h"

struct SDL_Texture;
class Entity : public IInitializable, public IRendereable, public IUpdateable, public ICleanable, public ITransformable {


	public:
		Entity();
		~Entity();

		// Inherited via IInitializable
		bool Init() override;
		// Inherited via IInitializable
		bool Start() override;
		// Inherited via IRendereable
		void Render() override;
		// Inherited via IUpdateable
		bool PreUpdate() override;
		// Inherited via IUpdateable
		bool Update() override;
		// Inherited via IUpdateable
		bool PostUpdate() override;
		// Inherited via ICleanable
		bool CleanUp() override;
		// Inherited via ITransformable
		void SetPosition(Vector2 newPosition) override;
		// Inherited via ITransformable
		Vector2 GetPosition() override;
		// Inherited via ITransformable
		void SetAngle(double newAngle) override;
		// Inherited via ITransformable
		void SetScale(float newScale) override;
		// Inherited via ITransformable
		double GetAngle() override;
		// Inherited via ITransformable
		float GetScale() override;

	public:

	private:
	private:

	protected:
	protected:

};