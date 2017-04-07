
// A convex + flat water body
class Water {

  public:

/*
  // Adds a wave using the given (closed!) outline
  void addSplash(std::vector<float[3]> outline) {
  }

  // Adds a line where waves are reflected
  void addCollider(float[2] a, float[2] b) {
  }
*/

  void draw() {
    //FIXME: Steal from https://www.shadertoy.com/view/Ms2SD1

    static Shader shader("data/shader/generic", "data/shader/water");
    shader.activate();
    static float t = 0.0f;
    t += 0.01f;
    shader.set("iGlobalTime", t);
    shader.set("iResolution", glm::vec3(800.0f, 450.0f, 0.0f));
  
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-100.0f, -100.0f, 0.0f);
    glVertex3f(-100.0f, +100.0f, 0.0f);
    glVertex3f(+100.0f, -100.0f, 0.0f);
    glVertex3f(+100.0f, +100.0f, 0.0f);
    glEnd();
  }
};
