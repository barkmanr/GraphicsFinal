#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
class Mesh;
class Shader;
class Camera;
class Skybox;
class PostProcessor;
class Font;

class GameController : public Singleton<GameController>
{
public:
	GameController();
	virtual ~GameController();
	void HandleKeys();

	void RunGame();
	vector<Mesh*>& GetLights() { return lights; }
	const Camera* GetCamera() { return cameras[curentCamera]; }
	Mesh* GetShip() { return ship; }
	void SetShip();
private:
	void Initialize();
	void TransformText();
	Shader* shaderColor;
	Shader* shaderDiffuse;
	Shader* shaderFont;
	Shader* shaderSkybox;
	Shader* shaderPost;

	vector<Mesh*> lights;
	Mesh* ship;
	Mesh* fish;
	Skybox* skybox;
	PostProcessor* postProcessor;
	GLuint vao;

	vector<Camera*> cameras;
	int curentCamera;

	double XPOS;
	double YPOS;

	double XPOS2;
	double YPOS2;
	double ZPOS2;
	bool MouseMoveL; //Left
	bool MouseMoveM; //Middle
	Font* arialFont;
};

#endif GAME_CONTROLLER_H