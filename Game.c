#include <stdio.h>
#include <SFML/Graphics.h>
#include "Game.h"
#include "Animation.h"

Animation* anim;
static sfEvent event;

void GameLoad()
{
	anim = AnimationCreate((sfVector2u) { 96, 125 }, 4);
	AnimationSetSpriteSheet(
		anim,
		"Assets/Images/SpriteSheet.png",
		"horizontal",
		NULL,
		NULL);
	AnimationPlay(anim, Default | Looped);
	AnimationSetFramerate(anim, 8);
	AnimationMove(anim, (sfVector2f) { 256 / 2, 256 / 2 });
}

static void GameEvents(sfRenderWindow* _window)
{
	// polling for event
	while (sfRenderWindow_pollEvent(_window, &event))
	{
		switch (event.type)
		{
		// key pressed
		case sfEvtKeyPressed:
			switch (event.key.code)
			{
			case sfKeyEnter:
				AnimationStop(anim);
				break;

			case sfKeySpace:
				if (AnimationIsPlaying(anim))
				{
					AnimationPause(anim);
				}
				else
				{
					AnimationPlay(anim, AnimationGetState(anim));
				}
				break;

			default:
				break;
			}
			break;

		// window closed
		case sfEvtClosed:
			sfRenderWindow_close(_window);
			break;

		default:
			break;
		}
	}
}

void GameUpdate(float _dt, sfRenderWindow* _window)
{
	GameEvents(_window);
	AnimationUpdate(anim, _dt);
}
void GameDraw(sfRenderWindow* _window)
{
	AnimationDraw(_window, anim, NULL);
}
void GameDestroy()
{
	AnimationDestroy(anim);
}