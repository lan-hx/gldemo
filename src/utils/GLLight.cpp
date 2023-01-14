//
// Created by lan on 2023/1/8.
//

#include "utils/GLLight.h"

#include <GLES3/gl3.h>

#include <QOpenGLExtraFunctions>

#define ASSERT(expr, info) assert(expr &&info)

void GLLights::CheckOffsets(QOpenGLShaderProgram *shader) {
  static constexpr const char *names[] = {
      "lights[0].position", "lights[0].direction", "lights[0].color", "lights[0].intensity", "lights[0].angle",
      "lights[0].kc",       "lights[0].kl",        "lights[0].kq",    "lights[0].type",      "lights[1].position"};
  static constexpr size_t count = sizeof(names) / sizeof(names[0]);
  static constexpr size_t c_offset[] = {offsetof(GLLightStd140, position_), offsetof(GLLightStd140, direction_),
                                        offsetof(GLLightStd140, color_),    offsetof(GLLightStd140, intensity_),
                                        offsetof(GLLightStd140, angle_),    offsetof(GLLightStd140, kc_),
                                        offsetof(GLLightStd140, kl_),       offsetof(GLLightStd140, kq_),
                                        offsetof(GLLightStd140, type_),     sizeof(GLLightStd140)};

  QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

  auto index = f->glGetUniformBlockIndex(shader->programId(), "Lights");
  int size;
  f->glGetActiveUniformBlockiv(shader->programId(), index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
  qDebug() << "[INFO] GLLights::CheckOffsets: uniform size: " << size;
  unsigned int indices[count];
  memset(indices, 0, sizeof(indices));
  f->glGetUniformIndices(shader->programId(), count, names, indices);
  int offset[count];
  memset(offset, 0, sizeof(offset));
  f->glGetActiveUniformsiv(shader->programId(), count, indices, GL_UNIFORM_OFFSET, offset);
  qDebug() << "[INFO] GLLights::CheckOffsets: glGetActiveUniformsiv error: " << f->glGetError();
  for (size_t i = 0; i < count; ++i) {
    qDebug() << "[INFO] GLLights::CheckOffsets: " << names[i] << ": index " << indices[i] << " offset " << offset[i];
  }
  for (size_t i = 0; i < count; ++i) {
    ASSERT(offset[i] == c_offset[i], "offset error");
  }
}
