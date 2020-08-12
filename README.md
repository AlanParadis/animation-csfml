# Animation CSFML

Animation system written in C with CSFML

The animation system is only [Animation.c](https://github.com/AlanParadis/animation-csfml/blob/master/Animation.c) and [Animation.h](https://github.com/AlanParadis/animation-csfml/blob/master/Animation.h)

## How to use
Create a new animation with :
```c
Animation* AnimationCreate(sfVector2u _frameSize, unsigned char _framesNb)
```
Set a sprite sheet for the animation  :
```c
void AnimationSetSpriteSheet(Animation* _anim, const char* _filename, const char* _structure, sfVector2u* _animationOffset, sfVector2u* _blockLength);
```
Play with the animation with those functions :
```c
void AnimationPlay(Animation* _anim, unsigned char _state);

void AnimationPause(Animation* _anim);

void AnimationRewind(Animation* _anim);

void AnimationStop(Animation* _anim);
```
This animation system support 5 differents sates : 

 - Default (Default state)
 
 - 	Looped (Set animation in loop)
 
 -  Reversed (Reverse the animation order)

 - FlipX (Apply a horizontal flip)

 -  FlipY (Apply a vertical flip)

Animation has the same function as standard sfSprite but the name format is different.
The function are "Animation" followed by the PascalCase name of the function:

example : `type AnimationThisIsAFunction(type _arg,...)`

See an example of an animation and interaction with the animation in [Game.c](https://github.com/AlanParadis/animation-csfml/blob/master/Game.c)

Note :
All animation has there origin center in the middle and cannot be changed.

## Acknowledgements

License: [DBDA](https://dbad-license.org/)

This library uses and is based on [SFML](http://www.sfml-dev.org/ "Simple and Fast Multimedia Library") and [CSFML](http://www.sfml-dev.org/download/csfml/).

C programming language is used.
