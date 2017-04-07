#include <iostream>
#include <memory>

#include <chrono>
#include <ratio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sound.h"
#include "mesh.h"
#include "camera.h"
#include "platform.h"
#include "shader.h"
#include "texture.h"
#include "modelcurve.h"
#include "text.h"
#include "animation.h"
#include "soundcloud.h"

#include "track.h"
#include "sky.h"
#include "boat.h"
#include "water.h"
#include "sparks.h"


#if 0 
//FIXME!!!
#include "glad/glad.h"
#endif

int main(int argc, char* argv[]) {

  std::cout << "Hey!" << std::endl;

  Platform::initialize();

#if 0
  // Check if soundtrack downloading works
  auto trackId = SoundCloud::resolveTrackUrl("https://soundcloud.com/howl-naj-basit/mudmen-animal");
  SoundCloud::Track foo(trackId);
  foo.download();
#endif

  std::shared_ptr<Animation> animation = std::make_shared<Animation>("data/anim/fire");
  animation->play();

#if 0
  if (!Music::initialize()) {
    // 1. schedule uncached songs for online-check
    // 2. ask the user if accessing soundcloud is okay
    // 3. perform check
    // 4. report errors via HUD
    // 5. schedule background download and report errors via ticker
  }

  Music::play();
#endif

  std::vector<std::shared_ptr<Sparks>> sparks;

  std::shared_ptr<Sound> sound = std::make_shared<Sound>("data/music/music");
//  sound->play();

  std::shared_ptr<Texture> texture = std::make_shared<Texture>("data/font/main");

  std::shared_ptr<Text> text = std::make_shared<Text>("data/font/arial_black_regular_65");

  std::string meshPath = "data/mesh/cube";
  std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
  std::shared_ptr<Mesh> meshSliced = std::make_shared<Mesh>();

  glm::vec3 slicePlaneOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
  Plane slicePlane = Plane(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)), slicePlaneOrigin);

#if 0
  // Unit test Plane::collideRay
  std::cout << "Length 1: " << slicePlane.collideRay(glm::vec3(0.0f, 0.0f, -1.0f), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))) << std::endl;
  std::cout << "Length 2: " << slicePlane.collideRay(glm::vec3(0.0f, 0.0f, 3.0f), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))) << std::endl;
  std::cout << "Length 3: " << slicePlane.collideRay(glm::vec3(0.0f, 2.0f, 3.0f), glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))) << std::endl;
  std::cout << "Length 4: " << slicePlane.collideRay(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f))) << std::endl;
  std::cout << "Length 5: " << slicePlane.collideRay(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(glm::vec3(0.0f, -2.0f, -1.0f))) << std::endl;
#endif

#if 0
  // Unit test slicer
  for(unsigned int i = 0; i < 10; i++) {
    meshSliced->slice(Plane(glm::vec3(1.0,0.0,0.0),-4.0f + i * 1.0f));
  }
#endif

  std::shared_ptr<Shader> genericShader = std::make_shared<Shader>("data/shader/generic");
  std::shared_ptr<Shader> genericTexturedShader = std::make_shared<Shader>("data/shader/generic","data/shader/font");

  genericShader->activate();

  std::shared_ptr<Camera> camera = std::make_shared<Camera>();
  auto updateMatrix = [&]() {
#if 1
      //Hack to get simple output with legacy GL
      glMatrixMode(GL_PROJECTION);
#if 0
      glLoadIdentity();
      gluPerspective(45.0f, 1.0f, 1.0f, 1000.0f);
#else
      glLoadMatrixf(glm::value_ptr(camera->projectionMatrix()));
#endif
      glMatrixMode(GL_MODELVIEW);
#if 0
      glLoadIdentity();
      gluLookAt(10.0f, 10.0f, 10.0f,
               0.0f, 0.0f, 0.0f,
               0.0f, 0.0f, 1.0f);
#else
      glLoadMatrixf(glm::value_ptr(camera->viewMatrix()));
#endif
#endif
  };

  Curve2 curve;
  Curve2 curveRigid;
  float l = 10.0f;
  float sl = 3.5f;
  for(int i = 0; i <= l / sl; i++) {

    float a = (float)i / (l / sl) * 2.0f * 3.141;

    float x = -l/2.0f + i * sl;
    float y = 1*sinf(a * 3.0f);

    float r = 4.0f + y;
    float nx = cosf(a) * r;
    float ny = sinf(a) * r;

    curve.nodes.emplace_back(glm::vec2(nx, ny-0.3));
    curveRigid.nodes.emplace_back(glm::vec2(nx, ny+0.3));
  }

  Track track("data/track/test");

  std::shared_ptr<Model> model = std::make_shared<Model>(mesh);

  std::shared_ptr<ModelCurve> modelCurve = std::make_shared<ModelCurve>(curve);
  std::shared_ptr<ModelCurve> modelCurveRigid = std::make_shared<ModelCurve>(curveRigid);
  std::vector<std::shared_ptr<Model>> models;
  models.push_back(model);
