#include "GameController.h"
#include "WindowController.h"
#include "Mesh.h"
#include "Shader.h"
#include "ToolWindow.h"
#include "Camera.h"
#include "Skybox.h"
#include "Font.h"
#include "GameTime.h"
#include "PostProcessor.h"

GameController::GameController()
{
	shaderColor = nullptr;
	shaderDiffuse = nullptr;
	shaderFont = nullptr;
	shaderSkybox = nullptr;
	skybox = nullptr;
	curentCamera = 0;
	postProcessor = nullptr;
	shaderPost = nullptr;
	XPOS = 0;
	YPOS = 0;
	ship = nullptr;
	fish = nullptr;
	XPOS2 = 0;
	YPOS2 = 0;
	ZPOS2 = 0;
	MouseMoveL = false;
	MouseMoveM = false;
	arialFont = nullptr;
	Initialize();
}

GameController::~GameController()
{	
	if (ship == nullptr)
	{
		delete ship;
		ship = nullptr;
	}
	if (fish == nullptr)
	{
		delete fish;
		fish = nullptr;
	}
	for (Mesh* mesh : lights)
	{
		delete mesh;
	}
	lights.clear();

	if (skybox != nullptr)
	{
		delete skybox;
		skybox = nullptr;
	}
	if (postProcessor != nullptr)
	{
		delete postProcessor;
		postProcessor = nullptr;
	}

	if (shaderColor != nullptr)
	{
		delete shaderColor;
		shaderColor = nullptr;
	}
	if (shaderDiffuse != nullptr)
	{
		delete shaderDiffuse;
		shaderDiffuse = nullptr;
	}
	if (shaderFont != nullptr)
	{
		delete shaderFont;
		shaderFont = nullptr;
	}

	if (shaderSkybox != nullptr)
	{
		delete shaderSkybox;
		shaderSkybox = nullptr;
	}
	if (shaderPost != nullptr)
	{
		delete shaderPost;
		shaderPost = nullptr;
	}
	for (Camera* cam : cameras)
	{
		delete cam;
	}
	cameras.clear();
	if (arialFont != nullptr)
	{
		delete arialFont;
		arialFont = nullptr;
	}
}

void GameController::Initialize()
{
	GLFWwindow* window = WindowController::Instance().GetWindow();
	M_ASSERT(glewInit() == GLEW_OK, "Failed to initialize GLEW.");
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //RGBA
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	srand(time(0));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	cameras.push_back(new Camera(WindowController::Instance().GetResolution(), glm::vec3(0, 0, 5)));
	cameras.push_back(new Camera(WindowController::Instance().GetResolution(), glm::vec3(0, 0, 10)));
	curentCamera = 0;
}

