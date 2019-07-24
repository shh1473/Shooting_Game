#pragma once

#include "Common.h"

class Shader
{
public:
	Shader();
	~Shader();

public:
	ID3DXEffect *shader;

};