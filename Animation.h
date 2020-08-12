#pragma once

///
// Headers
/// 
#include <SFML/Graphics.h>

/// @brief Differents possible states of playing a animation
/// (Default, Looped, Reversed, FlipX, FlipY)
enum State
{
	/// @brief Default state
	Default = 0x01,
	/// @brief Set animation in loop
	Looped = 0x02,
	/// @brief Reverse the animation order
	Reversed = 0x04,
	/// @brief Apply a horizontal flip
	FlipX = 0x08,
	/// @brief Apply a vertical flip
	FlipY = 0x10
};

typedef struct
{
	float Clock;
	unsigned char State;
	sfBool IsPlaying;
	float Duration;
	sfTexture* SpriteSheet;
	sfSprite* Sprite;
	sfVector2u FrameSize;
	unsigned char FramesNb;
	unsigned char CurrentFrame;
	char* SpriteSheetStructure;
	sfVector2u AnimationOffset;
	sfVector2u BlockLength;
	sfVector2f Scale;
}Animation;

//////////////////////////////////////////////////////////////
/// @brief Create a new animation
/// 
/// @param _frameSize Size of a animation
/// 
/// @param _frameNb Number of frames
/// 
/// @return Animation object, or NULL if the animation cannot be create
//////////////////////////////////////////////////////////////
Animation* AnimationCreate(sfVector2u _frameSize, unsigned char _framesNb);

