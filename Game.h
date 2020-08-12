#pragma once

// Headers
#include <SFML/Graphics.h>

/// @brief Initialize game.
void GameLoad();

/// @brief Poll events from the game
/// 
/// @param _window Render window object
static void GameEvents(sfRenderWindow* _window);

/// @brief Update the game.
/// 
/// @param _dt Delta time (in seconds)
/// 
/// @param _window Render window object
void GameUpdate(float _dt, sfRenderWindow* _window);

/// @brief Draw the game.
/// 
/// @param _window Render window object
void GameDraw(sfRenderWindow* _window);

/// @brief Destroy the game.
/// 
/// Free memory and destroy objects.
void GameDestroy();