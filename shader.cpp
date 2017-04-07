#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>

class Shader {
  //FIXME: Steal my usual shader.c

  private:

  GLuint program;

  class ShaderCode {

    public:

    GLuint shader;

    ShaderCode(GLuint type, std::string path) {
      std::ifstream file(path);
      std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      shader = glCreateShader(type);

      const char* codeCString = code.c_str();
      glShaderSource(shader, 1, &codeCString, NULL);

      glCompileShader(shader);
    }
  };

  public:
  
  //FIXME: This is a bad idea as GL might not be ready in constructors..
  Shader(std::string vertexPath, std::string fragmentPath) {
    ShaderCode vs(GL_VERTEX_SHADER, vertexPath + ".vsh");
    ShaderCode fs(GL_FRAGMENT_SHADER, fragmentPath + ".fsh");

    program = glCreateProgram();

    glAttachShader(program, vs.shader);
    glAttachShader(program, fs.shader);

    glBindAttribLocation(program, 0, "vertex");

    glLinkProgram(program);
  }

  Shader(std::string path) : Shader(path, path) {}

  void activate() {
    //FIXME: Shitty API?
    glUseProgram(program);
  }

  void set(std::string variable, glm::mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(program, variable.c_str()), 1, GL_TRUE, glm::value_ptr(value));
  }
  void set(std::string variable, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program, variable.c_str()), 1, GL_TRUE, glm::value_ptr(value));
  }
  void set(std::string variable, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(program, variable.c_str()), 1, glm::value_ptr(value));
  }
  void set(std::string variable, float value) {
    glUniform1f(glGetUniformLocation(program, variable.c_str()), value);
  }
  void set(std::string variable, unsigned int value) {
    glUniform1i(glGetUniformLocation(program, variable.c_str()), value);
  }
};
