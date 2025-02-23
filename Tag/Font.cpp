#include "Font.h"
#include "Shader.h"
#include "WindowController.h"

Font::Font()
{
	shader = nullptr;
	library = nullptr;
	face = nullptr;
	vertexBuffer = 0;
	orthoProj = {};
}

Font::~Font()
{

}

void Font::Create(Shader* _shader, string _name, FT_UInt _size)
{
	shader = _shader;
	AllocateBuffers();
	Initialize(_name, _size);
}

void Font::AllocateBuffers()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
}

void Font::Initialize(string _fileNmae, FT_UInt _size)
{
	Resolution r = WindowController::Instance().GetResolution();
	orthoProj = glm::ortho(0.0f, (float)r.width, 0.0f, (float)r.height);

	M_ASSERT(FT_Init_FreeType(&library) == false, "Could not init FreeType Library");
	M_ASSERT(FT_New_Face(library, _fileNmae.c_str(), 0, &face) == false, "Failed to load font");
	M_ASSERT(FT_Set_Pixel_Sizes(face, 0, _size) == false, "Failed to set character size");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	CreateCharacters();

	FT_Done_Face(face);
	FT_Done_FreeType(library);
}

void Font::CreateCharacters()
{
	for (unsigned char c = 0; c < 128; c++)
	{
		M_ASSERT(FT_Load_Char(face, c, FT_LOAD_RENDER) == false, "Failed to load Glyph");
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int)face->glyph->advance.x};
		characters.insert(pair<char, Character>(c, character));
	}
}

void Font::RenderText(string _text, float _x, float _y, float _scale, glm::vec3 _color)
{
	_y = WindowController::Instance().GetResolution().height - _y;
	glUseProgram(shader->GetProgramID());
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	shader->SetVec3("textColor", _color);
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(orthoProj));

	string::const_iterator c;
	for (c = _text.begin(); c != _text.end(); c++)
	{
		Character ch = characters[*c];
		float xpos = _x + ch.bearing.x * _scale;
		float ypos = _y - (ch.size.y - ch.bearing.y) * _scale;
		float w = ch.size.x * _scale;
		float h = ch.size.y * _scale;

		float vertices[6][4] = {
			{xpos, ypos + h, 0.0f,0.0f},
			{xpos, ypos, 0.0f,1.0f},
			{xpos + w,ypos, 1.0f,1.0f},
			{xpos, ypos + h,0.0f,0.0f},
			{xpos + w,ypos, 1.0f,1.0f},
			{xpos + w,ypos + h,1.0f,0.0f}
		};

		shader->SetTextureSampler("material.diffuseTexture", GL_TEXTURE0, 0, ch.textureID);

		glEnableVertexAttribArray(shader->GetAttrVertices());
		glVertexAttribPointer(shader->GetAttrVertices(), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		_x += (ch.advance >> 6) * _scale;
	}
	glDisableVertexAttribArray(shader->GetAttrVertices());
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void Font::Cleanup()
{
	glDeleteBuffers(1, &vertexBuffer);
	for(auto it : characters)
	{
		glGenTextures(1, &(it.second.textureID));
	}
}