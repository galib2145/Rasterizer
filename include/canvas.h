#ifndef CANVAS_H
#define CANVAS_H


class Canvas
{
public:
	float left, right, top, bottom;
	Canvas(float &left, float &right, float &bottom, float &top);
	Canvas();
	~Canvas();
};

#endif

