#pragma once

#include <FreeImage.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <string>

struct HUD {
  GLubyte *texture;
  unsigned int width;
  unsigned int height;

  void load(const std::string &fileName) {
    FIBITMAP *bitMap = FreeImage_Load(FreeImage_GetFileType(fileName.c_str()),
                                      fileName.c_str());
    {
      FIBITMAP *temp = bitMap;
      bitMap = FreeImage_ConvertTo32Bits(bitMap);
      FreeImage_Unload(temp);
    }
    width = FreeImage_GetWidth(bitMap);
    height = FreeImage_GetHeight(bitMap);
    int texSize = width * height * 4;
    GLubyte *temp = FreeImage_GetBits(bitMap);
    texture = new GLubyte[texSize];
    std::copy(temp, temp + texSize, texture);

    FreeImage_Unload(bitMap);
  }

  void draw(unsigned int x, unsigned int y, float zoom) {
    glRasterPos2i(x, y);

    glPixelZoom(zoom, zoom);
    glDrawPixels(width, height, GL_BGRA, GL_UNSIGNED_BYTE, texture);
  }

  ~HUD() { delete[] texture; }
};
