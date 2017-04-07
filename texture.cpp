//FIXME: Load into GL

#include <IL/il.h>
#include <IL/ilu.h>

#include <GL/glew.h>

class Texture {
  private:

  GLuint texture;
  unsigned int width;
  unsigned int height;

  public:

  Texture(std::string path) {
    //FIXME: DevIL magic
    std::cout << "Loading " << path << std::endl;

    ILuint image;
    ilGenImages(1, &image);
    ilBindImage(image);
    ilLoadImage((path + ".png").c_str());

    ilConvertImage(IL_RGBA, GL_UNSIGNED_BYTE);

    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
    glGenerateMipmap(GL_TEXTURE_2D);

    ilDeleteImages(1, &image);
  }

  ~Texture() {
    std::cout << "Removing texture" << std::endl;
    glDeleteTextures(1, &texture);
  }

  void activate(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);    
  }
};
