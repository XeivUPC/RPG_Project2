#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleUpdater.h"
#include "IRendereable.h"
#include "ITransformable.h"
#include "DrawingTools.h"
#include "Log.h"
#include "Globals.h"
#include "UserPrefs.h"

#include <algorithm>

ModuleRender::ModuleRender(bool start_active) : Module(start_active)
{

}

ModuleRender::~ModuleRender()
{
	
}


const DrawingTools& ModuleRender::painter()
{
	return *drawingTools;
}

bool ModuleRender::Init()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	SetVSync(UserPrefs::Instance().GetBool("screen_vsync", VSYNC));
	if (vSync) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}


	SDL_Window* window = Engine::Instance().m_window->window;

	renderer = SDL_CreateRenderer(window, -1, flags);

	

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		SDL_DisplayMode displayMode;
		if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
			LOG("No se pudo obtener el modo de pantalla: %s", SDL_GetError());

		//SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
		SDL_RenderSetLogicalSize(renderer, LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT);
		camera.viewport = { LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT };

		
	}
	return ret;
}

bool ModuleRender::Start()
{
	LOG("render start");

	drawingTools = new DrawingTools(renderer, camera, cameraMode);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
	Engine::Instance().m_updater->AddToUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

	return true;
}

bool ModuleRender::CleanUp()
{
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::PRE_UPDATE);
	Engine::Instance().m_updater->RemoveFromUpdateQueue(*this, ModuleUpdater::UpdateMode::POST_UPDATE);

	LOG("Destroying SDL render");
	renderQueue.clear();
	transformMap.clear();
	delete drawingTools;

	SDL_DestroyRenderer(renderer);
	return true;
}

bool ModuleRender::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool ModuleRender::Update()
{
	return true;
}

bool ModuleRender::PostUpdate()
{
	camera.rect = camera.GetRect();
	SortRenderTasks();
	RenderAll();

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

void ModuleRender::RemoveFomRenderQueue(IRendereable& rendereableObj)
{
	if (renderQueue.size() == 0)
		return;

	auto it = transformMap.find(&rendereableObj);
	if (it != transformMap.end()) {
		transformMap.erase(it);
	}

	renderQueue.erase(
		remove(renderQueue.begin(), renderQueue.end(), &rendereableObj),
		renderQueue.end()
	);
	renderQueueDirty = true;
}

void ModuleRender::SortRenderQueueLayerByPosition(int targetLayer) {
	SortRenderTasks();

	auto begin = std::lower_bound(renderQueue.begin(), renderQueue.end(), targetLayer,
		[](IRendereable* a, int layer) { return a->renderLayer < layer; });
	auto end = std::upper_bound(renderQueue.begin(), renderQueue.end(), targetLayer,
		[](int layer, IRendereable* a) { return layer < a->renderLayer; });

	auto mid = std::stable_partition(begin, end, [this](IRendereable* a) {
		return transformMap.find(a) != transformMap.end();
		});

	std::sort(begin, mid, [this](IRendereable* a, IRendereable* b) {
		return transformMap[a]->GetPosition().y < transformMap[b]->GetPosition().y;
		});
}
void ModuleRender::AddToRenderQueue(IRendereable& rendereableObj)
{
	renderQueue.emplace_back(&rendereableObj);
	renderQueueDirty = true;
}

void ModuleRender::AddToRenderQueue(IRendereable& rendereableObj, ITransformable& transformableObj)
{
	AddToRenderQueue(rendereableObj);
	transformMap[&rendereableObj] = &transformableObj;
}

void ModuleRender::SortRenderTasks()
{
	if (!renderQueueDirty)
		return;
	sort(renderQueue.begin(), renderQueue.end(),
		[](IRendereable* a, IRendereable* b) {
			return a->renderLayer < b->renderLayer;
		});
}

void ModuleRender::RenderAll()
{
	for (const auto& renderTask : renderQueue)
	{
		if(renderTask->isVisible)
			renderTask->Render();
	}
}

bool ModuleRender::IsCameraModeActive() const
{
	return cameraMode;
}

void ModuleRender::SetCameraMode(bool isActive)
{
	cameraMode = isActive;
}

const Camera& ModuleRender::GetCamera() const
{
	return camera;
}

void ModuleRender::MoveCamera(const Vector2& moveAmount)
{
	camera.position.x += moveAmount.x;
	camera.position.y += moveAmount.y;
}

void ModuleRender::SetCameraPosition(const Vector2Int& position)
{
	camera.position.x = (float)position.x;
	camera.position.y = (float)position.y;
}

void ModuleRender::SetCameraOffset(const Vector2& offset)
{
	camera.offset = { offset.x, offset.y};
}

void ModuleRender::SetCameraZoom(float zoom)
{
	camera.zoom = zoom;
}

bool ModuleRender::IsRectCameraVisible(const SDL_Rect& rect) const
{
	SDL_Rect cameraRect = camera.GetRect();
	return SDL_HasIntersection(&rect, &cameraRect);
}

bool ModuleRender::IsCircleCameraVisible(const Vector2& center, float radius) const
{
	SDL_Rect cameraRect = camera.GetRect();

	// Circle's bounding box
	SDL_Rect circleBounds;
	circleBounds.x = (int)(center.x - radius);
	circleBounds.y = (int)(center.y - radius);
	circleBounds.w = (int)(radius * 2);
	circleBounds.h = (int)(radius * 2);

	// Check if the bounding box of the circle intersects with the camera
	if (!SDL_HasIntersection(&circleBounds, &cameraRect))
		return false;

	// If the bounding box intersects, do a more precise check
	Vector2 closestPoint;
	closestPoint.x = std::clamp(center.x, (float)cameraRect.x, (float)(cameraRect.x + cameraRect.w));
	closestPoint.y = std::clamp(center.y, (float)cameraRect.y, (float)(cameraRect.y + cameraRect.h));

	float distanceSquared = (center.x - closestPoint.x) * (center.x - closestPoint.x) +
		(center.y - closestPoint.y) * (center.y - closestPoint.y);

	return distanceSquared <= (radius * radius);
}

bool ModuleRender::IsPointCameraVisible(const Vector2& point) const
{
	SDL_Rect cameraRect = camera.GetRect();
	SDL_Point pointSDL = { (int)point.x, (int)point.y };
	return SDL_PointInRect(&pointSDL, &cameraRect);
}

bool ModuleRender::IsLineCameraVisible(const Vector2& point1, const Vector2& point2) const
{
	SDL_Rect cameraRect = camera.GetRect();
	int x1 = (int)point1.x;
	int x2 = (int)point2.x;
	int y1 = (int)point1.y;
	int y2 = (int)point2.y;
	return SDL_IntersectRectAndLine(&cameraRect, &x1, &y1, &x2, &y2);
}

void ModuleRender::SetVSync(bool _vSync)
{
	vSync = _vSync;
	SDL_RenderSetVSync(renderer, vSync);
	UserPrefs::Instance().SaveBool("screen_vsync", vSync);
}

bool ModuleRender::IsVSync()
{
	return vSync;
}

