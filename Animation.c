#include "Animation.h"

#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Animation* AnimationCreate(sfVector2u _frameSize, unsigned char _framesNb)
{
	Animation* anim = malloc(sizeof(Animation));
	if (anim == NULL)
	{
		printf("Error during memory allocation !");
		return NULL;
	}
	anim->IsPlaying = sfFalse;
	anim->State = Default;
	anim->FrameSize = _frameSize;
	anim->FramesNb = _framesNb;
	anim->Clock = 0;
	anim->Duration = 1;
	anim->Scale = (sfVector2f){ 1, 1 };
	// creating the sprite
	anim->Sprite = sfSprite_create();
	if (anim->Sprite == NULL)
	{
		printf("Error during cration of the sprite !");
		return NULL;
	}
	sfSprite_setOrigin(anim->Sprite,
		(sfVector2f) {
		(float)anim->FrameSize.x / 2,
		(float)anim->FrameSize.y / 2 });

	return anim;
}

static void AnimationFrameUpdate(Animation* _anim)
{
	// The position of the top left corner of the next frame in the texture
	sfVector2i framePos = {
			_anim->FrameSize.x * (int)_anim->CurrentFrame,
			(_anim->FrameSize.y * (int)_anim->CurrentFrame) };

	// Select the next frame according to the sprite sheet structure
	if (_anim->SpriteSheetStructure == "horizontal")
	{
		sfSprite_setTextureRect(
			_anim->Sprite,
			(sfIntRect) {
				framePos.x,
				0,
				_anim->FrameSize.x,
				_anim->FrameSize.y });
	}
	else if (_anim->SpriteSheetStructure == "vertical")
	{
		sfSprite_setTextureRect(
			_anim->Sprite,
			(sfIntRect) {
				0,
				framePos.y,
				_anim->FrameSize.x,
				_anim->FrameSize.y });
	}
	else if (_anim->SpriteSheetStructure == "block")
	{
		// Block length in pixel
		sfVector2i blockPixelSize = {
			_anim->BlockLength.x * (int)_anim->FrameSize.x,
			_anim->BlockLength.y * (int)_anim->FrameSize.y
		};

		sfIntRect frameRect = {
			framePos.x % blockPixelSize.x,
			framePos.y / blockPixelSize.y * _anim->FrameSize.y,
			_anim->FrameSize.x,
			_anim->FrameSize.y
		};

		sfSprite_setTextureRect(_anim->Sprite, frameRect);
	}
}

