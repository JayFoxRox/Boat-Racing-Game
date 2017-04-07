class Sky {
  std::shared_ptr<Model> model;

  public:

  Sky() {
    model = std::make_shared<Model>("data/model/cube", "data/model/Daylight Box UV");
  }

  void draw() {
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);
    m[3*4+0] = 0.0f;
    m[3*4+1] = 0.0f;
    m[3*4+2] = 0.0f;
    glLoadMatrixf(m);
    glScalef(100.0f, 100.0f, 100.0f);
    model->draw();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
  }
};
