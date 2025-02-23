#include "PostProcessor.h"
#include "WindowController.h"
#include "Shader.h"

PostProcessor::PostProcessor() 
{
	framebuffer = 0;
	textureColorbuffer = 0;
	renderBufferObject = 0;
	vertexBuffer = 0;
	postShader = nullptr;
	state = static_cast<State>(1);
	isline = false;
}

void PostProcessor::Cleanup()
{
	if (framebuffer == 0) { return; }
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &textureColorbuffer);
	glDeleteRenderbuffers(1, &renderBufferObject);
}

void PostProcessor::Create(Shader* shade) 
{
	postShader = shade;
	CreateBuffers();
	CreateVertices();
}

void PostProcessor::CreateBuffers()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	Resolution r = WindowController::Instance().GetResolution();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, r.width, r.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &renderBufferObject);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, r.width, r.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);

	M_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::CreateVertices()
{
	float vertexData[] = {
		-1.0f,1.0f,		0.0f,1.0f,
		-1.0f,-1.0f,	0.0f,0.0f,
		1.0f, -1.0f,	1.0f,0.0f,

		-1.0f, 1.0f,	0.0f,1.0f,
		1.0f,-1.0f,		1.0f,0.0f,
		1.0f,1.0f,		1.0f,1.0f
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PostProcessor::BindVertices()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(postShader->GetAttrVertices());
	glVertexAttribPointer(postShader->GetAttrVertices(),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(postShader->GetAttrTexCoords());
	glVertexAttribPointer(postShader->GetAttrTexCoords(),
		2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void PostProcessor::Start()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void PostProcessor::End(int freq, int amp, float time)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	glUseProgram(postShader->GetProgramID());
	postShader->SetTextureSampler("screenTexture", GL_TEXTURE0, 0, textureColorbuffer);
	BindVertices();
	postShader->SetInt("State", (int)state);
	postShader->SetFloat("Frequency", (float)freq/100.0f);
	postShader->SetFloat("Amplitude", (float)amp/100.0f);
	postShader->SetFloat("Time", time);
	if (isline)
	{
		glLineWidth(5.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else 
	{
		glLineWidth(5.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(postShader->GetAttrVertices());
	glDisableVertexAttribArray(postShader->GetAttrTexCoords());
	glEnable(GL_DEPTH_TEST);//added this

}

void PostProcessor::updateState()
{
	int i = state;
	i++;
	if (i > 2) { i = 1; }
	state = static_cast<State>(i);
}
void PostProcessor::updateState2()
{
	int i = state;
	i--;
	if (i < 1) { i = 2; }
	state = static_cast<State>(i);
}

string PostProcessor::getState()
{
	switch (state)
	{
	case Normal:
		return "Normal";
		break;
	case Blue:
		return "Blue";
		break;
	default:
		return "Error";
		break;
	}
}


