#include<iostream>
#include "Render.h"

int InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "[ERROR]SDL initialisation failed: " << SDL_GetError() << std::endl;
		return -1;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		std::cout << "[ERROR]IMG initialisation failed: " << SDL_GetError() << std::endl;
		return -1;
	}
	return 0;
}

void QuitSDL()
{
	SDL_Quit();
	IMG_Quit();
}

Window::Window(const char* TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT)
	:window(NULL), renderer(NULL), SW(SCREEN_WIDTH), SH(SCREEN_HEIGHT)
{
	window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SW, SH, SDL_WINDOW_SHOWN);

	if (window == NULL)
		std::cout << "[ERROR]Failed to create window: " << SDL_GetError() << std::endl;
	else
	{
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	}
}

void Window::clearWindow(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint8 rp, gp, bp, ap;
	SDL_GetRenderDrawColor(renderer, &rp, &gp, &bp, &ap);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, rp, gp, bp, ap);
}


SDL_Texture* Window::loadTexture(const char* path)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, path);

	if(texture == NULL)
		std::cout << "[ERROR]Failed to load texture" << SDL_GetError() << std::endl;

	return texture;
		
}

void Window::renderTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst)
{
	SDL_RenderCopy(renderer, texture, src, dst);
}

void Window::displayWindow()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* Window::getRenderer()
{
	return renderer;
}

void Window::setRenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::drawLine(int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint8 rp, gp, bp, ap;
	SDL_GetRenderDrawColor(renderer, &rp, &gp, &bp, &ap);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
	SDL_SetRenderDrawColor(renderer, rp, gp, bp, ap);
}

void Window::drawRect(int x0, int y0, int x1, int y1, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool fill)
{
	Uint8 rp, gp, bp, ap;
	SDL_GetRenderDrawColor(renderer, &rp, &gp, &bp, &ap);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_Rect rec = { x0, y0, x1, y1 };
	if(fill)
		SDL_RenderFillRect(renderer, &rec);
	else
		SDL_RenderDrawRect(renderer, &rec);
	SDL_SetRenderDrawColor(renderer, rp, gp, bp, ap);
}

void Window::drawPoint(int x0, int y0, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint8 rp, gp, bp, ap;
	SDL_GetRenderDrawColor(renderer, &rp, &gp, &bp, &ap);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoint(renderer, x0, y0);
	SDL_SetRenderDrawColor(renderer, rp, gp, bp, ap);
}


Window::~Window()
{
	SDL_DestroyWindow(window);
}