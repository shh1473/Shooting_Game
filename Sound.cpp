#include "DXUT.h"
#include "Sound.h"

CSoundManager Sound::manager;

Sound::Sound() :
	sound(nullptr)
{

}

Sound::~Sound()
{
	SAFE_DELETE(sound);
}