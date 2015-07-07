#include "canvas.h"

Canvas::Canvas(float &left, float &right, float &bottom, float &top)
{
	this->bottom = bottom;
	this->top = top;
	this->left = left;
	this->right = right;
}

Canvas::Canvas()
{
}


Canvas::~Canvas()
{
}
