#include "DrawingTools.h"
#include "Camera.h"
#include <SDL2/SDL.h>
#include <sstream>


DrawingTools::DrawingTools(SDL_Renderer* _renderer, Camera& _camera, bool& _cameraMode)
{
    SetRenderer(_renderer);
    SetCameraMode(_cameraMode);
    SetCamera(_camera);
}

DrawingTools::~DrawingTools()
{

}

void DrawingTools::SetRenderer(SDL_Renderer* _renderer)
{
	renderer = _renderer;
}

void DrawingTools::SetCameraMode(bool& _cameraMode)
{
    cameraMode = &_cameraMode;
}

void DrawingTools::SetCamera(Camera& _camera)
{
    camera = &_camera;
}

void DrawingTools::RenderTexture(SDL_Texture& texture, const Vector2& position, const SDL_Rect* section, const Vector2& scale, float angle, const Vector2& pivot, SDL_RendererFlip flipMode, const SDL_Color& color) const
{
    SDL_SetTextureColorMod(&texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(&texture, color.a);

    SDL_FRect dstRect;
    dstRect.x = position.x;
    dstRect.y = position.y;

    if (section == NULL) {
        int widht;
        int height;
        SDL_QueryTexture(&texture, NULL, NULL, &widht, &height);
        dstRect.w = (float)widht;
        dstRect.h = (float)height;
    }
    else {
        dstRect.w = (float)(section->w);
        dstRect.h = (float)(section->h);
    }

    dstRect.w = (dstRect.w * scale.x);
    dstRect.h = (dstRect.h * scale.y);

    if (*cameraMode) {
        dstRect.w = (dstRect.w * camera->zoom);
        dstRect.h = (dstRect.h * camera->zoom);
    }

    SDL_FPoint center = SDL_FPoint{ (dstRect.w * pivot.x), (dstRect.h * pivot.y)};
    
    if (*cameraMode) {
        float screenPivotX = (position.x - camera->rect.x) * camera->zoom;
        float screenPivotY = (position.y - camera->rect.y) * camera->zoom;

        dstRect.x = (screenPivotX - center.x);
        dstRect.y = (screenPivotY - center.y);
    }
    else {
        dstRect.x = position.x - center.x;
        dstRect.y = position.y - center.y;
    }

    SDL_RenderCopyExF(renderer, &texture, section, &dstRect, angle, &center, flipMode);

}

void DrawingTools::RenderBox(const Vector2& position, const Vector2& size, const Vector2& scale, const Vector2& pivot, bool fill, const SDL_Color& color) const
{
    SDL_FRect rect = { position.x, position.y, (size.x * scale.x), (size.y * scale.y)};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);

    SDL_FPoint center = SDL_FPoint{ (rect.w * pivot.x), (rect.h * pivot.y) };


    if (*cameraMode) {
        float screenPivotX = (position.x - camera->rect.x) * camera->zoom;
        float screenPivotY = (position.y - camera->rect.y) * camera->zoom;

        rect.x = (screenPivotX - center.x);
        rect.y = (screenPivotY - center.y);
    }
    else {
        rect.x = position.x - center.x;
        rect.y = position.y - center.y;
    }

    if (fill) {
        SDL_RenderFillRectF(renderer, &rect);
    }
    else {
        SDL_RenderDrawRectF(renderer, &rect);
    }
}

void DrawingTools::RenderCircle(const Vector2& position, float radius, const SDL_Color& color, const int precision) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);
    float angleStep = (float)(2.0f * M_PI / precision);

    for (int i = 0; i < precision; ++i)
    {
        float theta1 = i * angleStep;
        float theta2 = (i + 1) * angleStep;

        float x1 = (position.x + radius * cosf(theta1));
        float y1 = (position.y + radius * sinf(theta1));
        float x2 = (position.x + radius * cosf(theta2));
        float y2 = (position.y + radius * sinf(theta2));


        if (*cameraMode) {
            x1 += camera->rect.x;
            x2 += camera->rect.x;
            y1 += camera->rect.y;
            y2 += camera->rect.y;
        }
        SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
    }
}

