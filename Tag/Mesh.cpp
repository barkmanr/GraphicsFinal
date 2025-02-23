#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "GameController.h"
#include <OBJ_Loader.h>
#include "ASEReader.h"

Mesh::Mesh()
{
	shader = nullptr;
	textureDiffuse = nullptr;
	textureSpecular = nullptr;
	textureNormal = nullptr;
	enableNormalMaps = false;
	vertexBuffer = 0;
	isClean = true;
	indexBuffer = 0;
	world = glm::mat4(1);
	position = { 0.0f,0.0f,0.0f };
	rotation = { 0.0f,0.0f,0.0f };
	scale = { 1.0f,1.0f,1.0f };
	color = { 1.0f,1.0f,1.0f };
	lightPosition = { 0.0f,0.0f,0.0f };
	lightColor = { 1.0f,1.0f,1.0f };
	lightDirection = { 0.0f,0.0f,0.0f };
	cameraPosition = { 0.0f,0.0f,0.0f };
	lastScaler = 1.0f; //start with one to not mess with scale
	deltaTime = 0.0f;
	lightType = 0;
	On = true;
	vertexStride = 0;
	instanceBuffer = 0;
	instanceCount = 0;
	enableInstanceing = false;
	specStrength = 4;
	specColor = { 1,1,1 };
}

Mesh::~Mesh() 
{ 
	Cleanup();
	if (textureDiffuse != nullptr) 
	{
		delete textureDiffuse;
		textureDiffuse = nullptr;
	}
	if (textureSpecular != nullptr)
	{
		delete textureSpecular;
		textureSpecular = nullptr;
	}
	if (textureNormal != nullptr)
	{
		delete textureNormal;
		textureNormal = nullptr;
	}
}

void Mesh::Cleanup()
{
	if (!isClean)
	{
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &indexBuffer);
		isClean = true;
	}
}

void Mesh::Create(Shader* _shader, string _filePath, string _fileName, int _instanceCount)
{
	FileName = _fileName;
	string _file = _filePath + _fileName;

	shader = _shader;
	enableInstanceing = _instanceCount > 1 ? true : false;
	instanceCount = _instanceCount;

	if (EndsWith(_file, "ase"))
	{
		LoadASE(_file);
	}
	else 
	{
		LoadOBJ(_file);
	}

	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (enableInstanceing)
	{
		glGenBuffers(1, &instanceBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		srand(glfwGetTime());
		for (unsigned int i = 0; i < instanceCount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100));

			model = glm::rotate(model, (float)(rand() % 360), glm::vec3(0, 1, 0));
			model = glm::rotate(model, (float)(rand() % 360), glm::vec3(1, 0, 0));
			model = glm::rotate(model, (float)(rand() % 360), glm::vec3(0, 0, 1));
			float s = (float)(rand() % 1000) / 500;
			model = glm::scale(model, glm::vec3(s,s,s));
			for (int x = 0; x < 4; x++)
			{
				for (int y = 0; y < 4; y++)
				{
					instanceData.push_back(model[x][y]);
				}
			}
		}
		glBufferData(GL_ARRAY_BUFFER, instanceCount * sizeof(glm::mat4), instanceData.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	vertexStride = 8;
	if (enableNormalMaps)
	{
		vertexStride += 6;
	}
}

///


void Mesh::Render(glm::mat4 _pv)
{
	glUseProgram(shader->GetProgramID());
	//rotation.y += 0.005f;

	CalculateTransform();
	SetShaderVariables(_pv);
	BindAttributes();
	if (enableInstanceing)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, m_vertexData.size() / vertexStride,instanceCount);
	}
	else 
	{
		glDrawArrays(GL_TRIANGLES, 0, m_vertexData.size()/vertexStride);
	}
	glDisableVertexAttribArray(shader->GetAttrVertices());
	glDisableVertexAttribArray(shader->GetAtrrNormals());
	glDisableVertexAttribArray(shader->GetAttrTexCoords());
	if (enableNormalMaps)
	{
		glDisableVertexAttribArray(shader->GetAttrTangents());
		glDisableVertexAttribArray(shader->GetAttrBitangents());
	}
	if (enableInstanceing)
	{
		glDisableVertexAttribArray(shader->GetAttrrInstanceMatrix());
		glDisableVertexAttribArray(shader->GetAttrrInstanceMatrix()+1);
		glDisableVertexAttribArray(shader->GetAttrrInstanceMatrix()+2);
		glDisableVertexAttribArray(shader->GetAttrrInstanceMatrix()+3);
	}
}

