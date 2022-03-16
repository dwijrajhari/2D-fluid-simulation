#include "Render.h"
#include "fluid.h"
#include<iostream>

const int SW = PS * (sizex);
const int SH = PS * (sizey);

int WinMain(int argc, char* args[])
{
	field* force = (field*)calloc(1, sizeof(field));
	field* f1 = (field*)calloc(1, sizeof(field));
	field* f0 = (field*)calloc(1, sizeof(field));
	field* trace = (field*)calloc(1, sizeof(field));

	scalar* dye0 = (scalar*)calloc(1, sizeof(field));
	scalar* dye1 = (scalar*)calloc(1, sizeof(field));
	scalar* dye2 = (scalar*)calloc(1, sizeof(field));


	//initField(f1);

	if (InitSDL() == 0)
	{
		Window game("FluidSim", SW, SH);

		//Event Loop
		double time0 = 0, time1 = 0.1, delta = 0.1;;
		bool IS_RUNNING = true;
		bool IS_MOUSE_PRESSED = false;
		bool SHOW_VELOCITY_FIELD = false;

		SDL_Event event;
		int forceRadius = 6;
		int forceMultiplier = 500;
		scalar* currentDye = dye0;
		while (IS_RUNNING)
		{
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)
					IS_RUNNING = false;
				if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
					SHOW_VELOCITY_FIELD = SHOW_VELOCITY_FIELD ? false : true;
				if (event.type == SDL_MOUSEBUTTONDOWN)
				{
					IS_MOUSE_PRESSED = true;
					if (currentDye == dye0)
						currentDye = dye1;
					else if (currentDye == dye1)
						currentDye = dye2;
					else if (currentDye == dye2)
						currentDye = dye0;

				}
				if (event.type == SDL_MOUSEBUTTONUP)
					IS_MOUSE_PRESSED = false;

				if (event.type == SDL_MOUSEMOTION && IS_MOUSE_PRESSED)
				{
					int mx = event.motion.x / PS + 1, my = event.motion.y / PS + 1;


					
					int y0 = std::max(my - forceRadius, 1), y1 = std::min(my + forceRadius, sizey - 1);
					int x0 = std::max(mx - forceRadius, 1), x1 = std::min(mx + forceRadius, sizex - 1);
					for (int y = y0; y <= y1; y++)
					{
						for (int x = x0; x <= x1; x++)
						{
							int f = forceMultiplier / (1 + abs(my - y) * abs(mx - x));
							force->vy[y][x] += f * (double)event.motion.yrel;
							force->vx[y][x] += f * (double)event.motion.xrel;

							dye0->val[y][x] = 0;
							dye1->val[y][x] = 0;
							dye2->val[y][x] = 0;

							currentDye->val[y][x] = 255;


						}
					}
				}

			}
			delta = (time1 - time0) * 0.001;
			traceback(trace, f1, delta);

			addForce(f0, f1, force, delta);
			memset(force, 0, sizeof(field));

			//diffuseField(f0, 0.001, 20);
			bnd(f0);
			projectField(f0);
			bnd(f0);
			advectField(trace, f0);
			bnd(f0);
			projectField(f0);
			bnd(f0);

			advectScalar(trace, dye0);
			//diffuseScalar(dye0, diff, 20);

			advectScalar(trace, dye1);
			//diffuseScalar(dye1, diff, 20);

			advectScalar(trace, dye2);
			//diffuseScalar(dye2, diff, 20);

			swapFields(f0, f1);

			game.clearWindow(0x96, 0x4b, 0x00);

			Uint8 r, g, b;
			for (int y = 0; y < sizey; y++)
			{
				for (int x = 0; x < sizex; x++)
				{
					r = std::min((Uint8)dye0->val[y][x], (Uint8)255);
					g = std::min((Uint8)dye1->val[y][x], (Uint8)255);
					b = std::min((Uint8)dye2->val[y][x], (Uint8)255);
			
					r = r > 63 ? 255 : r * 4;
					g = g > 63 ? 255 : g * 4;
					b = b > 63 ? 255 : b * 4;
					game.drawRect((x) * PS, (y) * PS, PS, PS, r, g, b, 1, true);
				}
			
			}

			if(SHOW_VELOCITY_FIELD)
			{
				double dx, dy, dl;
				for (int y = 0; y < sizey; y+=5)
				{
					for (int x = 0; x < sizex; x+=5)
					{
						dx = f1->vx[y][x];
						dy = f1->vy[y][x];
						dl = sqrt((dx * dx) + (dy * dy)) ;
						if (dl > PS)
						{
							dx = dx/dl * 2 * PS;
							dy = dy/dl * 2 * PS;
						}
						game.drawLine((x+0.5) * PS, (y+0.5) * PS, (x+0.5) * PS + dx, (y+0.5) * PS + dy, 0, 255, 0, 1);
					}

				}
			}
			game.displayWindow();
			time0 = time1;
			time1 = SDL_GetTicks();
			
		}
	}
	else
		return -1;
	free(force);
	free(f1);
	free(f0);
	free(trace);
	QuitSDL();
	return 0;
}