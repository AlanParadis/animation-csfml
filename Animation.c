#include "Animation.h"

#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Animation
{
	float clock;
	unsigned char state;
	sfBool isPlaying;
	float duration;
	sfTexture* spriteSheet;
	sfSprite* sprite;
	sfVector2u frameSize;
	unsigned char framesNb;
	unsigned char currentFrame;
	char* spriteSheetStructure;
	sfVector2u animationOffset;
	sfVector2u blockLength;
	sfVector2f scale;
};

//////////////////////////////////////////////////////////////
/// @brief Update an animation sprite with its current frame
/// 
/// @param _anim Animation object
//////////////////////////////////////////////////////////////
static void AnimationFrameUpdate(Animation* _anim);

Animation* AnimationCreate(sfVector2u _frameSize, unsigned char _framesNb)
{
	Animation* anim = (Animation*) malloc(sizeof(Animation));
	if (anim == NULL)
	{
		printf("Error during memory allocation !");
		return NULL;
	}
	anim->isPlaying = sfFalse;
	anim->state = DEFAULT;
	anim->frameSize = _frameSize;
	anim->framesNb = _framesNb;
	anim->clock = 0;
	anim->duration = 1;
	anim->scale = (sfVector2f){ 1, 1 };
	// creating the sprite
	anim->sprite = sfSprite_create();
	if (anim->sprite == NULL)
	{
		printf("Error during cration of the sprite !");
		return NULL;
	}
	sfSprite_setOrigin(anim->sprite,
		(sfVector2f) {
		(float)anim->frameSize.x / 2,
		(float)anim->frameSize.y / 2 });
	anim->currentFrame = 0;
	anim->spriteSheetStructure = (char*) malloc(15);

	return anim;
}

static void AnimationFrameUpdate(Animation* _anim)
{
	// The position of the top left corner of the next frame in the texture
	sfVector2i framePos = {
			_anim->frameSize.x * (int)_anim->currentFrame,
			(_anim->frameSize.y * (int)_anim->currentFrame) };

	// Select the next frame according to the sprite sheet structure
	if (strcmp(_anim->spriteSheetStructure, "horizontal") == 0)
	{
		sfSprite_setTextureRect(
			_anim->sprite,
			(sfIntRect) {
				framePos.x,
				0,
				_anim->frameSize.x,
				_anim->frameSize.y });
	}
	else if (strcmp(_anim->spriteSheetStructure, "vertical") == 0)
	{
		sfSprite_setTextureRect(
			_anim->sprite,
			(sfIntRect) {
				0,
				framePos.y,
				_anim->frameSize.x,
				_anim->frameSize.y });
	}
	else if (strcmp(_anim->spriteSheetStructure, "block") == 0)
	{
		// Block length in pixel
		sfVector2i blockPixelSize = {
			_anim->blockLength.x * (int)_anim->frameSize.x,
			_anim->blockLength.y * (int)_anim->frameSize.y
		};

		sfIntRect frameRect = {
			framePos.x % blockPixelSize.x,
			(int)ceil(_anim->currentFrame / _anim->blockLength.x)* _anim->frameSize.y,
			_anim->frameSize.x,
			_anim->frameSize.y
		};

		sfSprite_setTextureRect(_anim->sprite, frameRect);
	}
}

void AnimationUpdate(Animation* _anim, float _dt)
{
	// Flip sprite according to the state of the animation
	if (_anim->state & FLIP_X && _anim->state & FLIP_Y)
	{
		sfSprite_setScale(_anim->sprite,
			(sfVector2f) { -_anim->scale.x, -_anim->scale.y });
	}
	else if (_anim->state & FLIP_X)
	{
		sfSprite_setScale(_anim->sprite,
			(sfVector2f) { -_anim->scale.x, _anim->scale.y });
	}
	else if (_anim->state & FLIP_Y)
	{
		sfSprite_setScale(_anim->sprite,
			(sfVector2f) { _anim->scale.x, -_anim->scale.y });
	}
	else
	{
		sfSprite_setScale(_anim->sprite,
			(sfVector2f) { _anim->scale.x, _anim->scale.y });
	}
		

	if (_anim->isPlaying)
	{

		if (_anim->state & REVERSED)
		{
			// Play animation in reverse 
			_anim->clock -= _dt;

			double animationProgression =
				(double)_anim->clock / (double)_anim->duration;

			int nextFrame =
				(int)(floor(animationProgression * (double)_anim->framesNb));

			// If animation has played his last frame
			if (_anim->clock < 0 && nextFrame < 0)
			{
				if (_anim->state & LOOPED)
				{
					_anim->clock = _anim->duration;
					_anim->currentFrame = _anim->framesNb - 1;
				}
				else
				{
					_anim->isPlaying = sfFalse;
					_anim->currentFrame = 0;
				}
			}
			else
			{
				// Update the animation frame
				_anim->currentFrame = (unsigned char)nextFrame;
			}
		}
		else
		{
			// Play animation
			_anim->clock += _dt;
			double animationProgression =
				(double)_anim->clock / (double)_anim->duration;

			int nextFrame =
				(int)(floor(animationProgression * (double)_anim->framesNb));

			// If animation has played his last frame
			if (_anim->clock > _anim->duration
				&& nextFrame > _anim->framesNb - 1)
			{
				if (_anim->state & LOOPED)
				{
					_anim->clock = 0;
					_anim->currentFrame = 0;
				}
				else
				{
					_anim->isPlaying = sfFalse;
					_anim->currentFrame = _anim->framesNb - 1;
				}
			}
			else
			{
				// Update the animation frame
				_anim->currentFrame = nextFrame;
			}
		}

		AnimationFrameUpdate(_anim);
	}
}

