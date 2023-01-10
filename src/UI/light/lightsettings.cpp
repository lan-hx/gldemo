#include "UI/light/lightsettings.h"

#include <QMessageBox>

#include "ui_lightsettings.h"

LightSettings::LightSettings(GLLights *lights, QWidget *parent)
    : QDialog(parent), ui(new Ui::LightSettings), lights_(lights) {
  ui->setupUi(this);

  // initialize
  ui->id->setEditable(false);
  ui->type->setEditable(false);
  connect(ui->id, &QComboBox::currentTextChanged, [&](const QString &text) {
    if (!text.isEmpty()) {
      LightRead(text.toULongLong());
    } else {
      current_id_ = UINT64_MAX;
    }
  });
  connect(ui->type, &QComboBox::currentTextChanged, [&](const QString &text) {
    if (current_id_ != UINT64_MAX) {
      auto &light = lights_->lights_[current_id_];
      uint64_t type = 0;
      for (auto &s : LIGHT_TYPE_NAME) {
        if (text == s) {
          light.type_ = static_cast<GLLightType>(type);
          lights_->Update();
          return;
        }
        ++type;
      }
      throw std::runtime_error(text.toStdString());
    }
  });
  connect(ui->position_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].position_.setX(value);
      lights_->Update();
    }
  });
  connect(ui->position_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].position_.setY(value);
      lights_->Update();
    }
  });
  connect(ui->position_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].position_.setZ(value);
      lights_->Update();
    }
  });
  connect(ui->direction_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].direction_.setX(value);
      lights_->Update();
    }
  });
  connect(ui->direction_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].direction_.setY(value);
      lights_->Update();
    }
  });
  connect(ui->direction_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].direction_.setZ(value);
      lights_->Update();
    }
  });
  connect(ui->color_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].color_.setX(value);
      lights_->Update();
    }
  });
  connect(ui->color_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].color_.setY(value);
      lights_->Update();
    }
  });
  connect(ui->color_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].color_.setZ(value);
      lights_->Update();
    }
  });
  connect(ui->intensity, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].intensity_ = value;
      lights_->Update();
    }
  });
  connect(ui->angle, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].angle_ = value;
      lights_->Update();
    }
  });
  connect(ui->kc, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].kc_ = value;
      lights_->Update();
    }
  });
  connect(ui->kl, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].kl_ = value;
      lights_->Update();
    }
  });
  connect(ui->kq, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      lights_->lights_[current_id_].kq_ = value;
      lights_->Update();
    }
  });
  connect(ui->newambient, &QPushButton::clicked, [&]() {
    if (lights_->lights_.size() >= lights_->max_count_) {
      QMessageBox::critical(this, tr("光源添加失败"), tr("光源已达最大数量"));
      return;
    }
    auto id = lights_->AddLight(GLLights::AddAmbientLight({1.0f, 1.0f, 1.0f}, 0.2f));
    UpdateIDs();
    ui->id->setCurrentText(QString::number(id));
    lights_->Update();
  });
  connect(ui->newdirectional, &QPushButton::clicked, [&]() {
    if (lights_->lights_.size() >= lights_->max_count_) {
      QMessageBox::critical(this, tr("光源添加失败"), tr("光源已达最大数量"));
      return;
    }
    auto id = lights_->AddLight(GLLights::AddDirectionalLight({0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, 1.0f));
    UpdateIDs();
    ui->id->setCurrentText(QString::number(id));
    lights_->Update();
  });
  connect(ui->newspot, &QPushButton::clicked, [&]() {
    if (lights_->lights_.size() >= lights_->max_count_) {
      QMessageBox::critical(this, tr("光源添加失败"), tr("光源已达最大数量"));
      return;
    }
    auto id = lights_->AddLight(GLLights::AddSpotLight({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f},
                                                       1.0f, 1.047198f, 1.0f, 0.0f, 1.0f));
    UpdateIDs();
    ui->id->setCurrentText(QString::number(id));
    lights_->Update();
  });
  connect(ui->deletelight, &QPushButton::clicked, [&]() {
    if (current_id_ == UINT64_MAX) {
      QMessageBox::critical(this, tr("光源删除失败"), tr("当前未选择光源"));
    } else {
      lights_->RemoveLight(current_id_);
      UpdateIDs();
    }
    lights_->Update();
  });

  ui->type->addItem(tr(LIGHT_TYPE_NAME[0]));
  ui->type->addItem(tr(LIGHT_TYPE_NAME[1]));
  ui->type->addItem(tr(LIGHT_TYPE_NAME[2]));
  UpdateIDs();
}

LightSettings::~LightSettings() { delete ui; }

void LightSettings::LightRead(uint64_t id) {
  assert(lights_->lights_.contains(id));
  current_id_ = id;
  auto &light = lights_->lights_[id];
  ui->type->setCurrentText(LIGHT_TYPE_NAME[light.type_]);
  ui->position_x->setValue(light.position_.x());
  ui->position_y->setValue(light.position_.y());
  ui->position_z->setValue(light.position_.z());
  ui->direction_x->setValue(light.direction_.x());
  ui->direction_y->setValue(light.direction_.y());
  ui->direction_z->setValue(light.direction_.z());
  ui->color_x->setValue(light.color_.x());
  ui->color_y->setValue(light.color_.y());
  ui->color_z->setValue(light.color_.z());
  ui->intensity->setValue(light.intensity_);
  ui->angle->setValue(light.angle_);
  ui->kc->setValue(light.kc_);
  ui->kl->setValue(light.kl_);
  ui->kq->setValue(light.kq_);
}
void LightSettings::UpdateIDs() {
  ui->id->clear();
  for (auto &light : lights_->lights_) {
    ui->id->addItem(QString::number(light.first));
  }
  if (lights_->lights_.empty()) {
    current_id_ = UINT64_MAX;
  }
}
