//
// Created by lan on 2023/1/8.
//

#include "opengl/GLObject.h"

#include <QOpenGLFunctions>

uint64_t GLObject::id_inc = 0;

// TODO
void GLObject::Draw() {
  auto f = QOpenGLContext::currentContext()->functions();
  model_->SetMaterial(shader_);
  model_->GetTexture()->bind();
  shader_->bind();
  model_->GetVao()->bind();
  shader_->setUniformValue("model", transform_.GetModelMatrix());
  // TODO: draw
  f->glDrawElements(GL_TRIANGLES, model_->indices_.size(), GL_UNSIGNED_INT, nullptr);
  model_->GetVao()->release();
  shader_->release();
}
