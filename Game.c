#include <stdio.h>
#include <SFML/Graphics.h>
#include "Game.h"
#include "Animation.h"

Animation* anim;

void GameLoad()
{
	anim = AnimationCreate((sfVector2u) { 256, 256 }, 3);
	AnimationSetSpriteSheet(
		anim,
		"Assets/Images/test.png",
		"horizontal",
		NULL,
		NULL);
	AnimationPlay(anim, Default);
	AnimationSetFramerate(anim, 1);
	AnimationMove(anim, (sfVector2f) { 256 / 2, 256 / 2 });
}
void GameUpdate(float _dt)
{
	AnimationUpdate(anim, _dt);
	if (sfKeyboard_isKeyPressed(sfKeyEnter))
		AnimationStop(anim);
}
void GameDraw(sfRenderWindow* _window)
{
	AnimationDraw(_window, anim, NULL);
}
void GameDestroy()
{
	AnimationDestroy(anim);
}