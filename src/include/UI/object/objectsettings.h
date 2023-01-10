#ifndef OBJECTSETTINGS_H
#define OBJECTSETTINGS_H

#include <QDialog>

namespace Ui {  // NOLINT
class ObjectSettings;
}

class ObjectSettings : public QDialog {
  Q_OBJECT

 public:
  explicit ObjectSettings(QWidget *parent = nullptr);
  ~ObjectSettings() override;

 private:
  Ui::ObjectSettings *ui;  // NOLINT
};

#endif  // OBJECTSETTINGS_H
