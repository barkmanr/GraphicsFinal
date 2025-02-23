#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "StandardIncludes.h"
#include "Texture.h"

class Shader;
enum State;

class PostProcessor
{
public:
	PostProcessor();
	void Create(Shader* _postShader);
	void Cleanup();
	void Start();
	void End(int freq, int amp, float time);
	void updateState();
	void updateState2();
	string getState();
	void setLine(bool val) { isline = val; }
	void setState(int num) { num++; state = static_cast<State>(num); }

private:
	void CreateVertices();
	void CreateBuffers();
	void BindVertices();

	GLuint framebuffer;
	GLuint textureColorbuffer;
	GLuint renderBufferObject;
	GLuint vertexBuffer; //gpu
	Shader* postShader;

	State state;
	bool isline;
};

enum State
{
	Normal = 1,
	Blue
};

#endif POSTPROCESSOR