void Mesh::BindAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	#pragma region Vertices attribute buffer
	glEnableVertexAttribArray(shader->GetAttrVertices());
	glVertexAttribPointer //position? don't know what these do
	(
		shader->GetAttrVertices(),
		3, //size?
		GL_FLOAT,
		GL_FALSE, //is normazlized?
		vertexStride * sizeof(float), //stride: x,y,z,r,g,b,a skip for next vertex
		(void*)0
	);
	
	#pragma endregion
	//
	#pragma region normals attribute buffer
	glEnableVertexAttribArray(shader->GetAtrrNormals());
	glVertexAttribPointer(shader->GetAtrrNormals(),
		3, GL_FLOAT, GL_FALSE,
		vertexStride * sizeof(float),
		(void*)(3 * sizeof(float))
	);
	#pragma endregion
	//
	#pragma region texCoords attribute buffer
	glEnableVertexAttribArray(shader->GetAttrTexCoords());
	glVertexAttribPointer(
		shader->GetAttrTexCoords(),
		2, GL_FLOAT, GL_FALSE,
		vertexStride * sizeof(float),
		(void*)(6 * sizeof(float))
	);
	#pragma endregion

	#pragma region NormalMap
	if (enableNormalMaps)
	{
		glEnableVertexAttribArray(shader->GetAttrTangents());
		glVertexAttribPointer(shader->GetAttrTangents(),
			3,GL_FLOAT, GL_FALSE, vertexStride * sizeof(float),
			(void*)(8*sizeof(float)));

		glEnableVertexAttribArray(shader->GetAttrBitangents());
		glVertexAttribPointer(shader->GetAttrBitangents(),
			3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float),
			(void*)(11 * sizeof(float)));
	}
	#pragma endregion

	#pragma region BindInstanceData
	if (enableInstanceing)
	{
		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

		glEnableVertexAttribArray(shader->GetAttrrInstanceMatrix());
		glVertexAttribPointer(shader->GetAttrrInstanceMatrix(), 4, GL_FLOAT, GL_FALSE,
			sizeof(glm::mat4), (void*)0);

		glEnableVertexAttribArray(shader->GetAttrrInstanceMatrix() + 1);
		glVertexAttribPointer(shader->GetAttrrInstanceMatrix() + 1, 4, GL_FLOAT, GL_FALSE,
			sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));

		glEnableVertexAttribArray(shader->GetAttrrInstanceMatrix() + 2);
		glVertexAttribPointer(shader->GetAttrrInstanceMatrix() + 2, 4, GL_FLOAT, GL_FALSE,
			sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));

		glEnableVertexAttribArray(shader->GetAttrrInstanceMatrix() + 3);
		glVertexAttribPointer(shader->GetAttrrInstanceMatrix() + 3, 4, GL_FLOAT, GL_FALSE,
			sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(shader->GetAttrrInstanceMatrix(), 1);
		glVertexAttribDivisor(shader->GetAttrrInstanceMatrix() + 1, 1);
		glVertexAttribDivisor(shader->GetAttrrInstanceMatrix() + 2, 1);
		glVertexAttribDivisor(shader->GetAttrrInstanceMatrix() + 3, 1);
	}
	#pragma endregion
}

void Mesh::CalculateTransform()
{
	world = glm::mat4(1);
	world = glm::translate(world, position);
	world = glm::rotate(world, rotation.y, glm::vec3(0, 1, 0));
	world = glm::rotate(world, rotation.x, glm::vec3(1, 0, 0));
	world = glm::rotate(world, rotation.z, glm::vec3(0, 0, 1));
	world = glm::scale(world, scale);
}

