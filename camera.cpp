#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

class Camera {

  private:

  float fov;
  float nearPlane;
  float farPlane;
  glm::vec3 position;
  glm::mat3 rotation;

  struct {
    float width;
    float height;
  } viewport = { 800.0f, 450.0f }; //FIXME: Retrieve from platform code or something?!

  public:

  //FIXME: Supply fov in radians..
  Camera() : fov(0.7853981634f), nearPlane(1.0f), farPlane(1000.0f) {}

  void setFov(float fov) {
    this->fov = fov;
  }

  void setPosition(glm::vec3 position) {
    this->position = position;
  }

  glm::vec3 getPosition() {
    return position;
  }

  glm::vec2 getViewport() {
    return glm::vec2(viewport.width, viewport.height);
  }

  void lookAt(glm::vec3 point, glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f)) {
    glm::mat4 m = glm::lookAt(position, point, up); //FIXME: Extract rotation
    rotation = glm::mat3(m);
  }

  //FIXME: a 3x4 / 4x3 would be enough!
  glm::mat4 viewMatrix() {
    return glm::translate(glm::mat4(rotation), -position); //FIXME: Untested
  }

  glm::mat4 projectionMatrix() {
    return glm::perspectiveFov(fov, viewport.width, viewport.height, nearPlane, farPlane);
  }

  glm::mat4 viewProjectionMatrix() {
    return viewMatrix() * projectionMatrix();
  }

  glm::vec3 unproject(glm::vec3 windowCoordinates) {
    windowCoordinates.y = viewport.height - windowCoordinates.y;
    return glm::unProject(windowCoordinates, viewMatrix(), projectionMatrix(), glm::vec4(0.0, 0.0, viewport.width, viewport.height));
  }

  glm::vec3 project(glm::vec3 objectCoordinates) {
    glm::vec3 windowCoordinates = glm::project(objectCoordinates, viewMatrix(), projectionMatrix(), glm::vec4(0.0, 0.0, viewport.width, viewport.height));
    windowCoordinates.y = viewport.height - windowCoordinates.y;
    return windowCoordinates;
  }

};
