#include<SDL.h>
#include<SDL_image.h>

int InitSDL();
void QuitSDL();

class Window
{
public:
	const int SW, SH;
	Window(const char* TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT);
	~Window();

	void clearWindow(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 1);
	SDL_Texture* loadTexture(const char*);
	void renderTexture(SDL_Texture*, SDL_Rect* src = NULL, SDL_Rect* dst = NULL);
	void displayWindow();

	SDL_Renderer* getRenderer();
	void setRenderColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void drawLine(int x0, int y0, int x1, int y1, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 1);
	void drawRect(int x0, int y0, int x1, int y1, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 1, bool fill = false);
	void drawPoint(int x0, int y0, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 1);


private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};