void AnimationDraw(
	const sfRenderWindow* _window,
	const Animation* _anim,
	const sfRenderStates* _states)
{
	sfRenderWindow_drawSprite(
		(sfRenderWindow*)_window, _anim->sprite, _states);
}

void AnimationDestroy(Animation* _anim)
{
	sfTexture_destroy((sfTexture*)_anim->spriteSheet);
	sfSprite_destroy(_anim->sprite);
	free(_anim->spriteSheetStructure);
	free(_anim);
}

void AnimationPlay(Animation* _anim, unsigned char _state)
{
	_anim->isPlaying = sfTrue;
	_anim->state = _state;
	if (_anim->state & REVERSED)
	{
		_anim->clock -= _anim->duration;
	}
}

void AnimationPause(Animation* _anim)
{
	_anim->isPlaying = sfFalse; 
}

void AnimationRewind(Animation* _anim)
{
	_anim->clock = 0;
	_anim->currentFrame = 0;
	if (_anim->state & REVERSED)
	{
		_anim->clock = _anim->duration;
		_anim->currentFrame = _anim->framesNb - 1;
	}
	AnimationFrameUpdate(_anim);
}

void AnimationStop(Animation* _anim)
{
	_anim->isPlaying = sfFalse;
	_anim->clock = 0;
	_anim->currentFrame = 0;
	if (_anim->state & REVERSED)
	{
		_anim->clock = _anim->duration;
		_anim->currentFrame = _anim->framesNb - 1;
	}
	AnimationFrameUpdate(_anim);
}

Animation* AnimationCopy(const Animation* _animation)
{
	Animation* dest = (Animation*) malloc(sizeof(*_animation));
	if (dest != NULL)
	{
		memcpy(dest, _animation, sizeof(*_animation));
		dest->sprite = sfSprite_copy(_animation->sprite);
		dest->spriteSheet = sfTexture_copy(_animation->spriteSheet);
		dest->spriteSheetStructure = (char*) malloc(15);
		strcpy_s(
			dest->spriteSheetStructure,
			15,
			_animation->spriteSheetStructure);
	}
	else
	{
		printf("Error during copying animation !");
	}
	return dest;
}

// Getters
float AnimationGetDuration(const Animation* _anim) 
{
	return _anim->duration; 
}

unsigned char AnimationGetCurrentFrame(const Animation* _anim)
{
	return _anim->currentFrame;
}

sfBool AnimationIsPlaying(const Animation* _anim) 
{
	return _anim->isPlaying; 
}

unsigned char AnimationGetState(const Animation* _anim) 
{
	return _anim->state; 
}

sfVector2u AnimationGetFrameSize(Animation* _anim) 
{
	return _anim->frameSize;
}

unsigned char AnimationGetFramesNb(Animation* _anim) 
{
	return _anim->framesNb; 
}

float AnimationGetFramerate(Animation* _anim)
{
	return _anim->duration / _anim->framesNb;
}

const sfTexture* AnimationGetSpriteSheetTexture(const Animation* _animation)
{
	return sfSprite_getTexture(_animation->sprite);
}

sfVector2f AnimationGetPosition(const Animation* _animation)
{
	return sfSprite_getPosition(_animation->sprite);
}

float AnimationGetRotation(const Animation* _animation)
{
	return sfSprite_getRotation(_animation->sprite);
}

sfVector2f AnimationGetScale(const Animation* _animation)
{
	return sfSprite_getScale(_animation->sprite);
}

