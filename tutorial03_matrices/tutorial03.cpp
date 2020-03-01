#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkSurface.h"
#include "include/utils/SkRandom.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "src/gpu/gl/GrGLUtil.h"
#include "src/gpu/gl/GrGLUtil.cpp"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>


void draw(SkCanvas* canvas) {
  canvas->drawColor(SkColorSetRGB(200, 200, 200));
  
  SkPaint paint;
  paint.setAntiAlias(true);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(4);
  paint.setColor(SK_ColorRED);
  
  SkRect rect = SkRect::MakeXYWH(150, 150, 140, 160);
  canvas->drawRect(rect, paint);
  
  SkRRect oval;
  oval.setOval(rect);
  oval.offset(140, 160);
  paint.setColor(SK_ColorBLUE);
  canvas->drawRRect(oval, paint);
  
  paint.setColor(SK_ColorCYAN);
  canvas->drawCircle(280, 150, 125, paint);
  
  rect.offset(180, 0);
  paint.setColor(SK_ColorYELLOW);
  canvas->drawRoundRect(rect, 10, 10, paint);
  
  SkPath path;
  path.cubicTo(768, 100, -512, 256, 256, 256);
  paint.setColor(SK_ColorGREEN);
  canvas->drawPath(path, paint);
}



int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

  static const int kMsaaSampleCount = 4;
  static const int kStencilBits = 8;
  
  glfwWindowHint(GLFW_SAMPLES, kMsaaSampleCount);
  glfwWindowHint(GLFW_STENCIL_BITS, kStencilBits);
  
  int width = 1024;
  int height = 768;
  
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( width, height, "Tutorial 03 - Matrices", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
//  glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
//  glPushAttrib(GL_ALL_ATTRIB_BITS);
  
  
  int fbo1 = 0;
  int fbo2 = 0;
  // setup GrContext
  auto interface = GrGLMakeNativeInterface();
  
  // setup contexts
  sk_sp<GrContext> grContext(GrContext::MakeGL(interface));
  SkASSERT(grContext);

  SkColorType colorType;
  colorType = kRGBA_8888_SkColorType;
  
  // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
  // render to it
  GrGLint buffer;
  GR_GL_GetIntegerv(interface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);
  GrGLFramebufferInfo info;
  info.fFBOID = (GrGLuint) buffer;
  
  info.fFormat = GR_GL_RGBA8;
  colorType = kRGBA_8888_SkColorType;
  GrBackendRenderTarget target(width, height, kMsaaSampleCount, kStencilBits, info);
  
  SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
  
  sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(grContext.get(), target,
                                                                  kBottomLeft_GrSurfaceOrigin,
                                                                  colorType, nullptr, &props));
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo1);
  SkCanvas* canvas = surface->getCanvas();
  //canvas->scale(float(width)/1920.f, float(height)/1080.f);
  
  

//  auto sk_texture = grContext->createBackendTexture(800, 600, colorType, GrMipMapped::kNo, GrRenderable::kYes);
//  GrGLTextureInfo tex_info;
//  sk_texture.getGLTextureInfo(&tex_info);
//
//
//  auto surface_tex = SkSurface::MakeFromBackendTexture(grContext.get(),
//                                                       sk_texture,
//                                                       GrSurfaceOrigin::kTopLeft_GrSurfaceOrigin,
//                                                       4,
//                                                       colorType,
//                                                       nullptr, //SkColorSpace::MakeSRGB(),
//                                                       nullptr);
//  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo2);
//
//  auto c = surface_tex->getCanvas();
//  draw(c);
//  c->flush();
  
  

  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  //glEnable(GL_MULTISAMPLE);
  glDisable(GL_MULTISAMPLE);
  glLoadIdentity();
  
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "SimpleTransform.vertexshader", "SingleColor.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	glm::mat4 View = glm::lookAt(
                               glm::vec3(0,0,3), // Camera is at (4,3,3), in World Space
                               glm::vec3(0,0,0), // and looks at the origin
                               glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                               );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	static const GLfloat g_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