//////////////////////////////////////////////////////////////
/// @brief Update an animation sprite with its current frame
/// 
/// @param _anim Animation object
//////////////////////////////////////////////////////////////
static void AnimationFrameUpdate(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Update an animation
/// 
/// @param _anim Animation object
/// 
/// @param _dt Delta time
//////////////////////////////////////////////////////////////
void AnimationUpdate(Animation* _anim, float _dt);

//////////////////////////////////////////////////////////////
/// @brief Draw an animation
/// 
/// @param _window Render window object
/// 
/// @param _anim Animation object
/// 
/// @param _states Render states to use, NULL to use the default states
//////////////////////////////////////////////////////////////
void AnimationDraw(
	const sfRenderWindow* _window,
	const Animation* _anim,
	const sfRenderStates* _states);

////////////////////////////////////////////////////////////
/// @brief Destroy an existing animation
///
/// @param _anim Animation object
///
////////////////////////////////////////////////////////////
void AnimationDestroy(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Play an animation with the given state
/// 
/// @param _anim Animation object
/// 
/// @param _state State of the animation
//////////////////////////////////////////////////////////////
void AnimationPlay(Animation* _anim, unsigned char _state);

//////////////////////////////////////////////////////////////
/// @brief Pause an animation
/// 
/// @param _anim Animation object
//////////////////////////////////////////////////////////////
void AnimationPause(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Rewind an animation
/// 
/// @param _anim Animation object
//////////////////////////////////////////////////////////////
void AnimationRewind(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Stop an animation (=pause and rewind)
/// 
/// @param _anim Animation object
//////////////////////////////////////////////////////////////
void AnimationStop(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Get the duration of an animation
/// 
/// @param _anim Animation object
/// 
/// @return Duration of a animation
//////////////////////////////////////////////////////////////
float AnimationGetDuration(const Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Get the frame that an animation is playing
/// 
/// @param _anim Animation object
/// 
/// @return The frame that the animation is playing
//////////////////////////////////////////////////////////////
unsigned char AnimationGetCurrentFrame(const Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Tell if an animation is playing
/// 
/// @param _anim Animation object
/// 
/// @return State of playing of a animation
//////////////////////////////////////////////////////////////
sfBool AnimationIsPlaying(const Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Get the state of an animation
/// 
/// @param _anim Animation object
/// 
/// @return State of the animation
//////////////////////////////////////////////////////////////
unsigned char AnimationGetState(const Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Get the frame size of an animation
/// 
/// @param _anim Animation object
/// 
/// @return Size of a frame
//////////////////////////////////////////////////////////////
sfVector2u AnimationGetFrameSize(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Get the frames number of an animation
/// 
/// @param _anim Animation object
/// 
/// @return Number of frame of the animation
//////////////////////////////////////////////////////////////
unsigned char AnimationGetFramesNb(Animation* _anim);

//////////////////////////////////////////////////////////////
/// @brief Create a sprite sheet texture and set 
/// 
/// @param _anim Animation object
/// 
/// @param _filename File name of the file containing sprite sheet
/// 
/// @param _structure Structure of the sprite sheet (horizontal, vertical, block)
/// 
/// @param _animationOffset Offset of the sprite sheet in the file,
///						   NULL for no offset
/// 
/// @param _blockLength Width and height of the block sprite sheet,
///					   NULL if sprite sheet is not a block
//////////////////////////////////////////////////////////////
void AnimationSetSpriteSheet(
	Animation* _anim,
	const char* _filename,
	const char* _structure,
	sfVector2u* _animationOffset,
	sfVector2u* _blockLength);

//////////////////////////////////////////////////////////////
/// @brief Set the duration of an animation 
/// 
/// @param _anim Animation object
/// 
/// @param _duration Duration of the animation
//////////////////////////////////////////////////////////////
void AnimationSetDuration(Animation* _anim, float _duration);

//////////////////////////////////////////////////////////////
/// @brief Set the frame rate of an animation
/// 
/// Calculate and set duration according to the wanted frame rate.
/// 
/// @param _anim Animation object
/// 
/// @param _framerate Frame rate of the animation
//////////////////////////////////////////////////////////////
void AnimationSetFramerate(Animation* _anim, float _framerate);

//////////////////////////////////////////////////////////////
/// @brief Set the sate of an animation
/// 
/// @param _anim Animation object
/// 
/// @param _state State for the animation
//////////////////////////////////////////////////////////////
void AnimationSetState(Animation* _anim, unsigned char _state);

//////////////////////////////////////////////////////////////
/// @brief Set the frame size of an animation
/// 
/// @param _anim Animation object
/// 
/// @param _frameSize Frame size of a animation
//////////////////////////////////////////////////////////////
void AnimationSetFrameSize(Animation* _anim, sfVector2u _frameSize);

//////////////////////////////////////////////////////////////
/// @brief Set the frames number of an animation
/// 
/// @param _anim Animation object
/// 
/// @param _framesNb Frames number
//////////////////////////////////////////////////////////////
void AnimationSetFramesNb(Animation* _anim, unsigned char _framesNb);

//////////////////////////////////////////////////////////////
/// @brief Get the frame rate of an animation
/// 
/// Calculate frame rate from the duration of the animation.
/// 
/// @param _anim Animation object
/// 
/// @return Frame rate of the animation
//////////////////////////////////////////////////////////////
float AnimationGetFramerate(Animation* _anim);

////////////////////////////////////////////////////////////
/// @brief Copy an existing animation
///
/// @param _animation Animation to copy
///
/// @return Copied object, NULL if the copy fail
///
////////////////////////////////////////////////////////////
Animation* AnimationCopy(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Set the position of an animation
///
/// This function completely overwrites the previous position.
/// See AnimationMove to apply an offset based on the previous position instead.
/// The default position of a animation Animation object is (0, 0).
///
/// @param _animation   Animation object
/// @param _position New position
///
////////////////////////////////////////////////////////////
void AnimationSetPosition(Animation* _animation, sfVector2f _position);

////////////////////////////////////////////////////////////
/// @brief Set the orientation of an animation
///
/// This function completely overwrites the previous rotation.
/// See AnimationRotate to add an angle based on the previous rotation instead.
/// The default rotation of a sprite Sprite object is 0.
///
/// @param _animation Animation object
/// @param _angle  New rotation, in degrees
///
////////////////////////////////////////////////////////////
void AnimationSetRotation(Animation* _animation, float _angle);

////////////////////////////////////////////////////////////
/// @brief Set the scale factors of an animation
///
/// This function completely overwrites the previous scale.
/// See AnimationScale to add a factor based on the previous scale instead.
/// The default scale of a sprite Sprite object is (1, 1).
///
/// @param _animation Animation object
/// @param _scale  New scale factors
///
////////////////////////////////////////////////////////////
void AnimationSetScale(Animation* _animation, sfVector2f _scale);

////////////////////////////////////////////////////////////
/// @brief Get the position of an animation
///
/// @param _animation Animation object
///
/// @return Current position
///
////////////////////////////////////////////////////////////
sfVector2f AnimationGetPosition(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the orientation of an animation
///
/// The rotation is always in the range [0, 360].
///
/// @param _animation Animation object
///
/// @return Current rotation, in degrees
///
////////////////////////////////////////////////////////////
float AnimationGetRotation(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the current scale of an animation
///
/// @param _animation Animation object
///
/// @return Current scale factors
///
////////////////////////////////////////////////////////////
sfVector2f AnimationGetScale(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the local origin of an animation
///
/// @param _animation Animation object
///
/// @return Current origin
///
////////////////////////////////////////////////////////////
sfVector2f AnimationGetOrigin(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Move an animation by a given offset
///
/// This function adds to the current position of the object,
/// unlike AnimationSetPosition which overwrites it.
///
/// @param _animation Animation object
///
///  @param _offset Offset
///
////////////////////////////////////////////////////////////
void AnimationMove(Animation* _animation, sfVector2f _offset);

////////////////////////////////////////////////////////////
/// @brief Rotate an animation
///
/// This function adds to the current rotation of the object,
/// unlike AnimationSetRotation which overwrites it.
///
/// @param _animation Animation object
/// 
/// @param _angle  Angle of rotation, in degrees
///
////////////////////////////////////////////////////////////
void AnimationRotate(Animation* _animation, float _angle);

////////////////////////////////////////////////////////////
/// @brief Scale an animation
///
/// This function multiplies the current scale of the object,
/// unlike AnimationSetScale which overwrites it.
///
/// @param _animation Animation object
/// 
/// @param _factors Scale factors
///
////////////////////////////////////////////////////////////
void AnimationScale(Animation* _animation, sfVector2f _factors);

////////////////////////////////////////////////////////////
/// @brief Get the combined transform of an animation
///
/// @param _animation Animation object
///
/// @return Transform combining the position/rotation/scale/origin of the object
///
////////////////////////////////////////////////////////////
sfTransform AnimationGetTransform(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the inverse of the combined transform of an animation
///
/// @param _animation Animation object
///
/// @return Inverse of the combined transformations applied to the object
///
////////////////////////////////////////////////////////////
sfTransform AnimationGetInverseTransform(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Set the global color of an animation
///
/// This color is modulated (multiplied) with the sprite's texture of the 
/// animation. It can be used to colorize the animation, or change its global 
/// opacity.
/// By default, the animation's color is opaque white.
///
/// @param _animation Animation object
/// 
/// @param _color  New color of the sprite
///
////////////////////////////////////////////////////////////
void AnimationSetColor(Animation* _animation, sfColor _color);

////////////////////////////////////////////////////////////
/// @brief Get the source sprite sheet texture of an animation
///
/// If the animation has no source sprite sheet texture,
/// a NULL pointer is returned.
/// The returned pointer is const, which means that you can't
/// modify the sprite sheet texture when you retrieve it with this function.
///
/// @param _animation Animation object
///
/// @return Pointer to the animation's sprite sheet texture
///
////////////////////////////////////////////////////////////
const sfTexture* AnimationGetSpriteSheetTexture(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the global color of an animation
///
/// @param _animation Animation object
///
/// @return Global color of the sprite
///
////////////////////////////////////////////////////////////
sfColor AnimationGetColor(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the local bounding rectangle of an animation
///
/// The returned rectangle is in local coordinates, which means
/// that it ignores the transformations (translation, rotation,
/// scale, ...) that are applied to the entity.
/// In other words, this function returns the bounds of the
/// entity in the entity's coordinate system.
///
/// @param _animation Animation object
///
/// @return Local bounding rectangle of the entity
///
////////////////////////////////////////////////////////////
sfFloatRect AnimationGetLocalBounds(const Animation* _animation);

////////////////////////////////////////////////////////////
/// @brief Get the global bounding rectangle of an animation
///
/// The returned rectangle is in global coordinates, which means
/// that it takes in account the transformations (translation,
/// rotation, scale, ...) that are applied to the entity.
/// In other words, this function returns the bounds of the
/// sprite in the global 2D world's coordinate system.
///
/// @param _animation Animation object
///
/// @return Global bounding rectangle of the entity
///
////////////////////////////////////////////////////////////
sfFloatRect AnimationGetGlobalBounds(const Animation* _animation);