#include "Skybox.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "OBJ_Loader.h"


Skybox::Skybox()
{
	shader = nullptr;
	texture = nullptr;
	vertexBuffer = 0;
}

Skybox::~Skybox()
{
	Cleanup();
	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}
}

void Skybox::Cleanup()
{
	if (vertexBuffer != 0)
	{
		glDeleteBuffers(1, &vertexBuffer);
		vertexBuffer = 0;
	}
}

void Skybox::Create(Shader* _shader, string _file, vector<string> _faces)
{
	shader = _shader;

	objl::Loader Loader;
	M_ASSERT(Loader.LoadFile(_file) == true, "Failed to load mesh");
	for (unsigned int i = 0; i < Loader.LoadedMeshes.size(); i++)
	{
		objl::Mesh curMesh = Loader.LoadedMeshes[i];
		for (unsigned int j = 0; j < curMesh.Vertices.size(); j++)
		{
			vertexData.push_back(curMesh.Vertices[j].Position.X);
			vertexData.push_back(curMesh.Vertices[j].Position.Y);
			vertexData.push_back(curMesh.Vertices[j].Position.Z);
		}
	}

	texture = new Texture();
	texture->LoadCubeMap(_faces);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void Skybox::SetShaderVariables(glm::mat4 _pv)
{
	shader->SetMat4("PV", _pv);
	shader->SetTextureSampler("skyboxTexture", GL_TEXTURE_CUBE_MAP, 0, texture->GetTexture());
}

void Skybox::BindAttributes()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(shader->GetAttrVertices());
	glVertexAttribPointer(shader->GetAttrVertices(),
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(float),
		(void*)0);
}

void Skybox::Render(glm::mat4 _pv)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(shader->GetProgramID());
	SetShaderVariables(_pv);
	BindAttributes();
	glDrawArrays(GL_TRIANGLES, 0, vertexData.size());
	glDisableVertexAttribArray(shader->GetAtrrNormals());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}
