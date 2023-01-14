#include "mainwindow.h"

#include <QDockWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <queue>

#include "UI/about/about.h"
#include "UI/camera/camerasettings.h"
#include "UI/light/lightsettings.h"
#include "UI/model/modelsettings.h"
#include "UI/object/objectsettings.h"
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
  // main_gl_->setFixedSize(800, 600);
  // adjustSize();
  // setFixedSize(width(), height());

  // dock_ = new QDockWidget(this);
  // dock_->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable |
  //                    QDockWidget::DockWidgetFloatable);

  button_[0] = new QPushButton("前", this);
  button_[1] = new QPushButton("后", this);
  button_[2] = new QPushButton("左", this);
  button_[3] = new QPushButton("右", this);
  // button_[0]->setFlat(true);
  // button_[1]->setFlat(true);
  // button_[2]->setFlat(true);
  // button_[3]->setFlat(true);
  //  button_[0]->setStyleSheet("background-color: rgba(255, 255, 255, 32);");
  //  button_[1]->setStyleSheet("background-color: rgba(255, 255, 255, 32);");
  //  button_[2]->setStyleSheet("background-color: rgba(255, 255, 255, 32);");
  //  button_[3]->setStyleSheet("background-color: rgba(255, 255, 255, 32);");
  //   dock_->setWindowOpacity(0.5);
  connect(button_[0], &QPushButton::pressed, [&]() { main_gl_->keys_.emplace(Qt::Key_W); });
  connect(button_[1], &QPushButton::pressed, [&]() { main_gl_->keys_.emplace(Qt::Key_S); });
  connect(button_[2], &QPushButton::pressed, [&]() { main_gl_->keys_.emplace(Qt::Key_A); });
  connect(button_[3], &QPushButton::pressed, [&]() { main_gl_->keys_.emplace(Qt::Key_D); });
  connect(button_[0], &QPushButton::released, [&]() { main_gl_->keys_.erase(Qt::Key_W); });
  connect(button_[1], &QPushButton::released, [&]() { main_gl_->keys_.erase(Qt::Key_S); });
  connect(button_[2], &QPushButton::released, [&]() { main_gl_->keys_.erase(Qt::Key_A); });
  connect(button_[3], &QPushButton::released, [&]() { main_gl_->keys_.erase(Qt::Key_D); });

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

  connect(ui->actionobject, &QAction::triggered, [&]() {
    auto s = new ObjectSettings(main_gl_->scene_, this);
    s->show();
  });

  connect(ui->actionmodel, &QAction::triggered, [&]() {
    auto s = new ModelSettings(main_gl_->scene_, main_gl_, this);
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
  //#ifndef Q_OS_ANDROID
  else if (key == Qt::Key_Back) {
    qDebug() << "back key got pressed!";
  }
  //#endif  // Q_OS_ANDROID
  else {
    event->ignore();
  }
}
void MainWindow::resizeEvent(QResizeEvent *event) {
  qDebug() << "resize: " << event;
  auto center = QSize(150, event->size().height() - 200);
  auto size = QSize(80, 80);
  button_[0]->setGeometry(center.width(), center.height() - size.height(), size.width(), size.height());
  button_[1]->setGeometry(center.width(), center.height() + size.height(), size.width(), size.height());
  button_[2]->setGeometry(center.width() - size.width(), center.height(), size.width(), size.height());
  button_[3]->setGeometry(center.width() + size.width(), center.height(), size.width(), size.height());
}
