#include "ASEReader.h"
#include <algorithm>

void ASEReader::ParseASEFile(const char* _fileName)
{
	vector<string> ASEFile;
	ReadFile(_fileName, ASEFile);

	ParseMaterials(ASEFile);
	ParseGeoObjects(ASEFile);
}

void ASEReader::ReadFile(const char* _fileName, vector<string>& _file)
{
	string line;
	ifstream file(_fileName);
	while (getline(file, line))
	{
		_file.push_back(line);
	}
	file.close();
}

void ASEReader::ParseMaterials(vector<string>& _file)
{
	vector<string> _materials;
	GetBlock("*MATERIAL_LIST ", _file, _materials);

	int materialCount = stoi(ExtractValue(_materials, "*MATERIAL_COUNT"));
	for (int count = 0; count < materialCount; count++) 
	{
		string materialTag = string("*MATERIAL ") + to_string(count);
		vector<string> _materialBlock;
		GetBlock(materialTag.c_str(), _materials, _materialBlock);

		Material* material = new Material();

		material->Index = count;
		material->Name = ExtractValue(_materialBlock, "*MATERIAL_NAME ");
		material->Class = ExtractValue(_materialBlock, "*MATERIAL_CLASS ");
		material->AmbientColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_AMBIENT "));
		material->DiffuseColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_DIFFUSE "));
		material->SpecularColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_SPECULAR "));
		material->Shine = stof(ExtractValue(_materialBlock, "*MATERIAL_SHINE "));
		material->ShineStrength = stof(ExtractValue(_materialBlock, "*MATERIAL_SHINESTRENGTH "));
		material->Transparancy = stof(ExtractValue(_materialBlock, "*MATERIAL_TRANSPARENCY "));
		material->WireSize = stof(ExtractValue(_materialBlock, "*MATERIAL_WIRESIZE "));
		material->Shading = ExtractValue(_materialBlock, "*MATERIAL_SHADING ");

		ParseMap("*MAP_DIFFUSE", "DIFFUSE", _materialBlock, material);
		ParseMap("*MAP_SPECULAR", "SPECULAR", _materialBlock, material);
		ParseMap("*MAP_BUMP", "BUMP", _materialBlock, material);

		Materials.push_back(material);

	}
}

void ASEReader::ParseGeoObjects(vector<string>& _file)
{
	vector<string> _geomObject;
	int index = GetBlock("*GEOMOBJECT ", _file, _geomObject);

	while (index != -1)
	{
		GeoObject* g = new GeoObject();

		g->Name = ExtractValue(_geomObject, "*NODE_NAME ");
		g->MaterialID = stoi(ExtractValue(_geomObject, "*MATERIAL_REF"));
		g->MeshI.TimeValue = stoi(ExtractValue(_geomObject, "*TIMEVALUE "));
		g->MeshI.NumVertex = stoi(ExtractValue(_geomObject, "*MESH_NUMVERTEX "));
		g->MeshI.NumFaces = stoi(ExtractValue(_geomObject, "*MESH_NUMFACES "));

		vector<string> dataBlock;

		FindAllTags("*MESH_VERTEX ", _geomObject, dataBlock);
		ExtractVectorList(g->MeshI.Vertices, dataBlock);

		FindAllTags("*MESH_TVERT ", _geomObject, dataBlock);
		ExtractVectorList(g->MeshI.TexVertices, dataBlock);

		FindAllTags("*MESH_TFACE ", _geomObject, dataBlock);
		ExtractVectorList(g->MeshI.TexFaces, dataBlock);

		FindAllTags("*MESH_VERTEXNORMAL ", _geomObject, dataBlock);
		ExtractVectorList(g->MeshI.VertexNormals, dataBlock);

		FindAllTags("*MESH_FACE ", _geomObject, dataBlock);
		for (auto& face : dataBlock)
		{
			replace(face.begin(), face.end(), '\t', ' ');
			face.erase(0, face.find_first_not_of(' '));

			vector<string> result;
			stringstream ss(face);
			string item = "";
			while (getline(ss, item, ' '))
			{
				if (item != "")
				{
					result.push_back(item);
				}
			}
			g->MeshI.Faces.push_back(glm::vec3{ stof(result[3]), stof(result[5]), stof(result[7]) });
		}
		GeoObjects.push_back(g);
		index = GetBlock("*GEOMOBJECT ", _file, _geomObject, index + 1);
	}
}

