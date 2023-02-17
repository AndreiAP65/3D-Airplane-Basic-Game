#include "Game.h"
#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
float rotatieocean = 0;
int firstperson = 0;
int thirdperson = 0;
float nivelcombustibil = 2;
int colisiondetected1 = 0;
int colisiondetected2 = 0;
float rotationangle = 0;
int miscare = 0;
float mousemove = 0;
float unghiinclinare = 0;
float planepos ;
float unghiinclinarelaterala = 0;
int lifepoints = 3;
int contor;
float rotatieobstacole = 0;
float translatienorispate = -1;
int numarnori = 25;
float norX[25] = { -9 };
float norY[25] ;
float norrandX[25][3];
float norrandY[25][3];
float norrandrot[25][3];
float norrandscale[25][3];
float rotationnori = 0;
int acceleration = 0;
float speed = 3;
float obstacol1X=-10;
float obstacol1Y = (float)rand() / (float)(RAND_MAX / 2);
float obstacol2X=0;
float obstacol2Y = (float)rand() / (float)(RAND_MAX / -2);
int coliziuneobstacol1 = 0;
int coliziuneobstacol2 = 0;
int coliziunecombustibil1 = 0;
int coliziunecombustibil2 = 0;
int coliziunecombustibil3 = 0;
int coliziunecombustibil4 = 0;
int coliziunecombustibil5 = 0;
float extraviataX = -5;
float extraviataY = (float)rand() / (float)(RAND_MAX / 2);
int coliziuneviata = 0;
float combustibilX = -10;
float combustibilY = (float)rand() / (float)(RAND_MAX / 5);
float fallonY = 0;
float fallonX = 0;
float fallangle = 0;

Game::Game()
{
}

Game::~Game()
{
}



