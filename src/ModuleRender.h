#pragma once
#include "Module.h" 
#include "Vector2.h" 
#include "Vector2Int.h" 
#include "Camera.h" 
#include <vector>
#include <unordered_map>

class IRendereable;
class ITransformable;
class DrawingTools;

///// Layers
/*
	0  - DEFAULT

	1  - Bottom

	2  - World2
	3  - World3
	4  - World4
	5  - World5

	6  - Canvas1
	7  - Canvas2
	8  - Canvas3
	9  - Canvas4

	10 - Top
*/
////


class ModuleRender : public Module {
	friend class Engine;

	public:
		

		ModuleRender(bool start_active = true);
		~ModuleRender();

		void SetVSync(bool _vSync);
		bool IsVSync();

		const DrawingTools& painter();

		void AddToRenderQueue(IRendereable& rendereableObj);
		void AddToRenderQueue(IRendereable& rendereableObj, ITransformable& transformableObj);
		void RemoveFomRenderQueue(IRendereable& rendereableObj);
		void SortRenderQueueLayerByPosition(int targetLayer);

		bool IsCameraModeActive() const;
		void SetCameraMode(bool isActive);

		const Camera& GetCamera() const;
		void MoveCamera(const Vector2& moveAmount);
		void SetCameraPosition(const Vector2Int& position);
		void SetCameraOffset(const Vector2& offset);
		void SetCameraZoom(float zoom);

		bool IsRectCameraVisible(const SDL_Rect& rect) const;
		bool IsCircleCameraVisible(const Vector2& center, float radius) const;
		bool IsPointCameraVisible(const Vector2& point) const;
		bool IsLineCameraVisible(const Vector2& point1, const Vector2& point2) const;
	public:
		SDL_Renderer* renderer = nullptr;

	private:
		// Inherited via IInitializable
		bool Init() override;
		// Inherited via IInitializable
		bool Start() override;
		// Inherited via ICleanable
		bool CleanUp() override;
		// Inherited via IUpdateable
		bool PreUpdate() override;
		// Inherited via IUpdateable
		bool Update() override;
		// Inherited via IUpdateable
		bool PostUpdate() override;

		void SortRenderTasks();
		void RenderAll();

	private:
		SDL_Color background = {0,0,0,255};
		Camera camera;

		bool vSync = false;

		vector<IRendereable*> renderQueue;
		unordered_map<IRendereable*, ITransformable*> transformMap;
		bool renderQueueDirty = false;

		bool cameraMode = true;

		DrawingTools* drawingTools = nullptr;
	
};