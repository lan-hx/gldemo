#include <QApplication>
#include <QMessageBox>
#include <QOpenGLContext>
#include <QSurfaceFormat>

#include "mainwindow.h"

bool camera_reverse;

#ifdef WIN32
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 1;         // NOLINT
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;  // NOLINT
}
#endif

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  // enable double buffer
  fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  auto ret = QMessageBox::information(nullptr, "垂直同步", "是否开启垂直同步？", QMessageBox::Yes | QMessageBox::No);
  // vsync
  fmt.setSwapInterval((ret & QMessageBox::Yes) != 0 ? 1 : 0);

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

  ret = QMessageBox::information(nullptr, "视角反转", "是否反转X轴和Y轴？", QMessageBox::Yes | QMessageBox::No);
  camera_reverse = ret & QMessageBox::Yes;

  MainWindow window;
  window.show();
  return a.exec();  // NOLINT
}
