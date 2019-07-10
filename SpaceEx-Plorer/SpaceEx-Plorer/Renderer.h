// Headerphile - Renderer.h
//
// This file controls most things related to the Renderer. 
// Put in a separate class to avoid code cluttering

#pragma once

// C++ standard
#include <iostream>
#include <string>	

// Headerphile
#include "Shader.h"
#include "Model.h"

// OpenGL stuff
#define GL3_PROTOTYPES 1
#include <glew.h>
#include <glm/glm.hpp>
#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Camera.h"

class Renderer
{
	std::string programName = "SpaceEx-Plorer";

	public :
	Renderer()
	{
		// Set up our matricies
		// I strongly encourage you to mess around with the values here and see what they do
		// It's a lot of fun, and you'll learn a lot
		// It'll also give you a better sense of what's going on
		// ...Which will make it easier for you to debug

		// Set up projection matric
		projection = glm::perspective
		(
			45.0f,       // Field of view ( how far out to the sides we can see
			16.0f / 9.0f, // Aspect ratio ( stretch image in widh or height )
			0.1f,        // Near plane ( anything close than this will be cut off )
			100.0f       // Far plane ( anything further away than this will be cut off )
		);

		// Set up view matric
		view = glm::lookAt
		(
			glm::vec3(0,0,-4), // Camera is at (0,0,-4), in World Space
			glm::vec3(0,0,0),  // And looks at the origin
			glm::vec3(0,-1,0)  // Head is up ( set to 0,-1,0 to look upside-down )
		);
	}

	glm::vec3 cameraPosition = glm::vec3(0, 0, -4);

	SDL_Window* getWindow() { return mainWindow; }

	bool Init()
	{
		// Initialize SDL's Video subsystem
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "Failed to init SDL\n";
			return false;
		}

		// Create our window centered at 512x512 resolution
		mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				1600, 900, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		// Check that everything worked out okay
		if (!mainWindow )
		{
			std::cout << "Unable to create window\n"<< std::endl;;
			CheckSDLError(__LINE__);
			return false;
		}

		SetSDL_GL_Attributes();

		// Create our opengl context and attach it to our window
		mainContext = SDL_GL_CreateContext(mainWindow );

		// This makes our buffer swap syncronized with the monitor's vertical refresh
		// 		( which means it enables v-sync)
		// Setting this to 0 will disable V-sync
		// 		Which means our application could run at unlimited fps
		SDL_GL_SetSwapInterval(1);

		// Init GLEW
		glewExperimental = GL_TRUE; 
		glewInit();

		SetOpenGLOptions();

		//if (!CreateSimpleShader()) return false;

		return true;
	}

	bool SetUpShader(const std::string &vertex, const std::string &fragment)
	{
		if (!simpleShader.Init())
			return false;

		//if (!simpleShader.LoadShader("vert.glsl", GL_VERTEX_SHADER))
		if (!simpleShader.LoadShader(vertex, GL_VERTEX_SHADER))
			return false;

		if (!simpleShader.LoadShader(fragment, GL_FRAGMENT_SHADER))
			return false;

		if (!simpleShader.LinkShaders())
			return false;

		return true;
	}

	// Do the first step of rendering ( clearing everything )
	void RenderStart()
	{
		// Make our background black
		glClearColor(0.1, 0.1, 0.1, 1.0);

		// Clear the depth buffer
		glClearDepth(1.0f);

		// We need to add GL_DEPTH_BUFFER_BIT so that the depth buffer will be cleared too.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	}

	// Renders a Model
	void RenderModel(const Model &model)
	{
		model.Render( );
	}

	// Called at the end to swap the buffers
	void RenderEnd()
	{
		// Swap our buffers to make our changes visible
		SDL_GL_SwapWindow(mainWindow);

		//glutSwapBuffers();
	}

	void SetMatrix( const glm::mat4 &model, Camera camera)
	{
		glm::mat4 mvp = projection * camera.getView() * model;
		simpleShader.SetMatrix( mvp );
	}

	void SetShader(Model &m)
	{
		m.SetShader(simpleShader);
	}

	void Cleanup()
	{
		// Cleanup all the things we bound and allocated
		simpleShader.CleanUp();

		// Delete our OpengL context
		SDL_GL_DeleteContext(mainContext);

		// Destroy our window
		SDL_DestroyWindow(mainWindow );

		// Shutdown SDL 2
		SDL_Quit();
	}
	// DO NOT CALL
	void printText(unsigned char string[])
	{
		int w = glutBitmapLength(GLUT_BITMAP_8_BY_13, string);

		float x = 0.5f;

		glRasterPos2f(x - (float)w / 2, 0.0);

		glColor3f(1, 1, 1);

		int len = (strlen((char*)string));

		for (int i = 0; i < len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[i]);
		}

	}

	void CheckSDLError(int line = -1)
	{
		std::string error = SDL_GetError();

		if (error != "")
		{
			std::cout << "SLD Error : " << error << std::endl;

			if (line != -1)
				std::cout << "\nLine : " << line << std::endl;

			SDL_ClearError();
		}
	}
	private:

	// This set our extra OpenGL options we'll need
	void SetOpenGLOptions()
	{
		// Enable blending so that we can have transparanet object
		glEnable(GL_BLEND ) ;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Enable depth testing so that closer triangles will hide the triangles farther away
		glEnable( GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		// Enable wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Enable backface culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	bool SetSDL_GL_Attributes()
	{
		// Set our OpenGL version.
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		// Turn on double buffering with a 24bit Z buffer.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return true;
	}

	// Our SDL_Window ( just like with SDL2 wihout OpenGL)
	SDL_Window *mainWindow;

	// Our opengl context handle
	SDL_GLContext mainContext;

	// Our wrapper to simplify the shader code
	Shader simpleShader;

	// Matricies
	glm::mat4 projection;
	glm::mat4 view;
};