int ASEReader::GetBlock(const char* _tag, vector<string>& _lines, vector<string>& _block, int _start)
{
	_block.clear();
	for (int i = _start; i < _lines.size(); i++)
	{
		if (_lines[i].find(_tag) != string::npos)
		{
			int indent = 0;
			i++;
			for (int j = i; j < _lines.size(); j++)
			{
				if (_lines[j].find('{') != string::npos) { indent++; }
				if (_lines[j].find('}') != string::npos)
				{
					if (indent == 0) return i;
					indent--;
				}
				_block.push_back(_lines[j]);
			}
		}
	}
	return -1;
}

string ASEReader::ExtractValue(vector<string>& _lines, const char* _tag)
{
	for (int i = 0; i < _lines.size(); i++)
	{
		if (_lines[i].find(_tag) != string::npos)
		{
			string line = _lines[i];

			replace(line.begin(), line.end(), '\t', ' ');
			line.erase(0, line.find_first_not_of(" \n\r\t"));
			line.erase(0, line.find(" ") + 1);
			line.erase(remove(line.begin(), line.end(), '\"'), line.end());
			line.erase(0, line.find_first_not_of(" \n\r\t"));

			return line;
		}
	}
	return string();
}

void ASEReader::FindAllTags(const char* _tag, vector<string>& _lines, vector<string>& _block)
{
	_block.clear();
	for (int i = 0; i < _lines.size(); i++)
	{
		if (_lines[i].find(_tag) != string::npos)
		{
			_block.push_back(_lines[i]);
		}
	}
}

glm::vec3 ASEReader::ParseStringToVec3(string _values)
{
	vector<string> result;
	stringstream ss(_values);
	string item = "";

	while (getline(ss, item, ' '))
	{
		result.push_back(item);
	}
	return glm::vec3{ stof(result[0]), stof(result[1]), stof(result[2]) };
}

void ASEReader::ParseMap(const char* _mapName, const char* _name, vector<string>& _materialBlock, ASEReader::Material* _material)
{
	vector<string> _mapBlock;
	GetBlock(_mapName, _materialBlock, _mapBlock);

	if (_mapBlock.size() > 0)
	{
		ASEReader::Map map;
		map.Name = _name;
		map.TextureFileName = ExtractValue(_mapBlock, "*BITMAP ");
		map.UVW_U_Offset = stof(ExtractValue(_mapBlock, "UVW_U_OFFSET "));
		map.UVW_V_Offset = stof(ExtractValue(_mapBlock, "UVW_V_OFFSET "));
		map.UVW_U_Tiling = stof(ExtractValue(_mapBlock, "UVW_U_TILING "));
		map.UVW_V_Tiling = stof(ExtractValue(_mapBlock, "UVW_V_TILING "));
		_material->Maps.push_back(map);
	}
}

void ASEReader::ExtractVectorList(vector<glm::vec3>& _list, vector<string>& _vertexList)
{
	for (auto& vertex : _vertexList)
	{
		replace(vertex.begin(), vertex.end(), '\t', ' ');
		vertex.erase(0, vertex.find_first_not_of(' '));

		vector<string> result;
		stringstream ss(vertex);
		string item = "";

		while (getline(ss, item, ' '))
		{
			if (item != "")
			{
				result.push_back(item);
			}
		}
		_list.push_back(glm::vec3{ stof(result[2]), stof(result[3]), stof(result[4]) });
	}
}

 