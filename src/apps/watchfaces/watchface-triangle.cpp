
#include "./apps/watchfaces/watchface_triangle.h"
#include "./fonts/Lato_Thin24pt7b.h"
#include "./fonts/Lato_Light12pt7b.h"
#include "./fonts/Lato_Light8pt7b.h"
// #define GIF_BG

#include <gfx_util.h>
#include <osw_app.h>
#include <osw_config.h>
#include <osw_config_keys.h>
#include <osw_hal.h>

#ifdef GIF_BG
#include "./apps/_experiments/gif_player.h"
#endif

int trinagleColor = rgb565(238, 37, 96);
int dotColor = rgb565(69, 217, 253);

void OswAppWatchface_triangle::drawWatch(OswHal* hal) {
  hal->gfx()->drawCircle(120, 120, 119, ui->getForegroundColor());
  //hal->gfx()->drawMinuteTicks(120, 120, 116, 112, ui->getForegroundColor());
  hal->gfx()->drawHourTicks(120, 120, 117, 110, ui->getForegroundColor());

  uint32_t second = 0;
  uint32_t minute = 0;
  uint32_t hour = 0;
  hal->getLocalTime(&hour, &minute, &second);

  //Time
  hal->gfx()->setFont(&Lato_Thin24pt7b);
  hal->gfx()->setTextColor(ui->getForegroundColor(),  ui->getBackgroundColor());
  hal->gfx()->setTextCursor(30, 127);
  hal->gfx()->printDecimal(hour, 2);
  hal->gfx()->print(":");
  hal->gfx()->printDecimal(minute, 2);

  //date
  string day = "";
  uint32_t dayInt = 0;
  uint32_t monthInt = 0;
  uint32_t yearInt = 0;
  int charLen = 2;
  hal->getWeekdayString(charLen, &day);
  hal->getDate(&dayInt, &monthInt, &yearInt);

  char date_Array[charLen + 1];
  strcpy(date_Array, day.c_str());

  hal->gfx()->setFont(&Lato_Light12pt7b);
  hal->gfx()->setTextCursor(30, 150);
  hal->gfx()->print(date_Array);
  hal->gfx()->print(", ");
  hal->gfx()->print(dayInt);
  hal->gfx()->print("/");
  hal->gfx()->print(monthInt);
  
  //steps
  uint32_t steps = hal->getStepCount();
  hal->gfx()->setFont(&Lato_Light8pt7b);
  hal->gfx()->setTextCursor(170, 125);
  hal->gfx()->print(steps);
  hal->gfx()->drawArc(190, 120, 0, 360 * (steps / 10800.0), 90, 30, 3, steps > 10800 ? ui->getSuccessColor() : ui->getInfoColor(), true);


  // hours
  uint32_t hx = rpx(120, 82, 360.0 / 12.0 * (1.0 * hour + minute / 60.0));
  uint32_t hy = rpy(120, 82, 360.0 / 12.0 * (1.0 * hour + minute / 60.0));

  // minutes
  uint32_t mx = rpx(120, 82, 360.0 / 60.0 * (1.0 * minute + second / 60.0));
  uint32_t my = rpy(120, 82, 360.0 / 60.0 * (1.0 * minute + second / 60.0));

  // seconds
  uint32_t sx = rpx(120, 82, 360.0 / 60.0 * second);
  uint32_t sy = rpy(120, 82, 360.0 / 60.0 * second);

  //draw
  hal->gfx()->drawTriangle(hx, hy, mx, my, sx, sy, trinagleColor);
  hal->gfx()->fillCircle(hx, hy, 18, ui->getBackgroundColor()); hal->gfx()->fillCircle(hx, hy, 15, dotColor);
  hal->gfx()->fillCircle(mx, my, 13, ui->getBackgroundColor()); hal->gfx()->fillCircle(mx, my, 10, dotColor); 
  hal->gfx()->fillCircle(sx, sy, 8, ui->getBackgroundColor());  hal->gfx()->fillCircle(sx, sy, 5, dotColor);   
}

#ifdef GIF_BG
OswAppGifPlayer* bgGif = new OswAppGifPlayer();
#endif

void OswAppWatchface_triangle::setup(OswHal* hal) {
#ifdef GIF_BG
  bgGif->setup(hal);
#endif
}

void OswAppWatchface_triangle::loop(OswHal* hal) {
  if (hal->btnHasGoneDown(BUTTON_3) && hal->screenBrightness()<255) 
    {
      hal->increaseBrightness(50);
    } 

  else if(hal->btnHasGoneDown(BUTTON_3) && hal->screenBrightness()<=255)
    {
      hal->setBrightness(10);
    }
  
  if (hal->btnHasGoneDown(BUTTON_2)) {
    trinagleColor = rgb565(rand() % 255, rand() % 255, rand() % 255);
    dotColor = rgb565(rand() % 255, rand() % 255, rand() % 255);
  }


  hal->gfx()->fill(ui->getBackgroundColor());

  drawWatch(hal);
  hal->requestFlush();
  hal->gfx()->clearFont();
}

void OswAppWatchface_triangle::stop(OswHal* hal) {
#ifdef GIF_BG
  bgGif->stop(hal);
#endif
}
