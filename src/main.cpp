#include <QApplication>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  // enable double buffer
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  // enable vsync
  fmt.setSwapInterval(1);

  // Request OpenGL 3.3 core or OpenGL ES 3.0.
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    qDebug("Requesting 3.3 core context");
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
  } else {
    qDebug("Requesting 3.0 context");
    fmt.setVersion(3, 0);
  }

  QSurfaceFormat::setDefaultFormat(fmt);

  MainWindow window;
  window.show();
  return a.exec();  // NOLINT
}
