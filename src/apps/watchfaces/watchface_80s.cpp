#include "./apps/watchfaces/watchface_80s.h"
#include "./fonts/DS_DIGIB37pt7b.h"
#include "./fonts/DS_DIGIB15pt7b.h"

#include <gfx_util.h>
#include <time.h>
#include <string>
#include <Tone32.h> 

bool sleepblock;
int BKGND = BLACK;

//HW pin cofiguration for piezo and vibration motor
#define BUZZER_PIN 2
#define VIBRATION_PIN 14
#define BUZZER_CHANNEL 0

//colors
//#define PINKY rgb565(255, 0, 150)     //Pink
#define PINKY rgb565(255, 84, 0)        //Orange
//#define DARKPINK rgb565(85, 0, 85)    //dark pink
#define DARKPINK rgb565(47, 60, 79)     //dark blue
//#define BLUISH rgb565(0, 170, 255)    //Blue v.1 (for pink)
#define BLUISH rgb565(1, 219, 250)      //Blue v.2 (for orange)
#define MINTY rgb565(1, 250, 190)       //Green
#define COLOR_ALERT rgb565(100, 0, 100)  

//Variables for timer
int period2 = 60000;
int period3 = 500;
unsigned long time_now2 = 0;
unsigned long time_now3 = 0;
unsigned long time_now4 = 0;

bool countdown = false;
bool switchy = false;
uint16_t vibrate2= 200;
int TONE2 = 4699;

int timeremaining = 5;
int switchyremaining = 6;

int countdownicon = 3;
int countdowniconcolor = BLUISH;
int cix = 40;
int ciy = 163;


//Variables for alarm clock
bool alarm_is_on = true;
int almhour = 7;
int almminute = 20;
const int daym = 1440;

int COLOR_ALMICON;
int COLOR_ALMHOUR;
int COLOR_ALMMINUTE;

#define aix 110
#define aiy 160

int timeleft;

//variables for choice switcher
int choice = 1;
int max_choice = 5;

//variables for power
unsigned long time_now_power = 0;
long appOnScreenSince2 = 0;   //probably does nothing now



using std::string;

namespace eighties_watchface{

void beep()
  {
    for (int i = 0; i <= 1; i++)
    {
      digitalWrite (VIBRATION_PIN, HIGH);
      BKGND = COLOR_ALERT;
      for (int i2 = 0; i2 <= 3; i2++)
      {
        tone(BUZZER_PIN, TONE2, 100, BUZZER_CHANNEL);
        noTone(BUZZER_PIN, BUZZER_CHANNEL);
        delay(50);
      }
      digitalWrite (VIBRATION_PIN, LOW);
      delay (150);
    }
    delay(250);  
  }

  void notifybox (OswHal* hal)
      {
        hal->gfx()->fillRFrame(32, 77, 176, 86, 10, BKGND);
        hal->gfx()->fillRFrame(35, 80, 170, 80, 10, rgb565(44, 44, 44));
        hal->gfx()->drawRFrame(35, 80, 170, 80, 10, PINKY);

        hal->gfx()->fillRFrame(52, 130, 138, 14, 7, rgb565(22, 22, 22));
        hal->gfx()->drawRFrame(52, 130, 138, 14, 7, PINKY);
      }


