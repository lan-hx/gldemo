#ifndef MODELSETTINGS_H
#define MODELSETTINGS_H

#include <QDialog>
#include <QOpenGLWidget>

#include "opengl/GLScene.h"

namespace Ui {  // NOLINT
class ModelSettings;
}

class ModelSettings : public QDialog {
  Q_OBJECT

 public:
  explicit ModelSettings(GLScene *scene, QOpenGLWidget *widget, QWidget *parent = nullptr);
  ~ModelSettings() override;

 private:
  void ReadModel(const std::string &path);
  void UpdatePaths();

 private:
  Ui::ModelSettings *ui;  // NOLINT
  GLScene *scene_;
  std::unordered_map<std::string, GLModel *> &models_;
  std::string current_path_;
  QOpenGLWidget *widget_;
};

#endif  // MODELSETTINGS_H