void GameController::HandleKeys()
{
	if (glfwGetMouseButton(WindowController::Instance().GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		arialFont->RenderText("Left Button: Down", 40, 130, 0.25f, { 0.847f,0.749f,0.847f });
		if (Tag::ToolWindow::_light)
		{
			Mesh* affected;
			affected = lights[0];
			int x = 0;
			int y = 0;
			glfwGetWindowSize(WindowController::Instance().GetWindow(), &x, &y);
			glm::vec3 middlePos = { x / 2,y / 2,0 };
			glm::vec3 mouse = { XPOS - middlePos.x,-(YPOS - middlePos.y),0 };
			float length = glm::length(mouse);
			mouse = glm::normalize(mouse) * (float)GameTime::Instance().DeltaTime();

			if (length < 1)
			{
				mouse *= 0.5f;
			}
			else if (length > 3)
			{
				mouse *= 2;
			}
			affected->SetPosition(affected->GetPosition() + mouse);
		}
		else if (Tag::ToolWindow::_transform)
		{
			if (!MouseMoveL)
			{
				MouseMoveL = true;
				YPOS2 = YPOS;
				XPOS2 = XPOS;
			}
			float x = XPOS > XPOS2 ? 1 : -1;
			float y = YPOS > YPOS2 ? -1 : 1;
			x = XPOS - XPOS2 < 30 && XPOS - XPOS2 > -30  ? 0 : x;
			y = YPOS - YPOS2 < 30 && YPOS - YPOS2 > -30 ? 0 : y;
			glm::vec3 change = { x,y,0 };
			if (glm::length(change) != 0)
			{
				change = glm::normalize(change);
				change *= (float)GameTime::Instance().DeltaTime();

				if (Tag::ToolWindow::_translate)
				{
					ship->SetPosition(ship->GetPosition() + change);
				}
				if (Tag::ToolWindow::_rotate)
				{
					glm::vec3 rot = { change.y,change.x,0 };
					ship->SetRotation(ship->GetRotation() + rot);
				}
				if (Tag::ToolWindow::_scale)
				{
					ship->SetScale(ship->GetScale() + change * 0.001f);
				}
			}
		}
	}
	else
	{
		MouseMoveL = false;
		arialFont->RenderText("Left Button: Up", 40, 130, 0.25f, { 0.847f,0.749f,0.847f });
	}
	if (glfwGetMouseButton(WindowController::Instance().GetWindow(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		arialFont->RenderText("Left Button: Down", 40, 160, 0.25f, { 0.847f,0.749f,0.847f });
		if (Tag::ToolWindow::_transform)
		{
			if (!MouseMoveM)
			{
				MouseMoveM = true;
				ZPOS2 = YPOS;
			}
			float z = YPOS > ZPOS2 ? -1 : 1;
			z = YPOS - ZPOS2 < 30 && YPOS - ZPOS2 > -30 ? 0 : z;
			glm::vec3 change = { 0,0,z };
			if (glm::length(change) == 0) { return; }
			change *= (float)GameTime::Instance().DeltaTime();

			if (Tag::ToolWindow::_translate)
			{
				ship->SetPosition(ship->GetPosition() + change);
			}
			if (Tag::ToolWindow::_rotate)
			{
				ship->SetRotation(ship->GetRotation() + change);
			}
			if (Tag::ToolWindow::_scale)
			{
				ship->SetScale(ship->GetScale() + change * 0.001f);
			}
		}
	}
	else 
	{
		arialFont->RenderText("Left Button: Up", 40, 160, 0.25f, { 0.847f,0.749f,0.847f });
		MouseMoveM = false;
	}

}


void GameController::RunGame()
{
	Tag::ToolWindow^ wind = gcnew Tag::ToolWindow(); //Side window
	wind->Show();

	shaderColor = new Shader();
	shaderColor->LoadShaders("Color.vertexshader", "Color.fragmentshader");
	
	shaderDiffuse = new Shader();
	shaderDiffuse->LoadShaders("Diffuse.vertexshader", "Diffuse.fragmentshader");

	shaderFont = new Shader();
	shaderFont->LoadShaders("Font.vertexshader", "Font.fragmentshader");

	shaderSkybox = new Shader();
	shaderSkybox->LoadShaders("Skybox.vertexshader", "Skybox.fragmentshader");

	shaderPost = new Shader();
	shaderPost->LoadShaders("PostProcessor.vertexshader", "PostProcessor.fragmentshader");

	postProcessor = new PostProcessor();
	postProcessor->Create(shaderPost);

	Mesh* light = new Mesh();
	light->Create(shaderColor, "../Assets/Models/","Sphere.obj");
	light->SetPosition({ 0.0f,0.0f,2.0f });
	light->SetColor({ 1.0f,1.0f,1.0f });
	light->SetScale({ 0.1f,0.1f,0.1f });
	light->SetLightType(4);
	lights.push_back(light);
	
	ship = new Mesh();
	ship->Create(shaderDiffuse, "../Assets/Models/","Fighter.ase",1);
	SetShip();

	fish = new Mesh();
	fish->Create(shaderDiffuse, "../Assets/Models/", "fish.ase", 100);
	fish->SetScale({ 0.25f,0.25f,0.25f });
	fish->SetPosition({ 0.0f,0.0f,0.0f });
	fish->SetRotation({ 0,0,0 });
	
	skybox = new Skybox();
	skybox->Create(shaderSkybox, "../Assets/Models/Skybox.obj",
		{
			"../Assets/Textures/Skybox/right.png",
			"../Assets/Textures/Skybox/left.png",
			"../Assets/Textures/Skybox/top.png",
			"../Assets/Textures/Skybox/bottom.png",
			"../Assets/Textures/Skybox/front.png",
			"../Assets/Textures/Skybox/back.png"
		});


	arialFont = new Font();
	arialFont->Create(shaderFont, "../Assets/Fonts/arial.ttf", 100);

	
	GameTime::Instance().Initialize();
	GLFWwindow* window = WindowController::Instance().GetWindow();
	do
	{
		System::Windows::Forms::Application::DoEvents(); //CLI
		glfwGetCursorPos(window, &XPOS, &YPOS);

		GameTime::Instance().Update();

		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );

		if (Tag::ToolWindow::_light || Tag::ToolWindow::_transform)
		{
			TransformText();
			if (Tag::ToolWindow::_light)
			{
				for (Mesh* light : lights)
				{
					light->Render(cameras[curentCamera]->GetProjection() * cameras[curentCamera]->GetView()); //p * v * w
				}
				glm::vec3 rotation = {1, 0, 0 };
				ship->SetRotation(ship->GetRotation() + (rotation * (float)GameTime::Instance().DeltaTime()));
			}
			ship->Render(cameras[curentCamera]->GetProjection() * cameras[curentCamera]->GetView());
		}
		else if (Tag::ToolWindow::_water)
		{
			postProcessor->Start();
			fish->SetInstanceCount(1);
			fish->SetPosition({0,0,0});
			postProcessor->setLine(Tag::ToolWindow::_wireframe);
			postProcessor->setState(Tag::ToolWindow::_blue);
			fish->Render(cameras[curentCamera]->GetProjection() * cameras[curentCamera]->GetView());
			postProcessor->End(Tag::ToolWindow::F, Tag::ToolWindow::A, (float)glfwGetTime());
		}
		else if (Tag::ToolWindow::_space)
		{
			fish->SetInstanceCount(100);
			ship->SetPosition({ 0,0,0 });
			fish->SetPosition(cameras[1]->GetPosition());
			cameras[1]->Rotate((float)GameTime::Instance().DeltaTime());
			glm::mat4 view = glm::mat4(glm::mat3(cameras[1]->GetView()));
			skybox->Render(cameras[1]->GetProjection() * view);
			fish->Render(cameras[1]->GetProjection() * cameras[1]->GetView());
			glm::vec3 rotation = { 1, 0, 0 };
			ship->SetRotation(ship->GetRotation() + (rotation * (float)GameTime::Instance().DeltaTime()));
			ship->Render(cameras[1]->GetProjection() * cameras[1]->GetView());
		}

		arialFont->RenderText("Ryan Barkman", 40, 40, 0.25f, { 0.847f,0.749f,0.847f }); 
		arialFont->RenderText(to_string(GameTime::Instance().Fps()), 40, 70, 0.25f, { 0.847f,0.749f,0.847f });
		ostringstream oss;
		oss << "Mouse Pos: (" << XPOS << " " << YPOS << ")";
		arialFont->RenderText(oss.str(), 40, 100, 0.25f, { 0.847f,0.749f,0.847f });

		HandleKeys(); //my own input

		glfwSwapBuffers(window); //swap the front and back
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	postProcessor->Cleanup();
	for (Mesh* light : lights)
	{
		light->Cleanup();
	}
	fish->Cleanup();
	ship->Cleanup();
	skybox->Cleanup();
	shaderPost->Cleanup();
	shaderFont->Cleanup();
	shaderDiffuse->Cleanup();
	shaderColor->Cleanup();
	shaderSkybox->Cleanup();
}

void GameController::SetShip()
{
	ship->SetScale({ 0.002f,0.002f,0.002f });
	ship->SetPosition({ 0.0f,0.0f,0.0f });
	ship->SetRotation({ 0,0,0 });
}

void GameController::TransformText()
{
	arialFont->RenderText("Fighter Position: {" + 
		to_string(ship->GetPosition().x) + ", " + 
		to_string(ship->GetPosition().y) + ", " + 
		to_string(ship->GetPosition().z) + "}",
		40, 190, 0.25f, { 0.847f,0.749f,0.847f });

	arialFont->RenderText("Fighter Rotation: {" +
		to_string(ship->GetRotation().x) + ", " +
		to_string(ship->GetRotation().y) + ", " +
		to_string(ship->GetRotation().z) + "}",
		40, 220, 0.25f, { 0.847f,0.749f,0.847f });

	arialFont->RenderText("Fighter Scale: {" +
		to_string(ship->GetScale().x) + ", " +
		to_string(ship->GetScale().y) + ", " +
		to_string(ship->GetScale().z) + "}",
		40, 250, 0.25f, { 0.847f,0.749f,0.847f });
}
