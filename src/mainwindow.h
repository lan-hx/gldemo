#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

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
  Ui::MainWindow *ui;
  QLabel *death_count_;
  QLabel *play_time_;
  QLabel *debug_output_;
  QLabel *fps_;
  QLabel *frame_latency_;
  MainGLWindow *main_gl_;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

 signals:
};

#endif  // MAINWINDOW_H