 void print_watchface(OswHal* hal){

   //TIME AND DATE-----------------------------------------------------------
    //GetTime
    uint32_t second = 0;
    uint32_t minute = 0;
    uint32_t hour   = 0;

    hal->getLocalTime(&hour, &minute, &second);
    

    //GetDate
    string day = "";
    uint32_t dayInt = 0;
    uint32_t monthInt = 0;
    uint32_t yearInt = 0;
    int charLen = 2;

    hal->getWeekdayString(charLen, &day);
    hal->getDate(&dayInt, &monthInt, &yearInt);

    char date_Array[charLen + 1];
    strcpy(date_Array, day.c_str());    

    //Clear display
    hal->gfx()->fill(BKGND);
      
    //DisplayTime
    
    hal->gfx()->setFont(&DS_DIGIB37pt7b);
    hal->gfx()->setTextColor(PINKY,BKGND);
    hal->gfx()->setTextCursor(35, 142);
    hal->gfx()->printDecimal(hour, 2);

    if(second % 2 == 0)
    {
      hal->gfx()->setTextColor(DARKPINK,BKGND);
    }

    else
    {
      hal->gfx()->setTextColor(BLUISH,BKGND);
    }

    hal->gfx()->print(":");
    hal->gfx()->setTextColor(PINKY,BKGND);
    hal->gfx()->printDecimal(minute, 2);
    hal->gfx()->setFont(&DS_DIGIB15pt7b);
    hal->gfx()->printDecimal(second, 2);

    //DisplayDate
    hal->gfx()->setFont(&DS_DIGIB15pt7b);
    hal->gfx()->setTextColor(BLUISH,BKGND);
    
    hal->gfx()->setTextCursor(38, 75);

    hal->gfx()->print(date_Array);
    hal->gfx()->print(", ");
    hal->gfx()->printDecimal(dayInt, 2);
    hal->gfx()->print(".");
    hal->gfx()->printDecimal(monthInt, 2);
    hal->gfx()->print(".");
    hal->gfx()->print(yearInt);

    //Display Steps
    hal->gfx()->setTextBottomAligned();
    hal->gfx()->setTextCursor(80, 222);
    hal->gfx()->printDecimal(hal->getStepCount(), 5);
 
    //Display graphics
    hal->gfx()->drawHLine(45, 45, 150, DARKPINK);   hal->gfx()->drawLine(0, 0, 45, 45, DARKPINK);   hal->gfx()->drawLine(195, 45, 240, 0, DARKPINK);
    hal->gfx()->drawHLine(45, 46, 150, DARKPINK);   hal->gfx()->drawLine(0, 1, 45, 46, DARKPINK);   hal->gfx()->drawLine(195, 46, 240, 1, DARKPINK);


    hal->gfx()->drawHLine(0, 85, 240, DARKPINK);
    hal->gfx()->drawHLine(0, 86, 240, DARKPINK);

    hal->gfx()->drawHLine(0, 153, 240, DARKPINK);
    hal->gfx()->drawHLine(0, 154, 240, DARKPINK);

    hal->gfx()->drawHLine(45, 193, 150, DARKPINK);  hal->gfx()->drawLine(0, 239, 45, 193, DARKPINK);  hal->gfx()->drawLine(195, 193, 240, 239, DARKPINK);
    hal->gfx()->drawHLine(45, 194, 150, DARKPINK);  hal->gfx()->drawLine(0, 240, 45, 194, DARKPINK);  hal->gfx()->drawLine(195, 194, 240, 240, DARKPINK);


    //CHOICE SWITCHER--------------------------------------------------
    if (hal->btnHasGoneDown(BUTTON_2) && choice < max_choice)
    {
      choice ++;
    }

    else if(hal->btnHasGoneDown(BUTTON_2) && choice == max_choice)
    {
      choice = 0;
    }

    //TIMER - COUNTDOWN------------------------------------------------------

      //a switchy thing - a workaround I made to prevent timer switching between on/off like crazy, so some time must pass before timer can be stopped/started
      if(switchyremaining == 0) 
      {
        switchy = false;
      }
    
      else if (millis() > time_now4 + period3)
      {
        time_now4 = millis();
        switchyremaining --;
      }  
      
      //start - stop the timer
      if(choice == 2)
      {
        if (hal->btnIsDownSince(BUTTON_3) > DEFAULTLAUNCHER_LONG_PRESS && countdown == true && switchy == false)
        {
          countdown = false;
          timeremaining = 10;
          switchy = true;
          switchyremaining = 4;
        }

        else if(hal->btnIsDownSince(BUTTON_3) > DEFAULTLAUNCHER_LONG_PRESS && countdown == false && switchy == false)
        {
          countdown = true;
          if(timeremaining>5)
          {
            timeremaining = timeremaining-5;
          }
          else
          {
            timeremaining = 60;
          }
          switchy = true;
          switchyremaining = 4;
        }
      }

    //add time when paused
    if (countdown == false )
    {
      if(choice == 2)
      {
        if (hal->btnHasGoneDown(BUTTON_3) && timeremaining < 60)
        {
          timeremaining = timeremaining+5;
        }

        
        else if (hal->btnHasGoneDown(BUTTON_3) && timeremaining >= 60) 
        {
          timeremaining = 5;
        }
      }
    }
    
    //Countdown if run is active - this part will be done differently in the future
      if(countdown == true)
      {
        sleepblock = true;
        countdowniconcolor = MINTY;

        if (timeremaining == 0)
        {
          countdownicon = 1;
          countdowniconcolor = RED;
          
          if (hal->btnIsDown(BUTTON_1) || hal->btnIsDown(BUTTON_2) || hal->btnIsDown(BUTTON_3))
          {
            countdown = false;
            timeremaining = 10;
            BKGND = BLACK;
          }
          hal->displayOn();
          beep();
        }

        else if (millis() > time_now2 + period2)
        {
          time_now2 = millis();
          timeremaining --;
          BKGND = BLACK;
        } 

        else if(millis() > time_now3 + period3)
        {
          time_now3 = millis();
          BKGND = BLACK;
        
          //hourglass icon animation
          if(countdownicon > 1)
          {
            countdownicon --;
          }
          
          else
          {
            countdownicon = 3;
          }
        }
        
      }

      else
      {
        countdowniconcolor = BLUISH;
        countdownicon = 1;
        sleepblock = false;
        BKGND = BLACK;
      }

    //Display countdown number in minutes
    
    hal->gfx()->setTextColor(countdowniconcolor, BKGND);
    hal->gfx()->setTextLeftAligned();
    hal->gfx()->setTextCursor(cix+20, ciy+19);         
    hal->gfx()->print(timeremaining);


    //Display horglass icon base
    hal->gfx()->drawFrame(cix-1, ciy-3, 16, 3, countdowniconcolor);
    hal->gfx()->drawTriangle(cix, ciy, cix+14, ciy, cix+7, ciy+11, countdowniconcolor);
    
    ciy = ciy + 21;
    hal->gfx()->drawTriangle(cix, ciy, cix+14, ciy, cix+7, ciy-11, countdowniconcolor);
    hal->gfx()->drawFrame(cix-1, ciy, 16, 3, countdowniconcolor);
    ciy = ciy - 21; 

    //display hourglass icon sand

    if (countdownicon == 3)
    {
      hal->gfx()->drawPixel(cix+7, ciy+8, countdowniconcolor);

      hal->gfx()->drawPixel(cix+6, ciy+6, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+6, countdowniconcolor);

      hal->gfx()->drawPixel(cix+5, ciy+4, countdowniconcolor);
      hal->gfx()->drawPixel(cix+7, ciy+4, countdowniconcolor);
      hal->gfx()->drawPixel(cix+9, ciy+4, countdowniconcolor);

      hal->gfx()->drawPixel(cix+4, ciy+2, countdowniconcolor);
      hal->gfx()->drawPixel(cix+6, ciy+2, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+2, countdowniconcolor);
      hal->gfx()->drawPixel(cix+10, ciy+2, countdowniconcolor);
    }
    
    if (countdownicon == 2)
    {
      hal->gfx()->drawPixel(cix+7, ciy+8, countdowniconcolor);

      hal->gfx()->drawPixel(cix+6, ciy+6, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+6, countdowniconcolor);

      hal->gfx()->drawPixel(cix+5, ciy+17, countdowniconcolor);
      hal->gfx()->drawPixel(cix+7, ciy+17, countdowniconcolor);
      hal->gfx()->drawPixel(cix+9, ciy+17, countdowniconcolor);

      hal->gfx()->drawPixel(cix+4, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+6, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+10, ciy+19, countdowniconcolor);
    }

    if (countdownicon == 1)
    {
      hal->gfx()->drawPixel(cix+7, ciy+13, countdowniconcolor);

      hal->gfx()->drawPixel(cix+6, ciy+15, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+15, countdowniconcolor);

      hal->gfx()->drawPixel(cix+5, ciy+17, countdowniconcolor);
      hal->gfx()->drawPixel(cix+7, ciy+17, countdowniconcolor);
      hal->gfx()->drawPixel(cix+9, ciy+17, countdowniconcolor);

      hal->gfx()->drawPixel(cix+4, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+6, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+8, ciy+19, countdowniconcolor);
      hal->gfx()->drawPixel(cix+10, ciy+19, countdowniconcolor);
    }
    //draw frame around the whole widget when it's adjustable
    if(choice == 2)
    {
      hal->gfx()->drawRFrame(cix-9, 156, 65, 36, 5, PINKY);
    }

    //ALARM CLOCK-----------------------------------------------------------
    //Alarm settings
    COLOR_ALMICON = BLUISH;
    if (choice == 3) 
    {
      COLOR_ALMICON = PINKY;
    
      if (hal->btnHasGoneDown(BUTTON_3))
      {
        if(alarm_is_on == false)
        {
          alarm_is_on = true;
          max_choice = 5;   //when alarm is active it's possible to set the alarm time
        }

        else
        {
          alarm_is_on = false;
          max_choice = 3; ////when alarm is inactive it isn't possible to set the alarm time
        }
      }
    }

    COLOR_ALMHOUR = BLUISH;
    if(choice == 4)
    {
      COLOR_ALMHOUR = PINKY;
    
      if (hal->btnHasGoneDown(BUTTON_3))
      {
        if(almhour < 23)
        {almhour ++;}

        else
        {almhour = 0;}
        
      }
    }

    COLOR_ALMMINUTE = BLUISH;
    if(choice == 5)
    {
      COLOR_ALMMINUTE = PINKY;

      if (hal->btnHasGoneDown(BUTTON_3))
      {
        if(almminute < 59)
        {almminute ++;}

        else
        {almminute = 0;}
        
      }
    }

    //Alarm logic
    int nowm = (hour*60 + minute);
    int almm = (almhour*60 + almminute);

    if(almm>nowm)
    {
      timeleft = ((almm - nowm)*60000000);
    }

    else
    {
      timeleft = ((daym - nowm + almm)*60000000);
    }

    if(hour == almhour && alarm_is_on == true)
    {
      if(minute == almminute)
      {
        COLOR_ALMICON = RED;
        beep();
        if (hal->btnIsDown(BUTTON_1) || hal->btnIsDown(BUTTON_2) || hal->btnIsDown(BUTTON_3))
        {
          alarm_is_on = false;
          max_choice = 3;
          BKGND = BLACK;
        }
        
      }
      else if (BKGND == COLOR_ALERT)
      {
        BKGND = BLACK;
      }
    }

    //display the bell icon
    if(alarm_is_on == true && !(choice == 3))
    {
      COLOR_ALMICON = MINTY;
    }
    
    hal->gfx()->drawCircle(aix+11, aiy+3, 3, COLOR_ALMICON);
    hal->gfx()->fillCircle(aix+11, aiy+10, 7, COLOR_ALMICON);
    hal->gfx()->fillFrame(aix+4, aiy+11, 15, 6, COLOR_ALMICON);
    hal->gfx()->fillFrame(aix+3, aiy+17, 17, 4, COLOR_ALMICON);
    hal->gfx()->fillFrame(aix+2, aiy+20, 19, 2, COLOR_ALMICON);
    hal->gfx()->fillFrame(aix+1, aiy+22, 21, 2, COLOR_ALMICON);
    hal->gfx()->fillCircle(aix+11, aiy+23, 4, COLOR_ALMICON);

    if(alarm_is_on == false)
    { 
      hal->gfx()->drawLine(aix+4, aiy+1, aix+21, aiy+26, PINKY);
      hal->gfx()->drawLine(aix+3, aiy+1, aix+20, aiy+26, PINKY);
      hal->gfx()->drawLine(aix+2, aiy+1, aix+19, aiy+26, BKGND);
      hal->gfx()->drawLine(aix+1, aiy+1, aix+18, aiy+26, BKGND);
      hal->gfx()->drawLine(aix+0, aiy+1, aix+17, aiy+26, BKGND);
    }

    hal->gfx()->setFont(&DS_DIGIB15pt7b);
    hal->gfx()->setTextColor(BLUISH, BKGND);
    hal->gfx()->setTextLeftAligned();
    hal->gfx()->setTextCursor(aix+29, aiy+22);    
    hal->gfx()->setTextColor(COLOR_ALMHOUR, BKGND);
    hal->gfx()->printDecimal(almhour, 2);
    hal->gfx()->print(":");
    hal->gfx()->setTextColor(COLOR_ALMMINUTE, BKGND);
    hal->gfx()->printDecimal(almminute, 2);

    if (3 <= choice && choice <= 5)
    {
      hal->gfx()->drawRFrame(aix-5, 156, 107, 36, 5, PINKY);
    }
    
    //BRIGHTNESS
    if(choice == 0)
    {
      if (hal->btnHasGoneDown(BUTTON_3) && hal->screenBrightness()<255) 
      {
        hal->increaseBrightness(50);
      } 

      else if(hal->btnHasGoneDown(BUTTON_3) && hal->screenBrightness()<=255)
      {
        hal->setBrightness(10);
      }

      notifybox(hal);
      

      hal->gfx()->clearFont();
      hal->gfx()->setTextSize(2);
      hal->gfx()->setTextColor(WHITE, rgb565(44, 44, 44));
      hal->gfx()->setTextCursor(58, 105);
      hal->gfx()->setTextMiddleAligned();
      
      hal->gfx()->print("Brightness");

      hal->gfx()->fillRFrame(55, 133, hal->screenBrightness()/2+4, 8, 3, BLUISH);
    }
    //POWER
    //this part of code is taken from another file and probably doesn't work properly

    if (sleepblock == true) 
    {
      long timeout = OswConfigAllKeys::settingDisplayTimeout.get();
      if ((millis() - appOnScreenSince2) > timeout * 1000) 
      {
        if (hal->btnIsDown(BUTTON_1) || hal->btnIsDown(BUTTON_2) || hal->btnIsDown(BUTTON_3))
        {
          appOnScreenSince2 = millis();
        }
        
        else
        {
          Serial.print("display goes off: ");
          Serial.println(timeout);
          hal->displayOff();
        }
      }

      if (hal->btnIsDown(BUTTON_1) || hal->btnIsDown(BUTTON_2) || hal->btnIsDown(BUTTON_3))
      {
        hal->displayOn();
      }
    }

  }

}

void OswAppWatchface80s::setup(OswHal* hal) {
    hal->gfx()->fill(BKGND);

    eighties_watchface::print_watchface(hal);

    hal->requestFlush();
}

void OswAppWatchface80s::loop(OswHal* hal) {
   
  eighties_watchface::print_watchface(hal);
  hal->requestFlush();
}

void OswAppWatchface80s::stop(OswHal* hal) {
  hal->gfx()->clearFont();
}