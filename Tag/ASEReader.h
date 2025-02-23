#ifndef ASE_READER
#define ASE_READER

#include "StandardIncludes.h"

class ASEReader
{
public:
	struct Map
	{
		string Name;
		string TextureFileName;
		float UVW_U_Offset;
		float UVW_V_Offset;
		float UVW_U_Tiling;
		float UVW_V_Tiling;
	};
	struct Material
	{
		int Index;
		string Name;
		string Class;
		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;
		float Shine;
		float ShineStrength;
		float Transparancy;
		float WireSize;
		string Shading;
		float XP_Falloff;
		float SelfIllum;
		string Falloff;
		string XP_Type;
		vector<ASEReader::Map> Maps;
	};

	struct MeshInfo
	{
		int TimeValue;
		int NumVertex;
		vector<glm::vec3> Vertices;
		int NumFaces;
		vector<glm::vec3> Faces;
		int NumTexVertex;
		vector<glm::vec3> TexVertices;
		int NumTVFaces;
		vector<glm::vec3> TexFaces;
		vector<glm::vec3> VertexNormals;
	};

	struct GeoObject 
	{
		string Name;
		MeshInfo MeshI;
		int MaterialID;
	};

	void ParseASEFile(const char* _fileName);

	string FileName;
	vector<Material*> Materials;
	vector<GeoObject*> GeoObjects;

private:
	void ParseMaterials(vector<string>& _file);
	void ParseGeoObjects(vector<string>& _file);
	void ReadFile(const char* _fileName, vector<string>& _filename);

	int GetBlock(const char* _tag, vector<string>& _lines, vector<string>& _block, int _start = 0);
	void FindAllTags(const char* _tag, vector<string>& _lines, vector<string>& _block);
	string ExtractValue(vector<string>& _lines, const char* _tag);
	glm::vec3 ParseStringToVec3(string _values);
	void ParseMap(const char* _mapName, const char* _name, vector<string>& _materialBlock, ASEReader::Material* _material);
	void ExtractVectorList(vector<glm::vec3>& _list, vector<string>& _vertexList);
};

#endif ASE_READER

