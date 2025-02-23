#ifndef TEXTURE_H
#define TEXTURE_H

#include "StandardIncludes.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();

	GLuint GetTexture() { return texture; }

	void LoadTexture(string _filename);
	void LoadCubeMap(vector<string> _faces);
	void Cleanup();

private:
	bool EndsWith(const string& _str, const string& _suffix);
	int width;
	int height;
	int channels;
	GLuint texture;
};

#endif TEXTURE_H

