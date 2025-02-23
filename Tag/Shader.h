#ifndef SHADER_H
#define SHADER_H

#include "StandardIncludes.h"

class Shader
{
public:
	Shader();
	virtual ~Shader();

	GLuint GetProgramID() { return programID; }
	GLuint GetAttrVertices() { return attrVertices; }
	GLuint GetAttrColors() { return atrrColors; }
	GLuint GetAtrrNormals() { return attrNormals; }
	GLuint GetAttrTexCoords() { return attrTexCoords; }
	GLuint GetAttrWVP() { return attrWVP; }
	GLuint GetAttrTangents() { return attrTangents; }
	GLuint GetAttrBitangents() { return attrBitangents; }
	GLuint GetAttrrInstanceMatrix() { return attrInstanceMatrix; }

	void LoadShaders(const char* _vertexFilePath, const char* _fragmentFilePath);
	void Cleanup();
	void SetInt(const char* _name, int _value);
	void SetVec3(const char* _name, glm::vec3 _value);
	void SetMat4(const char* _name, glm::mat4 _value);
	void SetFloat(const char* _name, float _value);
	void SetTextureSampler(const char* _name, GLuint _textUint, int _texUintId, int _value);

private:
	void CreateShaderProgram(const char* _vertexFilePath, const char* _fragmentFilePath);
	GLuint LoadShaderFile(const char* _filePath, GLenum _type);
	void LoadAttributes();
	void EvaluateShader(int _infoLength, GLuint _id);

	GLuint programID;
	GLuint attrVertices;
	GLuint atrrColors;
	GLuint attrNormals;
	GLuint attrTangents;
	GLuint attrBitangents;
	GLuint attrTexCoords;
	GLuint attrWVP;
	GLuint attrInstanceMatrix;
	GLint result;
	int infoLogLength;

};

#endif

