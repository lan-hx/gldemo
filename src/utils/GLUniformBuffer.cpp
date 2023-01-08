//
// Created by lan on 2023/1/8.
//

#include "include/utils/GLUniformBuffer.h"
#include <QOpenGLExtraFunctions>

GLUniformBuffer::GLUniformBuffer(size_t bufferSize, QObject *parent) : QObject(parent) {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glGenBuffers(1, &ubo_);
  f->glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
  f->glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
  f->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
GLUniformBuffer::~GLUniformBuffer() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glDeleteBuffers(1, &ubo_);
}
void GLUniformBuffer::SetShaderBindingPoint(QOpenGLShaderProgram *shader, const char *name, uint32_t index) {
  QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
  auto u = f->glGetUniformBlockIndex(shader->programId(), name);
  f->glUniformBlockBinding(shader->programId(), u, index);
}
void GLUniformBuffer::SetBindingPoint(uint32_t index) {
  QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();
  f->glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo_);
}
void GLUniformBuffer::bind() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
}
void GLUniformBuffer::release() {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void GLUniformBuffer::write(size_t offset, size_t size, void *data) {
  QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
  f->glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
  f->glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
  f->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