void AnimationUpdate(Animation* _anim, float _dt)
{
	// Flip sprite according to the state of the animation
	if (_anim->State & FlipX && _anim->State & FlipY)
	{
		sfSprite_setScale(_anim->Sprite,
			(sfVector2f) { -_anim->Scale.x, -_anim->Scale.y });
	}
	else if (_anim->State & FlipX)
	{
		sfSprite_setScale(_anim->Sprite,
			(sfVector2f) { -_anim->Scale.x, _anim->Scale.y });
	}
	else if (_anim->State & FlipY)
	{
		sfSprite_setScale(_anim->Sprite,
			(sfVector2f) { _anim->Scale.x, -_anim->Scale.y });
	}
	else
	{
		sfSprite_setScale(_anim->Sprite,
			(sfVector2f) { _anim->Scale.x, _anim->Scale.y });
	}
		

	if (_anim->IsPlaying)
	{

		if (_anim->State & Reversed)
		{
			// Play animation in reverse 
			_anim->Clock -= _dt;

			double animationProgression =
				(double)_anim->Clock / (double)_anim->Duration;

			int nextFrame =
				(int)(floor(animationProgression * (double)_anim->FramesNb));

			// If animation has played his last frame
			if (_anim->Clock < 0 && nextFrame < 0)
			{
				if (_anim->State & Looped)
				{
					_anim->Clock = _anim->Duration;
					_anim->CurrentFrame = _anim->FramesNb - 1;
				}
				else
				{
					_anim->IsPlaying = sfFalse;
					_anim->CurrentFrame = 0;
				}
			}
			else
			{
				// Update the animation frame
				_anim->CurrentFrame = (unsigned char)nextFrame;
			}
		}
		else
		{
			// Play animation
			_anim->Clock += _dt;
			double animationProgression =
				(double)_anim->Clock / (double)_anim->Duration;

			int nextFrame =
				(int)(floor(animationProgression * (double)_anim->FramesNb));

			// If animation has played his last frame
			if (_anim->Clock > _anim->Duration
				&& nextFrame > _anim->FramesNb - 1)
			{
				if (_anim->State & Looped)
				{
					_anim->Clock = 0;
					_anim->CurrentFrame = 0;
				}
				else
				{
					_anim->IsPlaying = sfFalse;
					_anim->CurrentFrame = _anim->FramesNb - 1;
				}
			}
			else
			{
				// Update the animation frame
				_anim->CurrentFrame = nextFrame;
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
		(sfRenderWindow*)_window, _anim->Sprite, _states);
}

void AnimationDestroy(Animation* _anim)
{
	sfTexture_destroy((sfTexture*)_anim->SpriteSheet);
	sfSprite_destroy(_anim->Sprite);
	free(_anim);
}

void AnimationPlay(Animation* _anim, unsigned char _state)
{
	_anim->IsPlaying = sfTrue;
	_anim->State = _state;
	if (_anim->State & Reversed)
	{
		_anim->Clock -= _anim->Duration;
	}
}

void AnimationPause(Animation* _anim)
{
	_anim->IsPlaying = sfFalse; 
}

void AnimationRewind(Animation* _anim)
{
	_anim->Clock = 0;
	_anim->CurrentFrame = 0;
	if (_anim->State & Reversed)
	{
		_anim->Clock = _anim->Duration;
		_anim->CurrentFrame = _anim->FramesNb - 1;
	}
	AnimationFrameUpdate(_anim);
}

void AnimationStop(Animation* _anim)
{
	_anim->IsPlaying = sfFalse;
	_anim->Clock = 0;
	_anim->CurrentFrame = 0;
	if (_anim->State & Reversed)
	{
		_anim->Clock = _anim->Duration;
		_anim->CurrentFrame = _anim->FramesNb - 1;
	}
	AnimationFrameUpdate(_anim);
}

Animation* AnimationCopy(const Animation* _animation)
{
	Animation* dest = malloc(sizeof(_animation));
	if (dest != NULL)
		memcpy(dest, _animation, sizeof(_animation));
	else
		printf("Error during copying animation !");
	return dest;
}

// Getters
float AnimationGetDuration(const Animation* _anim) 
{
	return _anim->Duration; 
}

unsigned char AnimationGetCurrentFrame(const Animation* _anim)
{
	return _anim->CurrentFrame;
}

sfBool AnimationIsPlaying(const Animation* _anim) 
{
	return _anim->IsPlaying; 
}

unsigned char AnimationGetState(const Animation* _anim) 
{
	return _anim->State; 
}

sfVector2u AnimationGetFrameSize(Animation* _anim) 
{
	return _anim->FrameSize;
}

unsigned char AnimationGetFramesNb(Animation* _anim) 
{
	return _anim->FramesNb; 
}

float AnimationGetFramerate(Animation* _anim)
{
	return _anim->Duration / _anim->FramesNb;
}

const sfTexture* AnimationGetSpriteSheetTexture(const Animation* _animation)
{
	return sfSprite_getTexture(_animation->Sprite);
}

sfVector2f AnimationGetPosition(const Animation* _animation)
{
	return sfSprite_getPosition(_animation->Sprite);
}

float AnimationGetRotation(const Animation* _animation)
{
	return sfSprite_getRotation(_animation->Sprite);
}

sfVector2f AnimationGetScale(const Animation* _animation)
{
	return sfSprite_getScale(_animation->Sprite);
}

sfVector2f AnimationGetOrigin(const Animation* _animation)
{
	return sfSprite_getOrigin(_animation->Sprite);
}

sfTransform AnimationGetTransform(const Animation* _animation)
{
	return sfSprite_getTransform(_animation->Sprite);
}

sfTransform AnimationGetInverseTransform(const Animation* _animation)
{
	return sfSprite_getInverseTransform(_animation->Sprite);
}

sfColor AnimationGetColor(const Animation* _animation)
{
	return sfSprite_getColor(_animation->Sprite);
}

sfFloatRect AnimationGetLocalBounds(const Animation* _animation)
{
	return sfSprite_getLocalBounds(_animation->Sprite);
}

sfFloatRect AnimationGetGlobalBounds(const Animation* _animation)
{
	return sfSprite_getGlobalBounds(_animation->Sprite);
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
		_anim->AnimationOffset = *_animationOffset;
	else
		_anim->AnimationOffset = (sfVector2u){ 0, 0 };

	if (_blockLength != NULL)
		_anim->BlockLength = *_blockLength;
	else
		_anim->BlockLength = (sfVector2u){ 0, 0 };

	_anim->SpriteSheetStructure = (char*)_structure;

	// Surface area for the sprite sheet
	const sfIntRect area;

	// Calculate surface area according to the sprite sheet structure
	if (_anim->SpriteSheetStructure == "horizontal")
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->AnimationOffset.x,
			_anim->AnimationOffset.y,
			_anim->FrameSize.x * (int)_anim->FramesNb,
			_anim->FrameSize.y
		};
	}
	else if (_anim->SpriteSheetStructure == "vertical")
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->AnimationOffset.x,
			_anim->AnimationOffset.y,
			_anim->FrameSize.x,
			_anim->FrameSize.y * (int)_anim->FramesNb
		};
	}
	else if (_anim->SpriteSheetStructure == "block")
	{
		*(sfIntRect*)&area = (sfIntRect){
			_anim->AnimationOffset.x,
			_anim->AnimationOffset.y,
			_anim->BlockLength.x * (int)_anim->FrameSize.x,
			_anim->BlockLength.y * (int)_anim->FrameSize.y };
	}

	_anim->SpriteSheet = sfTexture_createFromFile(
		_filename,
		(const sfIntRect*)&area);

	// creating texture for the spritesheet
	if (_anim->SpriteSheet == NULL)
	{
		printf("Error during cration of the sprite sheet texture !");
	}
	else
	{
		sfSprite_setTexture(_anim->Sprite, _anim->SpriteSheet, sfTrue);
		sfSprite_setTextureRect(
			_anim->Sprite,
			(sfIntRect) { 0, 0, _anim->FrameSize.x, _anim->FrameSize.y });
	}
}

