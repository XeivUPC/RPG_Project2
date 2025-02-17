#pragma once
#include "Module.h" 
#include "Vector2.h" 
#include "Vector2Int.h" 
#include "Camera.h" 
#include <vector>

class IRendereable;
class DrawingTools;




class ModuleRender : public Module {
	friend class Engine;

	public:
		

		ModuleRender(bool start_active = true);
		~ModuleRender();

		const DrawingTools& painter();

		void AddToRenderQueue(IRendereable& rendereableObj);
		void RemoveFomRenderQueue(IRendereable& rendereableObj);

		bool IsCameraModeActive() const;
		void SetCameraMode(bool isActive);

		const Camera& GetCamera() const;
		void MoveCamera(const Vector2& moveAmount);
		void SetCameraPosition(const Vector2Int& position);
		void SetCameraOffset(const Vector2& offset);
		void SetCameraZoom(float zoom);

		bool IsRectCameraVisible(const SDL_Rect& rect) const;
		bool IsPointCameraVisible(const Vector2& point) const;
		bool IsLineCameraVisible(const Vector2& point1, const Vector2& point2) const;
	public:
		SDL_Renderer* renderer = nullptr;

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
		void SortRenderTasks();
		void RenderAll();

	private:
		SDL_Color background = {0,0,0,255};
		Camera camera;

		vector<IRendereable*> renderQueue;
		bool renderQueueDirty = false;

		bool cameraMode = false;

		DrawingTools* drawingTools = nullptr;
	
};