#pragma once

#include "Common.h"

class Sound
{
public:
	Sound();
	~Sound();

public:
	static CSoundManager manager;

	CSound *sound;

};