#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "IRendereable.h"
#include "DrawingTools.h"
#include "Log.h"
#include "Globals.h"

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

	if (VSYNC) {
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

		SDL_RenderSetLogicalSize(renderer, LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT);
		camera.viewport = { LOGIC_SCREEN_WIDTH, LOGIC_SCREEN_HEIGHT };
	}
	return ret;
}

bool ModuleRender::Start()
{
	LOG("render start");

	drawingTools = new DrawingTools(renderer, camera, cameraMode);
	

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

bool ModuleRender::CleanUp()
{
	LOG("Destroying SDL render");
	renderQueue.clear();
	delete drawingTools;

	SDL_DestroyRenderer(renderer);
	return true;
}

void ModuleRender::RemoveFomRenderQueue(IRendereable& rendereableObj)
{
	if (renderQueue.size() == 0)
		return;
	renderQueue.erase(
		remove(renderQueue.begin(), renderQueue.end(), &rendereableObj),
		renderQueue.end()
	);
	renderQueueDirty = true;
}

void ModuleRender::AddToRenderQueue(IRendereable& rendereableObj)
{
	renderQueue.emplace_back(&rendereableObj);
	renderQueueDirty = true;
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
	camera.target.x += moveAmount.x;
	camera.target.y += moveAmount.y;
}

void ModuleRender::SetCameraPosition(const Vector2Int& position)
{
	camera.target.x = (float)position.x;
	camera.target.y = (float)position.y;
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

