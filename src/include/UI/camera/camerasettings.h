#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <QDialog>

class GLCamera;

namespace Ui {  // NOLINT
class CameraSettings;
}

class CameraSettings : public QDialog {
  Q_OBJECT

 public:
  explicit CameraSettings(GLCamera *camera, QWidget *parent = nullptr);
  ~CameraSettings() override;

 private:
  Ui::CameraSettings *ui;  // NOLINT
  GLCamera *camera_;
  bool update_immediately_ = true;

 signals:
  void ValueChanged();
};

#endif  // CAMERASETTINGS_H
