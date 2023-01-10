#include "UI/object/objectsettings.h"

#include <QMessageBox>
#include <map>

#include "ui_objectsettings.h"

ObjectSettings::ObjectSettings(GLScene *scene, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::ObjectSettings),
      scene_(scene),
      objects_(scene->GetObjects()),
      models_(scene->GetModels()) {
  ui->setupUi(this);

  // initialize
  ui->id->setEditable(false);
  ui->model->setEditable(false);
  connect(ui->id, &QComboBox::currentTextChanged, [&](const QString &text) {
    if (!text.isEmpty()) {
      ReadObject(text.toULongLong());
    } else {
      current_id_ = UINT64_MAX;
    }
  });
  connect(ui->model, &QComboBox::currentTextChanged, [&](const QString &text) {
    if (current_id_ != UINT64_MAX) {
      assert(models_.contains(text.toStdString()));
      auto &obj = objects_[current_id_];
      obj->model_ = models_[text.toStdString()];
    }
  });
  connect(ui->position_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.position_.setX(value);
    }
  });
  connect(ui->position_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.position_.setY(value);
    }
  });
  connect(ui->position_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.position_.setZ(value);
    }
  });
  auto change_angle_func = [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.SetAngles(ui->rotation_x->value(), ui->rotation_y->value(),
                                                  ui->rotation_z->value());
    }
  };
  connect(ui->rotation_x, &QDoubleSpinBox::valueChanged, change_angle_func);
  connect(ui->rotation_y, &QDoubleSpinBox::valueChanged, change_angle_func);
  connect(ui->rotation_z, &QDoubleSpinBox::valueChanged, change_angle_func);
  connect(ui->scale_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.scale_.setX(value);
    }
  });
  connect(ui->scale_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.scale_.setY(value);
    }
  });
  connect(ui->scale_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (current_id_ != UINT64_MAX) {
      objects_[current_id_]->transform_.scale_.setZ(value);
    }
  });
  connect(ui->newobj, &QPushButton::clicked, [&]() {
    auto obj = new GLObject((*models_.begin()).second, scene_->GetShader());
    auto id = scene_->AddObject(obj);
    UpdateIDs();
    ui->id->setCurrentText(QString::number(id));
  });

  connect(ui->delobj, &QPushButton::clicked, [&]() {
    if (current_id_ == UINT64_MAX) {
      QMessageBox::critical(this, tr("物体删除失败"), tr("当前未选择物体"));
    } else {
      scene_->RemoveObject(current_id_);
      UpdateIDs();
    }
  });

  for (auto &m : models_) {
    ui->model->addItem(m.first.c_str());
  }
  UpdateIDs();
}

ObjectSettings::~ObjectSettings() { delete ui; }
void ObjectSettings::UpdateIDs() {
  ui->id->clear();
  for (auto &obj : objects_) {
    ui->id->addItem(QString::number(obj.first));
  }
  if (objects_.empty()) {
    current_id_ = UINT64_MAX;
  }
}
void ObjectSettings::ReadObject(uint64_t id) {
  assert(objects_.contains(id));
  current_id_ = id;
  auto &obj = objects_[id];
  ui->model->setCurrentText(obj->model_->GetPath().c_str());
  ui->position_x->setValue(obj->transform_.position_.x());
  ui->position_y->setValue(obj->transform_.position_.y());
  ui->position_z->setValue(obj->transform_.position_.z());
  ui->rotation_x->setValue(obj->transform_.pitch_);
  ui->rotation_y->setValue(obj->transform_.yaw_);
  ui->rotation_z->setValue(obj->transform_.roll_);
  ui->scale_x->setValue(obj->transform_.scale_.x());
  ui->scale_y->setValue(obj->transform_.scale_.y());
  ui->scale_z->setValue(obj->transform_.scale_.z());
}
