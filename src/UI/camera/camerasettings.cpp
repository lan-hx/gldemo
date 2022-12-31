#include "UI/camera/camerasettings.h"

#include "camera/GLCamera.h"
#include "ui_camerasettings.h"

CameraSettings::CameraSettings(GLCamera *camera, QWidget *parent)
    : QDialog(parent), ui(new Ui::CameraSettings), camera_(camera) {
  ui->setupUi(this);

  auto update_lambda = [&]() {
    auto pos = camera_->GetPosition();
    ui->pos_x->setValue(pos.x());
    ui->pos_y->setValue(pos.y());
    ui->pos_z->setValue(pos.z());
    auto angles = camera_->GetAngles();
    ui->pitch->setValue(angles.x() / PI * 180);
    ui->yaw->setValue(angles.y() / PI * 180);
    ui->fov->setValue(camera_->GetFOV());
  };
  update_lambda();
  connect(camera_, &GLCamera::ValueChanged, update_lambda);

  connect(this, &CameraSettings::ValueChanged, [&]() {
    if (update_immediately_) {
      camera_->SetParameters({static_cast<float>(ui->pos_x->value()), static_cast<float>(ui->pos_y->value()),
                              static_cast<float>(ui->pos_z->value())},
                             ui->pitch->value() * PI / 180, ui->yaw->value() * PI / 180, ui->fov->value());
    }
  });

  connect(ui->buttonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button) {
    switch (ui->buttonBox->standardButton(button)) {
      case QDialogButtonBox::Ok:
        camera_->SetParameters({static_cast<float>(ui->pos_x->value()), static_cast<float>(ui->pos_y->value()),
                                static_cast<float>(ui->pos_z->value())},
                               ui->pitch->value() * PI / 180, ui->yaw->value() * PI / 180, ui->fov->value());
        close();
        break;
      case QDialogButtonBox::Save:
        camera_->SetParameters({static_cast<float>(ui->pos_x->value()), static_cast<float>(ui->pos_y->value()),
                                static_cast<float>(ui->pos_z->value())},
                               ui->pitch->value() * PI / 180, ui->yaw->value() * PI / 180, ui->fov->value());
        break;
      case QDialogButtonBox::Cancel:
        close();
        break;
      default:
        qDebug() << "invalid button in CameraSettings";
    }
  });

  connect(ui->pos_x, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
  connect(ui->pos_y, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
  connect(ui->pos_z, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
  connect(ui->pitch, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
  connect(ui->yaw, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
  connect(ui->fov, &QDoubleSpinBox::valueChanged, [&](double val) { emit ValueChanged(); });
}

CameraSettings::~CameraSettings() { delete ui; }
