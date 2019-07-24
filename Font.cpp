#include "DXUT.h"
#include "Font.h"

Font::Font() :
	font(nullptr)
{

}

Font::~Font()
{
	SAFE_RELEASE(font);
}