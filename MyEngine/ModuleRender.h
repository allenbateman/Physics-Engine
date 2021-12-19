#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

enum RenderFlip { None, Horizontal, Vertical };

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, bool useCamera = true, float multiplier = 1.0f, float speed = 1.0f, double angle = 0, SDL_Point pivot = { INT_MAX ,INT_MAX });
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	bool DrawCircle(fPoint center, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);

public:
	SDL_Renderer* renderer;
	SDL_Texture* cameraRender;
	SDL_Rect camera;
};