void Mesh::SetShaderVariables(glm::mat4 _pv)
{
	shader->SetMat4("World", world);
	shader->SetMat4("WVP", _pv * world);
	shader->SetVec3("CameraPosition", cameraPosition);
	shader->SetInt("EnableNormalMaps", enableNormalMaps);
	shader->SetInt("EnableInstancing", enableInstanceing);

	vector<Mesh*>& lights = GameController::Instance().GetLights();
	for (int i = 0; i < lights.size(); i++)
	{
		shader->SetInt(Concat("light[", i, "].type").c_str(), lights[i]->GetLightType());
		shader->SetInt(Concat("light[", i, "].On").c_str(), (int)lights[i]->On);

		shader->SetVec3(Concat("light[", i, "].position").c_str(), lights[i]->GetPosition());
		if (lights[i]->GetLightType() == 3)
		{
			shader->SetVec3(Concat("light[", i, "].direction").c_str(), lights[i]->GetLightDirection() - lights[i]->GetPosition());
		}
		else if (lights[i]->GetLightType() == 1)
		{
			shader->SetVec3(Concat("light[", i, "].direction").c_str(), lights[i]->GetLightDirection() - lights[i]->GetPosition());
		}
		else 
		{
			shader->SetVec3(Concat("light[", i, "].direction").c_str(), lights[i]->GetLightDirection());
		}
	
		shader->SetVec3(Concat("light[", i, "].ambientColor").c_str(), { 0.1f,0.1f,0.1f });
		shader->SetVec3(Concat("light[", i, "].diffuseColor").c_str(), lights[i]->GetColor());
		shader->SetVec3(Concat("light[", i, "].specularColor").c_str(), lights[i]->specColor);

		shader->SetFloat(Concat("light[", i, "].constant").c_str(), 1.0f);
		shader->SetFloat(Concat("light[", i, "].linear").c_str(), 0.09f);
		shader->SetFloat(Concat("light[", i, "].quadratic").c_str(), 0.032f);

		shader->SetFloat(Concat("light[", i, "].coneAngle").c_str(), glm::radians(5.0f));
		shader->SetFloat(Concat("light[", i, "].falloff").c_str(), 200);
	}

	shader->SetFloat("material.specularStrength", specStrength);
	shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, textureDiffuse->GetTexture());
	shader->SetTextureSampler("material.specularTexture", GL_TEXTURE1, 1, textureSpecular->GetTexture());
	shader->SetTextureSampler("material.normalTexture", GL_TEXTURE2, 2, textureNormal->GetTexture());
}


void Mesh::UpdateVertex(float x, float y) //my stuff, need to fix for new triangle
{
	if (x > 0.0f) //D
	{
	}
	else if (x < 0.0f) //A
	{
	}
	else if (y > 0.0f) //W
	{
	}
	else if (y < 0.0f) //S
	{
	}
}


void Mesh::setBuffers()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexData.size() * sizeof(float), indexData.data(), GL_STATIC_DRAW);
}

string Mesh::Concat(const string& _s1, int _index, const string& _s2)
{
	string index = to_string(_index);
	return (_s1 + index + _s2);
}

string Mesh::RemoveFolder(string& _map)
{
	const size_t last_slash_idx = _map.find_last_of("\\/");
	if (string::npos != last_slash_idx)
	{
		_map.erase(0, last_slash_idx + 1);
	}
	return _map;
}

void Mesh::CalculateTangents(vector<objl::Vertex> _vertices, objl::Vector3& _tangent, objl::Vector3& _bitangent)
{
	objl::Vector3 edge1 = _vertices[1].Position - _vertices[0].Position;
	objl::Vector3 edge2 = _vertices[2].Position - _vertices[0].Position;
	objl::Vector2 deltaUV1 = _vertices[1].TextureCoordinate - _vertices[0].TextureCoordinate;
	objl::Vector2 deltaUV2 = _vertices[2].TextureCoordinate - _vertices[0].TextureCoordinate;

	float f = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV2.X * deltaUV1.Y);

	_tangent.X = f * (deltaUV2.Y * edge1.X - deltaUV1.Y * edge2.X);
	_tangent.Y = f * (deltaUV2.Y * edge1.Y - deltaUV1.Y * edge2.Y);
	_tangent.Z = f * (deltaUV2.Y * edge1.Z - deltaUV1.Y * edge2.Z);

	_bitangent.X = f * (-deltaUV2.X * edge1.X + deltaUV1.X * edge2.X);
	_bitangent.Y = f * (-deltaUV2.X * edge1.Y + deltaUV1.X * edge2.Y);
	_bitangent.Z = f * (-deltaUV2.X * edge1.Z + deltaUV1.X * edge2.Z);
}

