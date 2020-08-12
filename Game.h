#pragma once

#include <SFML/Graphics.h>

void GameLoad();
void GameUpdate(float _dt);
void GameDraw(sfRenderWindow* _window);
void GameDestroy();