sfVector2f AnimationGetOrigin(const Animation* _animation)
{
	return sfSprite_getOrigin(_animation->sprite);
}

sfTransform AnimationGetTransform(const Animation* _animation)
{
	return sfSprite_getTransform(_animation->sprite);
}

sfTransform AnimationGetInverseTransform(const Animation* _animation)
{
	return sfSprite_getInverseTransform(_animation->sprite);
}

sfColor AnimationGetColor(const Animation* _animation)
{
	return sfSprite_getColor(_animation->sprite);
}

sfFloatRect AnimationGetLocalBounds(const Animation* _animation)
{
	return sfSprite_getLocalBounds(_animation->sprite);
}

sfFloatRect AnimationGetGlobalBounds(const Animation* _animation)
{
	return sfSprite_getGlobalBounds(_animation->sprite);
}

// Setters
void AnimationSetSpriteSheet(
	Animation* _anim,
	const char* _filename,
	const char* _structure,
	sfVector2u* _animationOffset,
	sfVector2u* _blockLength)
{
	if (_animationOffset != NULL)
		_anim->animationOffset = *_animationOffset;
	else
		_anim->animationOffset = (sfVector2u){ 0, 0 };

	if (_blockLength != NULL)
		_anim->blockLength = *_blockLength;
	else
		_anim->blockLength = (sfVector2u){ 0, 0 };

	strcpy_s(_anim->spriteSheetStructure,15,_structure);

	// Surface area for the sprite sheet
	const sfIntRect area;

	/// Calculate surface area according to the sprite sheet structure
	if (strcmp(_anim->spriteSheetStructure, "horizontal") == 0)
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->animationOffset.x,
			_anim->animationOffset.y,
			_anim->frameSize.x * (int)_anim->framesNb,
			_anim->frameSize.y
		};
	}
	else if (strcmp(_anim->spriteSheetStructure, "vertical") == 0)
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->animationOffset.x,
			_anim->animationOffset.y,
			_anim->frameSize.x,
			_anim->frameSize.y * (int)_anim->framesNb
		};
	}
	else if (strcmp(_anim->spriteSheetStructure, "block")  == 0)
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->animationOffset.x,
			_anim->animationOffset.y,
			_anim->blockLength.x * (int)_anim->frameSize.x,
			_anim->blockLength.y * (int)_anim->frameSize.y };
	}

	_anim->spriteSheet = sfTexture_createFromFile(
		_filename,
		(const sfIntRect*)&area);

	// creating texture for the spriteSheet
	if (_anim->spriteSheet == NULL)
	{
		printf("Error during cration of the sprite sheet texture !");
	}
	else
	{
		sfSprite_setTexture(_anim->sprite, _anim->spriteSheet, sfTrue);
		sfSprite_setTextureRect(
			_anim->sprite,
			(sfIntRect) { 0, 0, _anim->frameSize.x, _anim->frameSize.y });
	}
}

void AnimationSetDuration(Animation* _anim, float _duration)
{
	_anim->duration = _duration;
}

void AnimationSetFramerate(Animation* _anim, float _framerate)
{
	_anim->duration = (float)(_anim->framesNb) / _framerate;
}

void AnimationSetState(Animation* _anim, unsigned char _state)
{
	_anim->state = _state;
}

void AnimationSetFrameSize(Animation* _anim, sfVector2u _frameSize)
{
	_anim->frameSize = _frameSize;
}

void AnimationSetFramesNb(Animation* _anim, unsigned char _frameNb)
{
	_anim->framesNb = _frameNb;
}

void AnimationSetPosition(Animation* _animation, sfVector2f _position)
{
	sfSprite_setPosition(_animation->sprite, _position);
}

void AnimationSetRotation(Animation* _animation, float _angle)
{
	sfSprite_setRotation(_animation->sprite, _angle);
}

void AnimationSetScale(Animation* _animation, sfVector2f _scale)
{
	sfSprite_setScale(_animation->sprite, _scale);
	_animation->scale = _scale;
}

void AnimationMove(Animation* _animation, sfVector2f _offset)
{
	sfSprite_move(_animation->sprite, _offset);
}

void AnimationRotate(Animation* _animation, float _angle)
{
	sfSprite_rotate(_animation->sprite, _angle);
}

void AnimationScale(Animation* _animation, sfVector2f _factors)
{
	sfSprite_scale(_animation->sprite, _factors);
	_animation->scale = _factors;
}

void AnimationSetColor(Animation* _animation, sfColor _color)
{
	sfSprite_setColor(_animation->sprite, _color);
}