void Mesh::LoadASE(string& _file)
{
	ASEReader reader;
	reader.ParseASEFile(_file.c_str());
	ASEReader::MeshInfo& m = reader.GeoObjects[0]->MeshI;
	ASEReader::Material* mat = reader.Materials[reader.GeoObjects[0]->MaterialID];

	vector<objl::Vector3> tangents;
	vector<objl::Vector3> bitangents;
	vector<objl::Vertex> triangle;
	objl::Vector3 tangent;
	objl::Vector3 bitangent;
	int vCount = 0;

	for (int count = 0; count < m.NumFaces; count++)
	{
		glm::vec3 tF = m.TexFaces[count];
		glm::vec3 f = m.Faces[count];
		triangle.clear();

		objl::Vertex vert = objl::Vertex();
		vert.Position = objl::Vector3(m.Vertices[(int)f.x].x, m.Vertices[(int)f.x].y, m.Vertices[(int)f.x].z);
		vert.Normal = objl::Vector3(m.VertexNormals[vCount].x, m.VertexNormals[vCount].y, m.VertexNormals[vCount].z);
		vert.TextureCoordinate = objl::Vector2(m.TexVertices[(int)tF.x].x, m.TexVertices[(int)tF.x].y);
		triangle.push_back(vert);
		vCount++;

		vert = objl::Vertex();
		vert.Position = objl::Vector3(m.Vertices[(int)f.y].x, m.Vertices[(int)f.y].y, m.Vertices[(int)f.y].z);
		vert.Normal = objl::Vector3(m.VertexNormals[vCount].x, m.VertexNormals[vCount].y, m.VertexNormals[vCount].z);
		vert.TextureCoordinate = objl::Vector2(m.TexVertices[(int)tF.y].x, m.TexVertices[(int)tF.y].y);
		triangle.push_back(vert);
		vCount++;

		vert = objl::Vertex();
		vert.Position = objl::Vector3(m.Vertices[(int)f.z].x, m.Vertices[(int)f.z].y, m.Vertices[(int)f.z].z);
		vert.Normal = objl::Vector3(m.VertexNormals[vCount].x, m.VertexNormals[vCount].y, m.VertexNormals[vCount].z);
		vert.TextureCoordinate = objl::Vector2(m.TexVertices[(int)tF.z].x, m.TexVertices[(int)tF.z].y);
		triangle.push_back(vert);
		vCount++;

		CalculateTangents(triangle, tangent, bitangent);
		tangents.push_back(tangent);
		bitangents.push_back(bitangent);

		for (int c = 0; c < 3; c++)
		{
			m_vertexData.push_back(triangle[c].Position.X);
			m_vertexData.push_back(triangle[c].Position.Y);
			m_vertexData.push_back(triangle[c].Position.Z);
			m_vertexData.push_back(triangle[c].Normal.X);
			m_vertexData.push_back(triangle[c].Normal.Y);
			m_vertexData.push_back(triangle[c].Normal.Z);
			m_vertexData.push_back(triangle[c].TextureCoordinate.X);
			m_vertexData.push_back(triangle[c].TextureCoordinate.Y);

			int index = (vCount / 3) - 1;
			m_vertexData.push_back(tangents[index].X);
			m_vertexData.push_back(tangents[index].Y);
			m_vertexData.push_back(tangents[index].Z);
			m_vertexData.push_back(bitangents[index].X);
			m_vertexData.push_back(bitangents[index].Y);
			m_vertexData.push_back(bitangents[index].Z);
		}
	}
	textureDiffuse = new Texture();
	if (mat->Maps[0].Name == "DIFFUSE")
	{
		textureDiffuse->LoadTexture("../Assets/Textures/" + RemoveFolder(mat->Maps[0].TextureFileName));	
	}
	textureSpecular = new Texture();
	if (mat->Maps[1].Name == "SPECULAR")
	{
		textureSpecular->LoadTexture("../Assets/Textures/" + RemoveFolder(mat->Maps[1].TextureFileName));
	}
	textureNormal = new Texture();
	if (mat->Maps[1].Name == "BUMP")
	{
		textureNormal->LoadTexture("../Assets/Textures/" + RemoveFolder(mat->Maps[1].TextureFileName));
		enableNormalMaps = true;
	}
	else if (mat->Maps[2].Name == "BUMP")
	{
		textureNormal->LoadTexture("../Assets/Textures/" + RemoveFolder(mat->Maps[2].TextureFileName));
		enableNormalMaps = true;
	}
}

