#ifndef SKYBOX_H
#define SKYBOX_H
#include "Mesh.h"

class Texture;
class Shader;

class Skybox
{
public:
	Skybox();
	virtual ~Skybox();

	void Create(Shader* _shader, string _file, vector<string> _faces);
	void Cleanup();
	void Render(glm::mat4 _pv);

private:
	void SetShaderVariables(glm::mat4 _pv);
	void BindAttributes();

	Shader* shader;
	Texture* texture;
	GLuint vertexBuffer;
	vector<GLfloat> vertexData;
};


#endif SKYBOX_H

