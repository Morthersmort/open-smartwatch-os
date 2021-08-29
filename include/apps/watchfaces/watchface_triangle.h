#ifndef OSW_APP_WATCHFACE_TRIANGLE_H
#define OSW_APP_WATCHFACE_TRIANGLE_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"

class OswAppWatchface_triangle : public OswApp {
 public:
  OswAppWatchface_triangle(void) { ui = OswUI::getInstance(); };
  void setup(OswHal* hal);
  void loop(OswHal* hal);
  void stop(OswHal* hal);
  ~OswAppWatchface_triangle(){};

 private:
  OswUI* ui;
  void drawWatch(OswHal* hal);
};

#endif
