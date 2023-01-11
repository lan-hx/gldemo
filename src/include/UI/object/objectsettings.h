#ifndef OBJECTSETTINGS_H
#define OBJECTSETTINGS_H

#include <QDialog>

#include "opengl/GLObject.h"
#include "opengl/GLScene.h"

namespace Ui {  // NOLINT
class ObjectSettings;
}

class ObjectSettings : public QDialog {
  Q_OBJECT

 public:
  explicit ObjectSettings(GLScene *scene, QWidget *parent = nullptr);
  ~ObjectSettings() override;

 private:
  void UpdateIDs();
  void ReadObject(uint64_t id);

 private:
  Ui::ObjectSettings *ui;  // NOLINT
  GLScene *scene_;
  std::map<uint64_t, GLObject *> &objects_;
  std::unordered_map<std::string, GLModel *> &models_;
  uint64_t current_id_ = UINT64_MAX;
};

#endif  // OBJECTSETTINGS_H