void Game::Init()
{
	renderCameraTarget = false;

	camera = new Laborator::Camera();

	camera->Set(glm::vec3(0, 2, 4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;

	}
	{
		Mesh* mesh = new Mesh("teapot");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
		meshes[mesh->GetMeshID()] = mesh;

	}
	{
		Mesh* mesh = new Mesh("ocean");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "ocean.obj");
		meshes[mesh->GetMeshID()] = mesh;

	}

	{
		Shader* shader = new Shader("Shader1");
		shader->AddShader("Source/Laboratoare/Game/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Game/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;

	}
	{
		Shader* shader2 = new Shader("Shader2");
		shader2->AddShader("Source/Laboratoare/Game/Shaders/VertexShaderGUI.glsl", GL_VERTEX_SHADER);
		shader2->AddShader("Source/Laboratoare/Game/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader2->CreateAndLink();
		shaders[shader2->GetName()] = shader2;
	}

	{
		Shader* shader3 = new Shader("Shader3");
		shader3->AddShader("Source/Laboratoare/Game/Shaders/VertexShaderOcean.glsl", GL_VERTEX_SHADER);
		shader3->AddShader("Source/Laboratoare/Game/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader3->CreateAndLink();
		shaders[shader3->GetName()] = shader3;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(1, 6,3);
		materialShininess = 50;
		materialKd = 50;
		materialKs = 1;
	}

	projectionMatrix = glm::perspective(RADIANS(90), window->props.aspectRatio, 0.01f, 200.0f);
}

void Game::FrameStart()
{	
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0.74f, 0.93f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Game::Update(float deltaTimeSeconds)
{
	//CORPUL AVIONULUI
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, mousemove, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil == 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, mousemove, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.3f, 0.25f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(1,1, 1));
	}
	//CONEXIUNE CORP-ELICE
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove + 0, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.19f));
		RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(1, 0, 0));
	}
	//ARIPA PRINCIPALA PLUS TOP
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.23f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove + 0, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.05f, 0.7f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.169f, 0.169f, 0.169f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove + 0.1f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.23f, -0.1f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, 0.1f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove + 0.1f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.05f, 1));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	//COADA
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, mousemove + 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(+0.4f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, mousemove + 0, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.05f, 0.1f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
	
		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, mousemove + 0.15f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.35f, -0.15f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, 0.15f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, mousemove + 0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.05f, 0.7f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, mousemove + 0.2f, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.35f, -0.2f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, 0.2f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.35f, mousemove + 0.2f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.25f, 0.1f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}

	//ACOPERIS
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		
		if (lifepoints > 0 && nivelcombustibil > 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, mousemove + 0.2f, 0.07f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2f, -0.2f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, 0.2f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, mousemove + 0.2f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.25f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, mousemove + 0.2f, -0.07f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2f, -0.2f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, 0.2f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, mousemove + 0.2f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.25f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove + 0.33f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.23f, -0.33f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, 0.33f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.23f, mousemove + 0.33f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.01f, 0.7f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.255f, 0.228f, 0.181f));
	}

	//ROTI
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove - 0.15f, -0.11f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, 0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, -0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove -0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08f));
		RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(0, 0, 0));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove - 0.15f, 0.11f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.4f, 0.15f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, -0.15f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.4f, mousemove -0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08f));
		RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(0, 0, 0));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2f, mousemove + 0.15f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2f, -0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2f, 0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2f, mousemove + 0.15f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f, 0.1f, 0.2f));
		RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(0, 0, 0));
	}
	
	//ELICE
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));

		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.05f, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotationangle), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.35f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.05f, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::rotate(modelMatrix, RADIANS(45 + rotationangle), glm::vec3(1,0,0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.35f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.05f, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90 + rotationangle), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.35f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
		if (lifepoints > 0 && nivelcombustibil > 0) {
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove+0.05f, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinare), glm::vec3(0, 0, 1));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 0.05f, 0));
		}
		else if (lifepoints == 0 || nivelcombustibil <= 0 ){
			modelMatrix = glm::translate(modelMatrix, glm::vec3(fallonX, fallonY, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, mousemove + 0.05f, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(fallangle), glm::vec3(1, 0, 0));
		}
		modelMatrix = glm::rotate(modelMatrix, RADIANS(135+rotationangle), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghiinclinarelaterala), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f, 0.35f, 0.02f));
		RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.220f, 0.220f, 0.220f));
	}
	//Norii
	for (int i = 0; i < numarnori; i++) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, translatienorispate));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(norX[i], norY[i], 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(rotationnori), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
			RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.98f, 0.92f, 0.84f));
		}
		for (int j = 0; j < 3; j++) {
			if (j == 0) {
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, translatienorispate));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(norX[i] + norrandX[i][j], norY[i] + norrandY[i][j], 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(rotationnori), glm::vec3(1, 0, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(norrandrot[i][j]), glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(norrandscale[i][j]));
					RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(1, 1, 0.94f));
				}
			}
			else if (j == 1) {
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, translatienorispate));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(norX[i] + norrandX[i][j], norY[i] + norrandY[i][j], 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(rotationnori), glm::vec3(1, 0, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(norrandrot[i][j]), glm::vec3(0, 1, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(norrandscale[i][j]));
					RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(0.96f, 0.96f, 0.86f));
				}
			}
			else if (j == 2) {
				{
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, translatienorispate));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(norX[i] + norrandX[i][j], norY[i] + norrandY[i][j], 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(rotationnori), glm::vec3(1, 0, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(norrandrot[i][j]), glm::vec3(0, 0, 1));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(norrandscale[i][j]));
					RenderMesh(meshes["box"], shaders["Shader1"], modelMatrix, glm::vec3(1, 0.94f, 0.96f));
				}
			}
		}
	}
	
	//OBSTACOLE
	if (lifepoints > 0 && nivelcombustibil > 0) {
		if (coliziuneobstacol1 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(obstacol1X, obstacol1Y, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
				RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(1, 0, 0));
			}
		}
		if (coliziuneobstacol2 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(obstacol2X, -obstacol2Y, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
				RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(1, 0, 0));
			}
		}
	//COMBUSTIBIL
		if (coliziunecombustibil1 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(combustibilX, combustibilY, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				RenderMesh(meshes["teapot"], shaders["Shader1"], modelMatrix, glm::vec3(0.5f, 0, 0.5f));
			}
		}
		if (coliziunecombustibil2 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(combustibilX + 0.5f, combustibilY + 0.1f, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				RenderMesh(meshes["teapot"], shaders["Shader1"], modelMatrix, glm::vec3(0.5f, 0, 0.5f));
			}
		}
		if (coliziunecombustibil3 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(combustibilX + 1, combustibilY, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				RenderMesh(meshes["teapot"], shaders["Shader1"], modelMatrix, glm::vec3(0.5f, 0, 0.5f));
			}
		}
		if (coliziunecombustibil4 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(combustibilX + 1.5f, combustibilY - 0.1f, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				RenderMesh(meshes["teapot"], shaders["Shader1"], modelMatrix, glm::vec3(0.5f, 0, 0.5f));
			}
		}
		if (coliziunecombustibil5 == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(combustibilX + 2, combustibilY, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
				RenderMesh(meshes["teapot"], shaders["Shader1"], modelMatrix, glm::vec3(0.5f, 0, 0.5f));
			}
		}
		//extraviata
		if (coliziuneviata == 0) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(extraviataX, extraviataY, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieobstacole), glm::vec3(0, 1, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.4f));
				RenderMesh(meshes["sphere"], shaders["Shader1"], modelMatrix, glm::vec3(1, 1, 0));
			}
		}
	}
	//FUNDAL NEGRU VIETI PIERDUTE
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 2.5f, -3.02f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.21f, 0.21f, 0.01f));
		RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(0, 0, 0));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.5f, 2.5f, -3.02f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.21f, 0.21f, 0.01f));
		RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(0, 0, 0));
	}
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 2.5f, -3.02f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.21f, 0.21f, 0.01f));
		RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(0, 0, 0));
	}
	//AFISARE VIETI 
	if (lifepoints >= 1) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 2.5f, -3));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
			RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(1, 1, 0));
		}
	}
	if (lifepoints >= 2) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.5f, 2.5f, -3));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
			RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(1, 1, 0));
		}
	}
	if (lifepoints >= 3) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 2.5f, -3));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(30), glm::vec3(1, 1, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
			RenderMesh(meshes["sphere"], shaders["Shader2"], modelMatrix, glm::vec3(1, 1, 0));
		}
	}
	//AFISARE NIVEL COMBUSTIBIL
	if (lifepoints > 0) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(5.3f, 2.7f, -3));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 0.01f));
			RenderMesh(meshes["box"], shaders["Shader2"], modelMatrix, glm::vec3(0, 0, 0));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(5.3f, 2.7f, -2.99f));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(nivelcombustibil, 0.1f, 0.01f));
			RenderMesh(meshes["box"], shaders["Shader2"], modelMatrix, glm::vec3(0, 1, 0));
		}
	}
	{
		materialShininess = 50;
		materialKd = 5000;
		materialKs = 1;
		lightPosition = glm::vec3(0, 25, -40);
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -225, 20));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(10));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(rotatieocean), glm::vec3(0, 0, 1));
		RenderMesh(meshes["ocean"], shaders["Shader3"], modelMatrix, glm::vec3(0, 0.412f, 0.58f));
		lightPosition = glm::vec3(1, 6, 3);
		materialShininess = 50;
		materialKd = 50;
		materialKs = 1;
	}

	

	//scaderea constanta a nivelului combustibilului pana acesta ajunge sa fie 0
	if (nivelcombustibil >= 0.001f) {
		nivelcombustibil -= 0.001f;
	}
	else {
		lifepoints = 0;
	}
	//schimbarea inaltimii camerei in functie de miscarea mouse-ului
	if (firstperson == 1 || thirdperson == 1) {
		camera->position[1] = mousemove;
	}
	//mutarea pe axa X a camerei pana in fata avionului
	if (firstperson == 1) {
		camera->position[0] = 0.7f;
	}
	//Daca jocul a fost pierdut, avionul cade liber
	if (lifepoints == 0 || nivelcombustibil == 0) {
		fallonX += 1 * deltaTimeSeconds;
		fallonY -= 3 * deltaTimeSeconds;
		fallangle += 100 * deltaTimeSeconds;
	}
	//functii pt ridicarea sau caderea botului avionului in functie de coborarea sau ridicarea mouse-ului
	if (mousemove > 0 && unghiinclinare<20) {
			unghiinclinare += 75 * deltaTimeSeconds;	
	}
	else if (mousemove < 0 && unghiinclinare>-20) {
		unghiinclinare -= 75 * deltaTimeSeconds;
	}
	//rotatie continua a norilor
	rotationnori += 100 * deltaTimeSeconds;
	//inclinarea laterala a avionului
	if (contor == 0 && unghiinclinarelaterala < 5) {
		unghiinclinarelaterala += 50 * deltaTimeSeconds;
		if (unghiinclinarelaterala > 4.9f) {
			contor = 1;
		}
	}
	else if (contor == 1 && unghiinclinarelaterala > -5) {
		unghiinclinarelaterala -= 50 * deltaTimeSeconds;
		if (unghiinclinarelaterala < -4.9f) {
			contor = 0;
		}
	}
	//rotatia continua a obstacolelor
	rotatieobstacole += 500 * deltaTimeSeconds;
	//daca nu exista acceleratie, fac cele de mai jos
	if (acceleration == 0) {
		rotatieocean += speed * deltaTimeSeconds/2;
		extraviataX -= speed * deltaTimeSeconds;
		combustibilX -= speed * deltaTimeSeconds;
		obstacol1X -= speed * deltaTimeSeconds;
		obstacol2X -= speed * deltaTimeSeconds;
		rotationangle += 500 * deltaTimeSeconds;
		for (int i = 0; i < numarnori; i++) {
			norX[i] -= speed * deltaTimeSeconds;
		}
		if (speed >= 3.2f) {
			speed -= 0.05f;
		}
	}
	//daca apas space, creste viteza avionului si implicit viteza tuturor translatarilor
	else if (acceleration == 1) {
		rotatieocean += speed * deltaTimeSeconds/2;
		extraviataX -= speed * deltaTimeSeconds;
		combustibilX -= speed * deltaTimeSeconds;
		obstacol1X -= speed * deltaTimeSeconds;
		obstacol2X -= speed * deltaTimeSeconds;
		rotationangle += 1000 * deltaTimeSeconds;
		for (int i = 0; i < numarnori; i++) {
			norX[i] -= speed * deltaTimeSeconds;
		}
		if (speed <= 6) {
			speed += 0.05f;
		}
	}
	//resetarea obiectului pt extra viata care adauga in momentul coliziunii o viata daca nr de vieti e mai mic strict decat 3
	if (extraviataX < -5) {
		if (lifepoints < 3 && coliziuneviata == 1) {
			lifepoints += 1;
		}
		coliziuneviata = 0;
		extraviataX = 50;
		extraviataY = (float)rand() / (float)(RAND_MAX / 2);
	}
	//resetarea combustibililor 
	if (combustibilX < -30) {
		combustibilX = 10;
		combustibilY = (float)rand() / (float)(RAND_MAX / 2);
		coliziunecombustibil1 = 0;
		coliziunecombustibil2 = 0;
		coliziunecombustibil3 = 0;
		coliziunecombustibil4 = 0;
		coliziunecombustibil5 = 0;
	}
	//resetarea obstacolelor si verificarea coliziunii cu acestea
	if (obstacol1X < -10) {
		obstacol1X = 10;
		obstacol1Y = (float)rand() / (float)(RAND_MAX / 4);
		coliziuneobstacol1 = 0;
	}
	if (obstacol1X < -1 && coliziuneobstacol1 == 1 && colisiondetected1 == 1) {
		if (lifepoints > 0 ) 
			lifepoints -= 1;
		colisiondetected1 = 0;
	}
	if (obstacol2X < -10) {
		obstacol2X = 10;
		obstacol2Y = (float)rand() / (float)(RAND_MAX / 4);
		coliziuneobstacol2 = 0;
	}
	if (obstacol2X < -1 && coliziuneobstacol2 == 1 && colisiondetected2 == 1) {
		if (lifepoints > 0)
			lifepoints -= 1;
		colisiondetected2 = 0;
	}
	//resetarea norilor total aleatoare si a informatiilor despre acestia
	for (int i = 0; i < numarnori; i++) {
		if (norX[i] < -10) {
			norX[i] = 10+(float)rand() / (float)(RAND_MAX / 20);
			norY[i] = (float)rand() / (float)(RAND_MAX / 4) ;
			for (int j = 0; j < 3; j++) {
				norrandX[i][j] = (float)rand() / (float)(RAND_MAX / 0.75f);
				norrandY[i][j] = (float)rand() / (float)(RAND_MAX / 0.25f);
				norrandrot[i][j] = (float)rand() / (float)(RAND_MAX / 45);
				norrandscale[i][j] = (float)rand() / (float)(RAND_MAX / 0.5f);
			}
		}
	}
	//COLIZIUNI 
	if ((0 >= obstacol1X - 0.25f && 0 <= obstacol1X + 0.25f) &&
		(mousemove >= obstacol1Y - 0.25f && mousemove <= obstacol1Y + 0.25f)) {
		coliziuneobstacol1 = 1;
		colisiondetected1 = 1;
	}
	if ((0 >= obstacol2X - 0.25f && 0 <= obstacol2X + 0.25f) &&
		(-mousemove >= obstacol2Y - 0.25f && -mousemove <= obstacol2Y + 0.25f)) {
		coliziuneobstacol2 = 1;
		colisiondetected2 = 1;
	}
	if ((0 >= combustibilX - 0.4f && 0 <= combustibilX + 0.4f) &&
		(mousemove >= combustibilY - 0.6f && mousemove <= combustibilY + 0.4f)) {
		coliziunecombustibil1 = 1;
		if (nivelcombustibil < 2) {
			nivelcombustibil += 0.02f;
		}
	}
	if ((0 >= combustibilX +0.5f- 0.4f && 0 <= combustibilX +0.5f + 0.4f) &&
		(mousemove >= combustibilY +0.1f - 0.4f && mousemove <= combustibilY + 0.1f + 0.4f)) {
		coliziunecombustibil2 = 1;
		if (nivelcombustibil < 2) {
			nivelcombustibil += 0.02f;
		}
	}
	if ((0 >= combustibilX +1- 0.2f && 0 <= combustibilX +1+ 0.2f) &&
		(mousemove >= combustibilY - 0.2f &&mousemove <= combustibilY + 0.2f)) {
		coliziunecombustibil3 = 1;
		if (nivelcombustibil < 2) {
			nivelcombustibil += 0.02f;
		}
	}
	if ((0 >= combustibilX +1.5f- 0.4f && 0 <= combustibilX +1.5f+ 0.4f) &&
		(mousemove >= combustibilY -0.1f- 0.4f && mousemove <= combustibilY -0.1f+ 0.4f)) {
		coliziunecombustibil4 = 1;
		if (nivelcombustibil < 2) {
			nivelcombustibil += 0.02f;
		}
	}
	if ((0 >= combustibilX +2- 0.4f && 0 <= combustibilX +2+ 0.4f) &&
		(mousemove >= combustibilY - 0.4f && mousemove <= combustibilY + 0.4f)) {
		coliziunecombustibil5 = 1;
		if (nivelcombustibil < 2) {
			nivelcombustibil += 0.02f;
		}
	}
	if ((0 >= extraviataX - 0.25f && 0 <= extraviataX + 0.25f) &&
		(mousemove >= extraviataY - 0.25f && mousemove <= extraviataY + 0.25f)) {
		coliziuneviata = 1;
	}

	// Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
	if (renderCameraTarget)
	{

	}
}

