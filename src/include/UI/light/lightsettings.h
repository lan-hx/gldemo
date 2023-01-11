#ifndef LIGHTSETTINGS_H
#define LIGHTSETTINGS_H

#include <QDialog>

#include "utils/GLLight.h"

namespace Ui {  // NOLINT
class LightSettings;
}

class LightSettings : public QDialog {
  Q_OBJECT

 public:
  static constexpr const char *LIGHT_TYPE_NAME[] = {"ambient", "directional", "spot"};
  explicit LightSettings(GLLights *lights, QWidget *parent = nullptr);
  ~LightSettings() override;

 private:
  void LightRead(uint64_t id);
  void UpdateIDs();

 private:
  Ui::LightSettings *ui;  // NOLINT
  GLLights *lights_;
  uint64_t current_id_{UINT64_MAX};
};

#endif  // LIGHTSETTINGS_H
