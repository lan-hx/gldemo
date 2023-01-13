#include "UI/model/modelsettings.h"

#include <QFileDialog>
#include <QMessageBox>

#include "ui_modelsettings.h"

ModelSettings::ModelSettings(GLScene *scene, QOpenGLWidget *widget, QWidget *parent)
    : QDialog(parent), ui(new Ui::ModelSettings), scene_(scene), models_(scene->GetModels()), widget_(widget) {
  ui->setupUi(this);

  // initialize
  ui->model->setEditable(false);
  connect(ui->model, &QComboBox::currentTextChanged, [&](const QString &text) {
    if (!text.isEmpty()) {
      ReadModel(text.toStdString());
    } else {
      current_path_.clear();
    }
  });
  connect(ui->ka_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ka_.setX(value);
    }
  });
  connect(ui->ka_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ka_.setY(value);
    }
  });
  connect(ui->ka_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ka_.setZ(value);
    }
  });
  connect(ui->kd_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].kd_.setX(value);
    }
  });
  connect(ui->kd_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].kd_.setY(value);
    }
  });
  connect(ui->kd_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].kd_.setZ(value);
    }
  });
  connect(ui->ks_x, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ks_.setX(value);
    }
  });
  connect(ui->ks_y, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ks_.setY(value);
    }
  });
  connect(ui->ks_z, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ks_.setZ(value);
    }
  });
  connect(ui->ns, &QDoubleSpinBox::valueChanged, [&](double value) {
    if (!current_path_.empty()) {
      models_[current_path_]->materials_[0].ns_ = value;
    }
  });
  connect(ui->model_select, &QToolButton::clicked, [&]() {
    auto file_name = QFileDialog::getOpenFileName(this, tr("打开模型"), tr(""), tr("Model(*.obj)"));
    if (!file_name.isEmpty()) {
      ui->path->setText(file_name);
    }
  });
  connect(ui->path_button, &QPushButton::clicked, [&]() {
    auto path = ui->path->text();
    if (path.isEmpty()) {
      QMessageBox::critical(this, tr("模型添加失败"), tr("没有指定模型路径"));
      return;
    }
    auto model = new GLModel;
    widget_->makeCurrent();
    if (model->Load(path.toStdString(), {}, scene_->GetShader())) {
      // load successful
      scene_->AddModel(path.toStdString(), model);
      UpdatePaths();
      ui->model->setCurrentText(path);
    } else {
      QMessageBox::critical(this, tr("模型添加失败"), tr("模型添加失败，详见cerr输出。"));
      delete model;
    }
    widget_->doneCurrent();
  });
  connect(ui->texture_select, &QToolButton::clicked, [&]() {
    auto file_name = QFileDialog::getOpenFileName(this, tr("打开贴图"), tr(""), tr("Images(*.png *.jpg)"));
    if (!file_name.isEmpty()) {
      ui->texture->setText(file_name);
    }
  });
  connect(ui->set_texture, &QPushButton::clicked, [&]() {
    if (current_path_.empty()) {
      QMessageBox::critical(this, tr("纹理设置失败"), tr("当前未选择模型"));
      return;
    }
    auto path = ui->texture->text();
    auto model = models_.at(current_path_);
    if (!model->LoadTexture(path.toStdString())) {
      QMessageBox::critical(this, tr("纹理设置失败"), tr("纹理设置失败，详见cerr输出。纹理没有改变。"));
    }
  });

  UpdatePaths();
}

ModelSettings::~ModelSettings() { delete ui; }

void ModelSettings::ReadModel(const std::string &path) {
  assert(models_.contains(path));
  current_path_ = path;
  auto &model = models_[path];
  auto &material = model->materials_[0];
  ui->texture->setText(QString(model->texture_path_.c_str()));
  ui->ka_x->setValue(material.ka_.x());
  ui->ka_y->setValue(material.ka_.y());
  ui->ka_z->setValue(material.ka_.z());
  ui->kd_x->setValue(material.kd_.x());
  ui->kd_y->setValue(material.kd_.y());
  ui->kd_z->setValue(material.kd_.z());
  ui->ks_x->setValue(material.ks_.x());
  ui->ks_y->setValue(material.ks_.y());
  ui->ks_z->setValue(material.ks_.z());
  ui->ns->setValue(material.ns_);
}

void ModelSettings::UpdatePaths() {
  ui->model->clear();
  for (auto &model : models_) {
    ui->model->addItem(QString(model.first.c_str()));
  }
  if (models_.empty()) {
    current_path_.clear();
  }
}
