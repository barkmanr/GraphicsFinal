#ifndef RESOLUTION_H
#define RESOLUTION_H

struct Resolution
{
	float width;
	float height;
	float fov;

	Resolution(float _w, float _h) : Resolution(_w, _h, 45.0f) { }
	Resolution() : Resolution(1280.0f, 768.0f, 45.0f) {}
	Resolution(int _f) : Resolution(1280.0f, 768.0f, _f) { }

	Resolution(float _w, float _h, float _f)
	{
		width = _w;
		height = _h;
		fov = _f;
	}
};


#endif RESOLUTION_H
