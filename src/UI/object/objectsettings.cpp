#include "UI/object/objectsettings.h"

#include "ui_objectsettings.h"

ObjectSettings::ObjectSettings(QWidget *parent) : QDialog(parent), ui(new Ui::ObjectSettings) { ui->setupUi(this); }

ObjectSettings::~ObjectSettings() { delete ui; }
