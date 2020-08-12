#include "Game.h"

#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>
#include <time.h>
#include <Windows.h>



// function to create a render window
static sfRenderWindow* createRenderWindow(
	unsigned int x,
	unsigned int y,
	unsigned int bpp,
	char* title)
{
	sfVideoMode mode = { x, y, bpp };

	return (sfRenderWindow_create(mode, title, sfClose | sfResize, NULL));
}

int main(int argc, char* argv[], char* envp[])
{
	// check if the version is debug or release
#ifdef NDEBUG
	// hide console on release
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_HIDE);
#elif _DEBUG
	// activate console on debug
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_SHOW);
#endif

	// seed random
	srand((unsigned int)time(NULL));

	// setup CSFML
	sfClock* deltaClock = sfClock_create();
	sfRenderWindow* window = createRenderWindow(1280, 720, 32, "Animation");
	sfEvent event;

	GameLoad();

	while (sfRenderWindow_isOpen(window))
	{

		// get deltatime
		float dt = sfTime_asSeconds(sfClock_restart(deltaClock));

		// close the window when the user wishes
		while (sfRenderWindow_pollEvent(window, &event))
		{
			if (event.type == sfEvtClosed)
			{
				sfRenderWindow_close(window);
			}
		}

		if (dt < 1)
		{
			// ↓ update game ↓
			GameUpdate(dt);
		}
		// clean the window
		sfRenderWindow_clear(window, sfBlack);
		// ↓ draw game ↓
		GameDraw(window);
		// display the window
		sfRenderWindow_display(window);
	}

	GameDestroy();
	//destroy elements
	sfRenderWindow_destroy(window);
	sfClock_destroy(deltaClock);



}
