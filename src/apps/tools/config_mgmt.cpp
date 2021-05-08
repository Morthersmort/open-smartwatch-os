
#include "./apps/tools/config_mgmt.h"

#include <config.h>
#include <osw_app.h>
#include <osw_config.h>
#include <osw_config_types.h>
#include <osw_hal.h>

void OswAppConfigMgmt::setup(OswHal* hal) {}

void OswAppConfigMgmt::loop(OswHal* hal) {
  hal->getCanvas()->getGraphics2D()->fill(rgb565(0, 0, 0));
  hal->getCanvas()->setTextColor(rgb565(255, 255, 255));
  hal->getCanvas()->setTextSize(2);

  if (hal->btn3Down()) {
    OswConfig::getInstance()->enableWrite();
    OswConfigAllKeys::appWTFprimaryColor.set(rgb565(20, 220, 40));
    ESP.restart();
    hal->clearBtn2();
  } else if (hal->btn2Down()) {
    OswConfig::getInstance()->reset();
    ESP.restart();
    hal->clearBtn2();
  }

  hal->getCanvas()->setCursor(20, 110);
  hal->getCanvas()->setTextSize(3);
  hal->getCanvas()->print("Config tests");
  hal->getCanvas()->setTextSize(2);
  hal->getCanvas()->setCursor(40, 50);
  hal->getCanvas()->print("Test write...");
  hal->getCanvas()->setCursor(40, 190);
  hal->getCanvas()->print("Reset & Reload");
  hal->requestFlush();
}

void OswAppConfigMgmt::stop(OswHal* hal) {}