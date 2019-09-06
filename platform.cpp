#include <assimp/cimport.h>

#include <cstdio>
#include <cstdlib>
 
#include "SDL.h"

#include <IL/il.h>
#include <IL/ilu.h>

#include "GL/glew.h"

#include "camera.h"
 
//FIXME: Remove
static void sdl_die(const char * message) {
  fprintf(stderr, "%s: %s\n", message, SDL_GetError());
  exit(2);
}

namespace Platform {

  static SDL_Window *window = NULL;
  static SDL_GLContext maincontext;

  static void initialize(std::string title = "", unsigned int width = 800, unsigned int height = 450, bool fullscreen = false) {

    // Initialize SDL 
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      sdl_die("Couldn't initialize SDL");
    }
    atexit (SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

#if 0
    // Request an OpenGL 3.3 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create the window
    if (fullscreen) {
      window = SDL_CreateWindow(
        title.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
      );
    } else {
      //FIXME: Make resizeable
      window = SDL_CreateWindow(
        title.c_str(), 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        width, height, SDL_WINDOW_OPENGL
      );
    }
    if (window == NULL) {
      sdl_die("Couldn't set video mode");
    }

    maincontext = SDL_GL_CreateContext(window);
    if (maincontext == NULL) {
      sdl_die("Failed to create OpenGL context");
    }

#if 0
    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif

    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      exit(1);
    }

    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Initialize DevIL
    ilInit();
    iluInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    // Init assimp
    struct aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
  	aiAttachLogStream(&stream);

    // Init OpenAL
    alutInit(NULL, NULL);
    alListenerf(AL_GAIN, 0.15f); // FIXME: make this an option?

    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    // Disable depth test and face culling.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  }

  static bool quit = false;
  static bool hasQuit() { return quit; }

  static void step() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        return;
      }
    }

    SDL_GL_SwapWindow(window);
  }

  glm::vec3 pick(glm::vec2 windowCoordinates, std::shared_ptr<Camera> camera) {
    // Read GL Depth buffer
    float depth;
    glReadPixels(windowCoordinates.x, camera->getViewport().y - windowCoordinates.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    return camera->unproject(glm::vec3(windowCoordinates, depth));
  }

  static void drawLine(glm::vec3 a, glm::vec3 b) {
    //FIXME!!!
    glBegin(GL_LINES);
    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
    return;
  }

  static void drawPoint(glm::vec3 p) {
    glBegin(GL_POINTS);
    glVertex3f(p.x, p.y, p.z);
    glEnd();
  }

}