#if 1
  track.fillCurve(modelCurve, false, true, models);
  track.fillCurve(modelCurveRigid, true, true, models);
#endif

  // Some boats in the name of science :P
  std::vector<Boat> boats;
  for(unsigned int i = 0; i < 6; i++) {
    boats.emplace_back("data/boat/test");
    auto& boat = boats.back();
    boat.position.x = i * 12.0f;
  }

  std::shared_ptr<Model> testModel = std::make_shared<Model>("data/model/test");

  while(!Platform::hasQuit()) {


    static float t = -1000.0f;
    t += 0.01f;

    //std::cout << "Top: " << slicePlane.getDistance(camera.getPosition()) << std::endl;


//    renderer->setDepth(false);
//    renderer->sprite(anim->texture(), glm::mat3::identity());

    // Get input

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    bool quit = state[SDL_SCANCODE_ESCAPE];

    bool showSliced = state[SDL_SCANCODE_RETURN];

    // FIXME: Use simplefilewatcher later
    bool reloadTrack = state[SDL_SCANCODE_F5];
    bool removeParticles = state[SDL_SCANCODE_F6];

    if (quit) {
      break; //FIXME: Request SDL / platform to quit instead?!
    }
    
    if (reloadTrack) {
      track.reload();
    }

    if (removeParticles) {
      std::cout << "Clearing particles" << std::endl;
      sparks.clear();
    }

    // FIXME: Do logic

#if 0
    // Test sounds by making music sound absolutely ridiculous
    sound->setPitch(sinf(t*20.0f)*0.3+1.0f);
#endif

    // Start drawing

    glClearColor(sinf(t * 3.0f)*0.5f+0.5f, 0.5f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    genericShader->activate();
    genericShader->set("viewProjectionMatrix", camera->viewProjectionMatrix());

    auto& boat = boats[0];

#if 1
    // Update player boat input
    auto& input = boat.input;
    input.accelerate = state[SDL_SCANCODE_UP] ? 1.0f : 0.0f;
    input.brake = state[SDL_SCANCODE_DOWN] ? 1.0f : 0.0f;
    if (state[SDL_SCANCODE_LEFT]) {
      input.steer -= 1.0f;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
      input.steer += 1.0f;
    }
#endif

#if 1
    // Update boats
    for(unsigned int i = 0; i < boats.size(); i++) {
      auto& boat = boats[i];
      boat.step();
    }
#endif

    //FIXME: Resolve collisions
    for(unsigned int i = 0; i < boats.size(); i++) {
      auto& boatI = boats[i];
      for(unsigned int j = (i + 1); j < boats.size(); j++) {
        auto& boatJ = boats[j];
        glm::vec3 d = boatJ.position - boatI.position;
        //FIXME: Handle boats on exact same location!
        float r = 5.0f;
        if (glm::length(d) < r) {
          glm::vec3 p = boatI.position + d * 0.5f;
          float eps = 0.001; // 1mm (if you go lower than this you'll have infinite collisions)
          //FIXME: Add epsilon to make sure we don't have a collision after setting these back!
          boatI.position = p - glm::normalize(d) * (r + eps) / 2.0f;
          boatJ.position = p + glm::normalize(d) * (r + eps) / 2.0f;
          sparks.push_back(std::make_shared<Sparks>(p, glm::vec3(0.0f, 0.0f, 10.5f)));
        }
        boatI.position.z = 0.0f;
        boatJ.position.z = 0.0f;
      }
    }

#if 1
    // Set camera to follow player boat
    glm::vec2 o(cosf(boat.heading), sinf(boat.heading));
    glm::vec3 cp(boat.getPosition() + glm::vec3(o * 5.0f, 5.0f));

    //cp = glm::mix(camera->getPosition(), cp, 0.5f);

    glm::vec3 d = camera->getPosition() - cp;
    float maxDist = 15.0f;
    if (glm::length(d) > maxDist) {
      cp = glm::mix(camera->getPosition(), cp + glm::normalize(d) * maxDist, 0.3f);
      camera->setPosition(cp);
    }


    camera->lookAt(boat.getPosition() + glm::vec3(0.0f, 0.0f, 2.0f));
    updateMatrix();
    glm::vec3 p = camera->getPosition();
    alListener3f(AL_POSITION, p.x, p.y, p.z);
    //FIXME: velocity, forward and up
    alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    float orient[6] = { 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 1.0f };
    alListenerfv( AL_ORIENTATION, orient );

#endif
  
    // Draw sky
    static Sky sky = Sky();
    genericTexturedShader->activate();
    genericTexturedShader->set("texture", 0u);
    glColor3f(1.0f, 1.0f, 1.0f);
    sky.draw();

    static Water water = Water();
    //water.draw();

#if 1
    // Draw all boats
    for(unsigned int i = 0; i < boats.size(); i++) {
      auto& boat = boats[i];
      
      glColor3f(1.0f, 0.0f, 0.0f);

      genericTexturedShader->activate();
      genericTexturedShader->set("texture", 0u);

      boat.draw();
    }
#endif

    genericShader->activate();

    float gap = 10.0f;
    slicePlane.draw(gap, 1100.0f / gap, 1100.0f / gap);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glLineWidth(1.0f);
#if 0
    if (showSliced) {
      glColor3f(1.0f, 0.0f, 0.0f);
      mesh->draw();
    } else {
      glColor3f(0.0f, 0.0f, 1.0f);
      meshSliced->draw();
    }
#endif
    
#if 0
    // Test curves

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    glPointSize(20.0f);
    float offset = t * 2.0f + 2000.0f; // - 10.0f;
    //offset = 1.0f + 8.0f;
    glPointSize(1.0f);

    glm::vec2 p = curveRigid.getNode(offset).position;
    int ni = curveRigid.getNodeIndex(offset);
    std::cout << "Node " << ni << " (" << curveRigid.getOffset(1) << ")" << std::endl;
    glm::vec2 lp = curveRigid.getNode(ni).position;
    glBegin(GL_POINTS);
    glColor3f(0.5f, 0.0f, 0.5f);
    glVertex3f(lp.x, lp.y, 0.0f);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(p.x, p.y, 0.0f);
    glEnd();
#endif

#if 0
    // Test curve drawing
    
    glColor3f(0.0f, 1.0f, 0.0f);
    modelCurve->draw();
    //modelCurveRigid->draw();
    genericShader->activate();
    modelCurve->curve.draw();
    //modelCurveRigid->curve.draw();
#endif

    glLineWidth(1.0f);

    // FIXME: Draw the world
    // FIXME: Draw the boats

#if 1
    // Draw a small axis system in legacy GL
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
#endif

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#if 0
    // Test models
    genericTexturedShader->activate();
    genericTexturedShader->set("texture", 0u);
    glColor3f(1.0f, 1.0f, 1.0f);
    testModel->draw();
#endif

#if 1
    // Test picking and mouse
    {
      int x, y;
      Uint32 buttons = SDL_GetMouseState(&x, &y);
      glm::vec3 p = Platform::pick(glm::vec2(x, y), camera);
      glPointSize(20.0f);
      glColor3f(1.0f, 0.0f, 0.0f);
      glUseProgram(0);
      Platform::drawPoint(p);
      glPointSize(1.0f);

      static bool lock = false;
      if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        if (!lock) {
          camera->lookAt(p);
          lock = true;
        }
      } else {
        lock = false;
      }

    }
#endif

#if 1
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    genericTexturedShader->activate();
    genericTexturedShader->set("texture", 0u);
    for(auto& s : sparks) {
      s->step(); //FIXME: Do this in an earlier step so these can react with the world
      s->draw();
    }
    glDepthMask(GL_TRUE);
#endif


    // FIXME: Draw HUD  

#if 0
    // Animation and sprite test
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    //animation->texture()->activate(0);
    animation->step();
    //FIXME: Draw stuff!
    glDisable(GL_BLEND);
#endif

#if 1
    //FIXME!!!
    auto current = std::chrono::steady_clock::now();
    static auto last = current;
//    std::chrono::duration<uint64_t, std::milli> mspf = std::chrono::duration_cast<uint64_t, std::milli>(current - last);
    using Duration = std::chrono::duration<uint64_t, std::micro>;
    Duration uspf = std::chrono::duration_cast<Duration>(current - last);
      
    last = current;
#endif

#if 1
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(-0.95f, 0.9f - 0.2f, 0.0f);

    //FIXME: Do AR correction elsewhere
    float aspectRatio = 800.0f / 450.0f;
    glScalef(0.0005f, 0.0005f * aspectRatio, 1.0f);

    // Test text
    glColor3f(0.1f, 0.2f, 0.5f);
    text->draw(std::string("boats: ") + std::to_string(boats.size()) + std::string("\nsparks: ") + std::to_string(sparks.size()) + std::string("\nus/F: ") + std::to_string(uspf.count()));

    glPopMatrix();
#endif

    glFinish();
    Platform::step();
  }

  return 0;
}