void Game::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Game::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int location = glGetUniformLocation(shader->GetProgramID(), "light_position");
	glUniform3fv(location, 1, glm::value_ptr(lightPosition));

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	location = glGetUniformLocation(shader->GetProgramID(), "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	location = glGetUniformLocation(shader->GetProgramID(), "material_shininess");
	glUniform1ui(location, materialShininess);

	location = glGetUniformLocation(shader->GetProgramID(), "material_kd");
	glUniform1f(location, materialKd);

	location = glGetUniformLocation(shader->GetProgramID(), "material_ks");
	glUniform1f(location, materialKs);

	location = glGetUniformLocation(shader->GetProgramID(), "object_color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));


	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	mesh->Render();

	
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Game::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			camera->TranslateForward(deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			camera->TranslateRight(-deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			camera->TranslateForward(-deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(deltaTime);
		}
	}
	//functia de detectare a tipurilor de camere
}

void Game::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	//daca apas pe space, creste acceleratia
	if (key == GLFW_KEY_SPACE) {
		acceleration = 1;
	}
	if (key == GLFW_KEY_C) {
		if (firstperson == 0 && thirdperson == 0) {
			camera->Set(glm::vec3(-2, 2, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			thirdperson = 1;
			firstperson = 0;
		}
		else if (thirdperson == 1 && firstperson == 0) {
			camera->Set(glm::vec3(-2, 0.1f, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			firstperson = 1;
			thirdperson = 0;
		}
		else if (thirdperson == 0 && firstperson == 1) {
			camera->Set(glm::vec3(0, 2, 4), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
			firstperson = 0;
			thirdperson = 0;
		}
	}
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
	acceleration = 0;
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// functie la fel ca cea din laborator (care se afla mai jos) pt rotirea camerei third person in mod continuu
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed+
			camera->RotateFirstPerson_OX(deltaY * sensivityOX);
			camera->RotateFirstPerson_OY(deltaX * sensivityOY);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(deltaY * sensivityOX);
			camera->RotateThirdPerson_OY(deltaX * sensivityOY);
		}

	}
	//functii de detectare a miscarii mouse-ului in sus si in jos, conditionate de nivelul combustibilului si de vieti
	if (mouseY < 0 && lifepoints > 0 && nivelcombustibil >0) {
		mousemove += 0.01*deltaY;
	}
	else if (mouseY > 0 && lifepoints > 0 && nivelcombustibil >0) {
		mousemove -= 0.01*deltaY;

	}
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Game::OnWindowResize(int width, int height)
{
}