void DrawingTools::RenderFillCircle(const Vector2& position, float radius, const SDL_Color& color) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);

    for (int y = -(int)radius; y <= (int)radius; ++y)
    {
        for (int x = -(int)radius; x <= (int)radius; ++x)
        {
            if (x * x + y * y <= radius * radius)
            {
                float xPos = (position.x + x);
                float yPos = (position.y + y);
                if (*cameraMode) {
                    xPos += camera->rect.x;
                    yPos += camera->rect.y;
                }
                SDL_RenderDrawPointF(renderer,xPos , yPos);
            }
        }
    }
}

void DrawingTools::RenderLine(const Vector2& start, const Vector2& end, const SDL_Color& color) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);

    if (*cameraMode) {
        // Transform both points to screen space
        const float screenStartX = (start.x - camera->rect.x) * camera->zoom;
        const float screenStartY = (start.y - camera->rect.y) * camera->zoom;
        const float screenEndX = (end.x - camera->rect.x) * camera->zoom;
        const float screenEndY = (end.y - camera->rect.y) * camera->zoom;

        SDL_RenderDrawLineF(renderer,
            (screenStartX),
            (screenStartY),
            (screenEndX),
            (screenEndY));
    }
    else {
        SDL_RenderDrawLineF(renderer, start.x, start.y, end.x, end.y);
    }
}

SDL_Texture* DrawingTools::RenderText(const string& text, TTF_Font& font, int fontSize, const Vector2& position, const Vector2& textBoxSize, bool returnTexture, const Vector2& scale, int horizonalAlignment, int verticalAlignment, bool wrap, const Vector2& pivot, const SDL_Color& color) const
{
    int current_fontSize = TTF_FontHeight(&font);
    if (current_fontSize != fontSize)
        TTF_SetFontSize(&font, fontSize);
    current_fontSize = fontSize;

    SDL_Surface* surface = nullptr;
    int maxSize = 5000;
    if (wrap) {
        maxSize = (int)textBoxSize.x;
    }
    else {
        //// Get Longest Line
        istringstream stream(text);
        string line;
        size_t maxLength = 0;

        while (getline(stream, line)) {
            if (line.length() > maxLength) {

                maxLength = line.length();
            }
        }

        maxSize = (int)(maxLength * current_fontSize * scale.x);
    }

    TTF_SetFontWrappedAlign(&font, horizonalAlignment);
    surface = TTF_RenderText_Blended_Wrapped(&font, text.c_str(), color, maxSize);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);


    RenderInsideBox(*texture, position, textBoxSize, scale, horizonalAlignment, verticalAlignment, pivot);

    if (returnTexture) {
        return texture;
    }
    SDL_DestroyTexture(texture);
    return nullptr;
}

void DrawingTools::RenderInsideBox(SDL_Texture& texture, const Vector2& position, const Vector2& boxSize, const Vector2& scale, int horizonalAlignment, int verticalAlignment, const Vector2& pivot) const
{
    SDL_Rect textureRect = { (int)(boxSize.x * scale.x),(int)(boxSize.y * scale.y),0,0 };
    SDL_QueryTexture(&texture, NULL, NULL, &textureRect.w, &textureRect.h);

    Vector2 drawinPosition = { 0,0 };
    SDL_FPoint center = SDL_FPoint{ (textureRect.x * pivot.x), (textureRect.y * pivot.y) };

    if (horizonalAlignment == 0)      // Left
        drawinPosition.x = position.x;
    else if (horizonalAlignment == 1) // Center
        drawinPosition.x = (position.x + textureRect.x / 2 - textureRect.w / 2 * scale.x);
    else if (horizonalAlignment == 2) // Right
        drawinPosition.x = (position.x + textureRect.x - textureRect.w * scale.x);

    if (verticalAlignment == 0)      // Top
        drawinPosition.y = position.y;
    else if (verticalAlignment == 1) // Center
        drawinPosition.y = (position.y + textureRect.y / 2 - textureRect.h / 2 * scale.y);
    else if (verticalAlignment == 2) // Bottom
        drawinPosition.y = (position.y + textureRect.y * scale.y - textureRect.h * scale.y);

    drawinPosition.x -= center.x;
    drawinPosition.y -= center.y;

    textureRect.x = 0;
    textureRect.y = 0;

    RenderTexture(texture, drawinPosition, &textureRect, scale, 0);
}


void DrawingTools::SetColor(const SDL_Color& color) const
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
