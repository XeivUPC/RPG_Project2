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

void DrawingTools::RenderTexture(SDL_Texture& texture, const Vector2Int& position, const SDL_Rect* section, const Vector2& scale, float angle, const Vector2& pivot, SDL_RendererFlip flipMode, const SDL_Color& color) const
{
    SDL_SetTextureColorMod(&texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(&texture, color.a);

    SDL_Rect dstRect;
    dstRect.x = (int)position.x;
    dstRect.y = (int)position.y;

    if (section == NULL) {
        SDL_QueryTexture(&texture, NULL, NULL, &dstRect.w, &dstRect.h);
    }
    else {
        dstRect.w = (int)(section->w);
        dstRect.h = (int)(section->h);
    }

    dstRect.w = (int)(dstRect.w * scale.x);
    dstRect.h = (int)(dstRect.h * scale.y);

    if (*cameraMode) {
        dstRect.w = static_cast<int>(dstRect.w * camera->zoom);
        dstRect.h = static_cast<int>(dstRect.h * camera->zoom);
    }

    SDL_Point center = SDL_Point{ (int)(dstRect.w * pivot.x), (int)(dstRect.h * pivot.y)};
    dstRect.x -= center.x;
    dstRect.y -= center.y;    
    
    if (*cameraMode) {
        float screenPivotX = (position.x - camera->rect.x) * camera->zoom;
        float screenPivotY = (position.y - camera->rect.y) * camera->zoom;

        dstRect.x = static_cast<int>(screenPivotX - center.x);
        dstRect.y = static_cast<int>(screenPivotY - center.y);
    }
    else {
        dstRect.x = position.x - center.x;
        dstRect.y = position.y - center.y;
    }

    SDL_RenderCopyEx(renderer, &texture, section, &dstRect, angle, &center, flipMode);

}

void DrawingTools::RenderBox(const Vector2Int& position, const Vector2Int& size, const Vector2& scale, const Vector2& pivot, bool fill, const SDL_Color& color) const
{
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)(size.x * scale.x), (int)(size.y * scale.y)};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);

    SDL_Point center = SDL_Point{ (int)(rect.w * pivot.x), (int)(rect.h * pivot.y) };
    rect.x -= center.x;
    rect.y -= center.y;

    if (*cameraMode) {
        rect.x += camera->rect.x;
        rect.y += camera->rect.y;
    }

    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    }
    else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void DrawingTools::RenderCircle(const Vector2Int& position, float radius, const SDL_Color& color, const int precision) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);
    float angleStep = (float)(2.0f * M_PI / precision);

    for (int i = 0; i < precision; ++i)
    {
        float theta1 = i * angleStep;
        float theta2 = (i + 1) * angleStep;

        int x1 = (int)(position.x + radius * cosf(theta1));
        int y1 = (int)(position.y + radius * sinf(theta1));
        int x2 = (int)(position.x + radius * cosf(theta2));
        int y2 = (int)(position.y + radius * sinf(theta2));


        if (*cameraMode) {
            x1 += camera->rect.x;
            x2 += camera->rect.x;
            y1 += camera->rect.y;
            y2 += camera->rect.y;
        }
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

void DrawingTools::RenderFillCircle(const Vector2Int& position, float radius, const SDL_Color& color) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);

    for (int y = -(int)radius; y <= (int)radius; ++y)
    {
        for (int x = -(int)radius; x <= (int)radius; ++x)
        {
            if (x * x + y * y <= radius * radius)
            {
                int xPos = (int)(position.x + x);
                int yPos = (int)(position.y + y);
                if (*cameraMode) {
                    xPos += camera->rect.x;
                    yPos += camera->rect.y;
                }
                SDL_RenderDrawPoint(renderer,xPos , yPos);
            }
        }
    }
}

void DrawingTools::RenderLine(const Vector2Int& start, const Vector2Int& end, const SDL_Color& color) const
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SetColor(color);
    if (*cameraMode) {
        SDL_RenderDrawLine(renderer, start.x + camera->rect.x, start.y + camera->rect.y, end.x + camera->rect.x, end.y + camera->rect.y);
    }else
        SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    
}

SDL_Texture* DrawingTools::RenderText(const string& text, TTF_Font& font, int fontSize, const Vector2Int& position, const Vector2Int& textBoxSize, bool returnTexture, const Vector2& scale, int horizonalAlignment, int verticalAlignment, bool wrap, const Vector2& pivot, const SDL_Color& color) const
{
    int current_fontSize = TTF_FontHeight(&font);
    if (current_fontSize != fontSize)
        TTF_SetFontSize(&font, fontSize);
    current_fontSize = fontSize;

    SDL_Surface* surface = nullptr;
    int maxSize = 5000;
    if (wrap) {
        maxSize = textBoxSize.x;
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

void DrawingTools::RenderInsideBox(SDL_Texture& texture, const Vector2Int& position, const Vector2Int& boxSize, const Vector2& scale, int horizonalAlignment, int verticalAlignment, const Vector2& pivot) const
{
    SDL_Rect textureRect = { (int)(boxSize.x * scale.x),(int)(boxSize.y * scale.y),0,0 };
    SDL_QueryTexture(&texture, NULL, NULL, &textureRect.w, &textureRect.h);

    Vector2Int drawinPosition = { 0,0 };
    SDL_Point center = SDL_Point{ (int)(textureRect.x * pivot.x), (int)(textureRect.y * pivot.y) };

    if (horizonalAlignment == 0)      // Left
        drawinPosition.x = position.x;
    else if (horizonalAlignment == 1) // Center
        drawinPosition.x = (int)(position.x + textureRect.x / 2 - textureRect.w / 2 * scale.x);
    else if (horizonalAlignment == 2) // Right
        drawinPosition.x = (int)(position.x + textureRect.x - textureRect.w * scale.x);

    if (verticalAlignment == 0)      // Top
        drawinPosition.y = position.y;
    else if (verticalAlignment == 1) // Center
        drawinPosition.y = (int)(position.y + textureRect.y / 2 - textureRect.h / 2 * scale.y);
    else if (verticalAlignment == 2) // Bottom
        drawinPosition.y = (int)(position.y + textureRect.y * scale.y - textureRect.h * scale.y);

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
