#include "DXUT.h"
#include "Shader.h"

Shader::Shader() :
	shader(nullptr)
{

}

Shader::~Shader()
{
	SAFE_RELEASE(shader);
}