void AnimationSetDuration(Animation* _anim, float _duration)
{
	_anim->Duration = _duration;
}

void AnimationSetFramerate(Animation* _anim, float _framerate)
{
	_anim->Duration = (float)(_anim->FramesNb) / _framerate;
}

void AnimationSetState(Animation* _anim, unsigned char _state)
{
	_anim->State = _state;
}

void AnimationSetFrameSize(Animation* _anim, sfVector2u _frameSize)
{
	_anim->FrameSize = _frameSize;
}

void AnimationSetFramesNb(Animation* _anim, unsigned char _frameNb)
{
	_anim->FramesNb = _frameNb;
}

void AnimationSetPosition(Animation* _animation, sfVector2f _position)
{
	sfSprite_setPosition(_animation->Sprite, _position);
}

void AnimationSetRotation(Animation* _animation, float _angle)
{
	sfSprite_setRotation(_animation->Sprite, _angle);
}

void AnimationSetScale(Animation* _animation, sfVector2f _scale)
{
	sfSprite_setScale(_animation->Sprite, _scale);
	_animation->Scale = _scale;
}

void AnimationMove(Animation* _animation, sfVector2f _offset)
{
	sfSprite_move(_animation->Sprite, _offset);
}

void AnimationRotate(Animation* _animation, float _angle)
{
	sfSprite_rotate(_animation->Sprite, _angle);
}

void AnimationScale(Animation* _animation, sfVector2f _factors)
{
	sfSprite_scale(_animation->Sprite, _factors);
}

void AnimationSetColor(Animation* _animation, sfColor _color)
{
	sfSprite_setColor(_animation->Sprite, _color);
}