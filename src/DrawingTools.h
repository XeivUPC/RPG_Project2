#pragma once
#include "Vector2Int.h"
#include "Vector2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Camera;

class DrawingTools {
public:

	DrawingTools(SDL_Renderer* _renderer, Camera& _camera, bool& _cameraMode);
	~DrawingTools();

	void SetRenderer(SDL_Renderer* _renderer);
	void SetCameraMode(bool& _cameraMode);
	void SetCamera(Camera& _camera);

	void RenderTexture(SDL_Texture& texture, const Vector2& position, const SDL_Rect* section = NULL, const Vector2& scale = { 1,1 }, float angle = 0, const Vector2& pivot = { 0,0 }, const SDL_RendererFlip flipMode = SDL_FLIP_NONE, const SDL_Color& color = { 255,255,255,255 }) const;

	void RenderBox(const Vector2& position, const Vector2& size, const Vector2& scale = { 1,1 }, const Vector2& pivot = {0,0}, bool fill = true, const SDL_Color& color = { 255,255,255,255 }) const;

	void RenderCircle(const Vector2& position, float radius, const SDL_Color& color = { 255,255,255,255 }, const int precision = 100) const;

	void RenderFillCircle(const Vector2& position, float radius, const SDL_Color& color) const;

	void RenderLine(const Vector2& start, const Vector2& end, const SDL_Color& color = { 255,255,255,255 }) const;

	SDL_Texture* RenderText(const string& text, TTF_Font& font, int fontSize, const Vector2& position, const Vector2& textBoxSize, bool returnTexture = true, const Vector2& scale = {1,1}, int horizonalAlignment = 0, int verticalAlignment = 0, bool wrap = true, const Vector2& pivot = { 0,0 }, const SDL_Color& color = { 255,255,255,255 }) const;

	void RenderInsideBox(SDL_Texture& texture, const Vector2& position, const Vector2& boxSize, const Vector2& scale = { 1,1 }, int horizonalAlignment = 0, int verticalAlignment = 0, const Vector2& pivot = { 0,0 }) const;

private:
	void SetColor(const SDL_Color& color) const;
private:
	SDL_Renderer* renderer = nullptr;
	Camera* camera = nullptr;
	bool* cameraMode = nullptr;
};