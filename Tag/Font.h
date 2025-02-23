#ifndef FONT_H
#define FONT_H
#include "StandardIncludes.h"

class Shader;

struct Character 
{
	unsigned int textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font
{
public:
	Font();
	virtual ~Font();
	void Cleanup();
	void Create(Shader* _shader, string _name, FT_UInt _size);
	void RenderText(string _text, float _x, float _y, float _scale, glm::vec3 _color);
private:
	Shader* shader;
	FT_Library library;
	FT_Face face;
	map<char, Character> characters;
	GLuint vertexBuffer;
	glm::mat4 orthoProj = {};

	void Initialize(string _fileName, FT_UInt _size);
	void CreateCharacters();
	void AllocateBuffers();
};

#endif FONT_H

