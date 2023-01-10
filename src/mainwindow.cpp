#include "mainwindow.h"

//#include <QFileDialog>
//#include <QMessageBox>
//#include <QMovie>
#include <QKeyEvent>
#include <queue>

#include "UI/about/about.h"
#include "UI/camera/camerasettings.h"
#include "UI/light/lightsettings.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // status bar
  debug_output_ = new QLabel(this);
  ui->statusbar->addWidget(debug_output_);
  fps_ = new QLabel(this);
  fps_->setFixedWidth(80);
  ui->statusbar->addPermanentWidget(fps_);
  frame_latency_ = new QLabel(this);
  frame_latency_->setFixedWidth(120);
  ui->statusbar->addPermanentWidget(frame_latency_);

  main_gl_ = new MainGLWindow(this);
  setCentralWidget(main_gl_);
  main_gl_->setFixedSize(800, 600);
  adjustSize();
  setFixedSize(width(), height());

  connect(main_gl_, &MainGLWindow::UpdateInfo, [&](int64_t nsec, const char *debug_info) {
    static std::deque<int64_t> fps_indecator;
    fps_indecator.push_back(nsec);
    debug_output_->setText(debug_info);
    if (fps_indecator.size() >= 10) {
      auto nsecs = std::accumulate(fps_indecator.begin(), fps_indecator.end(), 0ll);
      fps_->setText("FPS: " + QString::number(fps_indecator.size() * 1e9 / nsecs, 'f', 2));
      fps_indecator.clear();
    }
    frame_latency_->setText("latency: " + QString::number(nsec / 1e6, 'f', 2) + "ms");
  });

  connect(ui->actioncamera, &QAction::triggered, [&]() {
    auto s = new CameraSettings(main_gl_->scene_->GetCamera(), this);
    s->show();
  });

  connect(ui->actionlight, &QAction::triggered, [&]() {
    auto s = new LightSettings(main_gl_->scene_->GetLights(), this);
    s->show();
  });

  connect(ui->actionscreenshot, &QAction::triggered, [&]() { main_gl_->TakeScreenShot(); });

  // connect(ui->actionproject_info, &QAction::triggered, [&]() {
  //   game_ui_->Pause();
  //   About ab(this);
  //   ab.exec();
  //   game_ui_->Continue();
  // });
  // connect(ui->actionauthor, &QAction::triggered, [&]() {});
  // connect(ui->actionpause, &QAction::triggered, [&]() {});
  // connect(ui->actionstop, &QAction::triggered, [&]() {});
  // connect(ui->actionopen, &QAction::triggered, [&]() {});
  // connect(ui->actionexit, &QAction::triggered, [&]() {);
}

MainWindow::~MainWindow() { delete ui; }
void MainWindow::paintEvent(QPaintEvent *event) { main_gl_->update(); }
void MainWindow::keyPressEvent(QKeyEvent *event) {
  auto key = event->key();
  if (key == Qt::Key_Q) {
    close();
  }
}
