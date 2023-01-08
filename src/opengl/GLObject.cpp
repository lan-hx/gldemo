//
// Created by lan on 2023/1/8.
//

#include "opengl/GLObject.h"

#include <QOpenGLFunctions>

uint64_t GLObject::id_inc = 0;

// TODO
void GLObject::Draw() {
  auto f = QOpenGLContext::currentContext()->functions();
  model_->GetTexture()->bind();
  shader_->bind();
  shader_->setUniformValue("model", transform_.GetModelMatrix());
  model_->GetVao()->bind();
  // TODO: draw
  // f->glDrawElements(GL_TRIANGLES, ..., GL_UNSIGNED_INT, 0);
  model_->GetVao()->release();
  shader_->release();
}
