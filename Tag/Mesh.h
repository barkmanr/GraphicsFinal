#ifndef MESH_H
#define MESH_H

#include "StandardIncludes.h"
#include "OBJ_Loader.h"

class Texture;
class Shader;

class Mesh //Shape
{
public:
	Mesh();
	virtual ~Mesh();

	bool getClean() { return isClean; }

	void Create(Shader* _shader, string _filePath, string _fileName, int _instanceCount = 1);
	void Cleanup();

	void UpdateVertex(float x, float y); //bonus
	void setBuffers();

	void SetPosition(glm::vec3 _position) { position = _position; }
	glm::vec3 GetPosition() { return position; }
	void SetRotation(glm::vec3 _rotation) { rotation = _rotation; }
	glm::vec3 GetRotation() { return rotation; }
	void SetScale(glm::vec3 _scale) { scale = _scale; }
	glm::vec3 GetScale() { return scale; }
	void SetLightPosition(glm::vec3 _lightPosition) { lightPosition = _lightPosition; }
	void SetLightColor(glm::vec3 _lightColor) { lightColor = _lightColor; }
	void SetCameraPosition(glm::vec3 _cameraPosition) { cameraPosition = _cameraPosition; }
	void SetColor(glm::vec3 _color) { color = _color; }
	glm::vec3 GetColor() { return color; }
	void SetLightDirection(glm::vec3 _lightDirection) { lightDirection = _lightDirection; }
	glm::vec3 GetLightDirection() { return lightDirection; }
	int GetLightType() {return lightType;}
	string getLightType() { if (lightType == 1) { return "Directional"; } if (lightType == 2) { return "Point"; } if (lightType == 3) { return "Spot"; } return "Extra"; }
	void SetLightType(int type) { lightType = type; }
	void SetOn(bool type) { On = type; }
	string GetFile() { return FileName; }
	void ChangeEnabled(bool value) { enableNormalMaps = value; }
	void SetInstancing(bool value) { enableInstanceing = value; }
	int GetInstanceCount() { return instanceCount; }
	void SetInstanceCount(int num) { instanceCount = num; enableInstanceing = num > 1 ? true : false;}

	void SetSpec(int _strength) { specStrength = _strength; }
	void SetSpecColor(glm::vec3 color) { specColor = color; }
	

	void Render(glm::mat4 _vp);
	void CalculateTransform();

private:
	void LoadOBJ(string& _file);
	void LoadASE(string& _file);
	void SetShaderVariables(glm::mat4 _pv);
	void BindAttributes();
	string Concat(const string& _s1, int _index, const string& _s2);
	string RemoveFolder(string& _map);
	void CalculateTangents(vector<objl::Vertex> _vertices, objl::Vector3& _tangent, objl::Vector3& _bitangent);
	bool EndsWith(const string& _str, const string& _suffix);

	Shader* shader;
	Texture* textureDiffuse;
	Texture* textureSpecular;
	Texture* textureNormal;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint instanceBuffer;
	vector<GLfloat> m_vertexData; //Points
	vector<GLubyte> indexData;
	vector<GLfloat> instanceData;
	bool enableNormalMaps;
	int vertexStride;
	int instanceCount;
	bool enableInstanceing;

	glm::mat4 world;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 color;
	glm::vec3 lightPosition;
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 cameraPosition;

	int lightType; //1=directional, 2=point, 3=spot
	bool On;
	string FileName;

	int specStrength;
	glm::vec3 specColor;

	bool isClean;
	float lastScaler;
	float deltaTime;
};

#endif MESH_H

