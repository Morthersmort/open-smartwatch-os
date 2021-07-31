#ifndef OSW_APP_WATCHFACE_80S_H
#define OSW_APP_WATCHFACE_80S_H

#include <osw_hal.h>
#include <osw_ui.h>

#include "osw_app.h"

class OswAppWatchface80s : public OswApp {
 public:
  OswAppWatchface80s(void){
    ui = OswUI::getInstance();
  };
  void setup(OswHal* hal);
  void loop(OswHal* hal);
  void stop(OswHal* hal);
  ~OswAppWatchface80s(){};

 private:
  bool useMMDDYYYY = false;
  OswUI* ui;
};

#endif
