#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

#include "opengl/MainGLWindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui = nullptr;
  QLabel *death_count_ = nullptr;
  QLabel *play_time_ = nullptr;
  QLabel *debug_output_ = nullptr;
  QLabel *fps_ = nullptr;
  QLabel *frame_latency_ = nullptr;
  MainGLWindow *main_gl_ = nullptr;

  // android
  QPushButton *button_[4];
  // QDockWidget *dock_;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

 signals:
};

#endif  // MAINWINDOW_H