void Mesh::LoadOBJ(string& _file)
{
	objl::Loader loader;
	M_ASSERT(loader.LoadFile(_file) == true, "Failed to load mesh");

	for (unsigned int i = 0; i < loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh curMesh = loader.LoadedMeshes[i];
		vector < objl::Vector3> tangents;
		vector<objl::Vector3> bitangents;
		vector<objl::Vertex> triangle;
		objl::Vector3 tangent;
		objl::Vector3 bitangent;
		for (unsigned int j = 0; j < curMesh.Vertices.size(); j += 3)
		{
			triangle.clear();
			triangle.push_back(curMesh.Vertices[j]);
			triangle.push_back(curMesh.Vertices[j + 1]);
			triangle.push_back(curMesh.Vertices[j + 2]);
			CalculateTangents(triangle, tangent, bitangent);
			tangents.push_back(tangent);
			bitangents.push_back(bitangent);
		}

		for (unsigned int j = 0; j < curMesh.Vertices.size(); j++)
		{
			m_vertexData.push_back(curMesh.Vertices[j].Position.X);
			m_vertexData.push_back(curMesh.Vertices[j].Position.Y);
			m_vertexData.push_back(curMesh.Vertices[j].Position.Z);
			m_vertexData.push_back(curMesh.Vertices[j].Normal.X);
			m_vertexData.push_back(curMesh.Vertices[j].Normal.Y);
			m_vertexData.push_back(curMesh.Vertices[j].Normal.Z);
			m_vertexData.push_back(curMesh.Vertices[j].TextureCoordinate.X);
			m_vertexData.push_back(curMesh.Vertices[j].TextureCoordinate.Y);

			if (loader.LoadedMaterials[0].map_bump != "")
			{
				int index = j / 3;
				m_vertexData.push_back(tangents[index].X);
				m_vertexData.push_back(tangents[index].Y);
				m_vertexData.push_back(tangents[index].Z);
				m_vertexData.push_back(bitangents[index].X);
				m_vertexData.push_back(bitangents[index].Y);
				m_vertexData.push_back(bitangents[index].Z);
			}
		}
	}
	textureDiffuse = new Texture();
	if (loader.LoadedMaterials[0].map_Kd != "")
	{
		textureDiffuse->LoadTexture("../Assets/Textures/" + RemoveFolder(loader.LoadedMaterials[0].map_Kd));
	}
	else
	{
		textureDiffuse->LoadTexture("../Assets/Textures/Tacos.png"); //backup
	}

	textureSpecular = new Texture();
	if (loader.LoadedMaterials[0].map_Ks != "")
	{
		textureSpecular->LoadTexture("../Assets/Textures/" + RemoveFolder(loader.LoadedMaterials[0].map_Ks));
	}

	textureNormal = new Texture();
	if (loader.LoadedMaterials[0].map_bump != "")
	{
		enableNormalMaps = true;
		textureNormal->LoadTexture("../Assets/Textures/" + RemoveFolder(loader.LoadedMaterials[0].map_bump));
	}
}

bool Mesh::EndsWith(const string& _str, const string& _suffix)
{
	return _str.size() >= _suffix.size() && 0 == _str.compare(_str.size() - _suffix.size(), _suffix.size(), _suffix);
}