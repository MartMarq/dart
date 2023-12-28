#include "Uhr.h"


RTC_DATA_ATTR int Welche = 1;

void Uhr::drawWatchFace() {

  // reset step counter at midnight
  if (currentTime.Hour == 00 && currentTime.Minute == 00) {
    sensor.resetStepCounter();
    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
  }

  //time adjustment start
  //if you don't need a time adjustment, just delete all lines between time adjustment start and end
  //my watchy runs too fast (15 seconds and 500 ms a day), this code set back time each day by 15 seconds and 500 ms

  if (currentTime.Hour == 00 && currentTime.Minute == 30) {

    RTC.read(currentTime);
    int8_t sekunde = currentTime.Second;
    int8_t minute = currentTime.Minute;
    int8_t hour = currentTime.Hour;
    int8_t day = currentTime.Day;
    int8_t month = currentTime.Month;
    int8_t year = tmYearToY2k(currentTime.Year);

    delay(15500);

    tmElements_t tm;
    tm.Month = month;
    tm.Day = day;
    tm.Year = y2kYearToTm(year);
    tm.Hour = hour;
    tm.Minute = minute;
    tm.Second = sekunde;

    RTC.set(tm);
  }
  // time adjustment end

  int stundeA = currentTime.Hour;
  int minuteA = currentTime.Minute;
  float VBAT = getBatteryVoltage();
  int n = 0;
  int dart[21] = { 0,1,8,10,3,19,5,12,14,17,6,15,18,4,16,7,13,9,2,11,20 };
  const uint8_t* tagx[] = { xsun, xmon, xtue, xwed, xthu, xfri, xsat, xsun };
  const uint8_t* monx[] = { xjan, xfeb, xmar, xapr, xmay, xjun, xjul, xaug, xsep, xoct, xnov, xdec };

  uint32_t Schritt = sensor.getCounter();
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  int16_t xc, yc, hour ;
  uint16_t wc, hc;
  double winke, lange, cx1,cy1;

  if (Welche == 2) {
    display.setFont(&FreeSans40pt7b);

    display.setCursor(0, 60);

    if (stundeA < 10) {
      display.print("0");
    }
    display.print(stundeA);
    display.print(":");
    if (minuteA < 10) {
      display.print("0");
    }
    display.println(minuteA);

    display.setFont(&FreeSans16pt7b);

    int16_t x1, y1;
    uint16_t w, h;

    String dayOfWeek = dayShortStr(currentTime.Wday);
    display.getTextBounds(dayOfWeek, 5, 85, &x1, &y1, &w, &h);

    display.setCursor(50, 125);
    display.println(dayOfWeek);

    String month = monthShortStr(currentTime.Month);
    display.getTextBounds(month, 60, 110, &x1, &y1, &w, &h);
    display.setCursor(50, 95);
    display.println(month);

    display.setCursor(5, 95);
    if (currentTime.Day < 10) {
      display.print("0");
    }
    display.println(currentTime.Day);

    display.drawBitmap(5, 175, steps, 19, 23, GxEPD_BLACK);
    display.drawBitmap(5, 145, akku, 30, 20, GxEPD_BLACK);
    display.setCursor(50, 195);
    display.println(Schritt);

    display.setCursor(50, 165);
    uint8_t Protz = min<int>(100 - ((4.20 - VBAT) * 100), 100);
    Protz = max<int>(Protz, 0);
    display.print(Protz);
    display.print(" %");

  } else {

   
    int WTag = currentTime.Wday - 1;
    display.drawBitmap(0, 155, tagx[WTag], 45, 45, GxEPD_BLACK);

    WTag = currentTime.Month - 1;
    display.drawBitmap(155, 155, monx[WTag], 45, 45, GxEPD_BLACK);

    display.setFont(&FreeMonoBold16pt7b);
    display.setCursor(5, 22);
    WTag = currentTime.Day;
    WTag = WTag / 10;
    display.println(WTag);
    WTag = currentTime.Day - (WTag * 10);
    display.setCursor(177, 22);
    display.println(WTag);


    display.drawBitmap(0, 0, xdart, 200, 200, GxEPD_BLACK);
    hour = dart[stundeA];
    if (stundeA > 0) {
        winke = (hour * 18);
        lange = ((winke + 180) * 71) / 4068.0;
        cx1 = 100 - (sin(lange) * 35);
        cy1 = 100 + (cos(lange) * 35);

        if (hour & 1) {
            display.fillCircle(cx1, cy1, 3, GxEPD_WHITE);
            display.drawCircle(cx1, cy1, 4, GxEPD_BLACK);
            display.drawLine(cx1 - 5, cy1, cx1 + 5, cy1, GxEPD_BLACK);
            display.drawLine(cx1, cy1 - 5, cx1, cy1 + 5, GxEPD_BLACK);
        }
        else {
            display.fillCircle(cx1, cy1, 3, GxEPD_BLACK);
            display.drawCircle(cx1, cy1, 4, GxEPD_WHITE);
            display.drawLine(cx1 - 5, cy1, cx1 + 5, cy1, GxEPD_WHITE);
            display.drawLine(cx1, cy1 - 5, cx1, cy1 + 5, GxEPD_WHITE);
        }
    }

    n = minuteA;
    
    if (n >= 40) {
        display.fillCircle(100, 28, 3, GxEPD_WHITE);
        display.drawCircle(100, 28, 4, GxEPD_BLACK);
        display.drawLine(95, 28, 105, 28, GxEPD_BLACK);
        display.drawLine(100, 23, 100, 33, GxEPD_BLACK);
        n = n - 40;
    }

    if (n >= 20) {
        display.fillCircle(100, 45, 3, GxEPD_BLACK);
        display.drawCircle(100, 45, 4, GxEPD_WHITE);
        display.drawLine(95, 45, 105, 45, GxEPD_WHITE);
        display.drawLine(100, 40, 100, 50, GxEPD_WHITE);
        n = n - 20;
    }

    if (n > 0) {
        hour = dart[n];
        winke = (hour * 18);
        lange = ((winke + 180) * 71) / 4068.0;
        cx1 = 100 - (sin(lange) * 63);
        cy1 = 100 + (cos(lange) * 63);

        if (hour & 1) {
            display.fillCircle(cx1, cy1, 3, GxEPD_WHITE);
            display.drawCircle(cx1, cy1, 4, GxEPD_BLACK);
            display.drawLine(cx1 - 5, cy1, cx1 + 5, cy1, GxEPD_BLACK);
            display.drawLine(cx1, cy1 - 5, cx1, cy1 + 5, GxEPD_BLACK);
        }
        else {
            display.fillCircle(cx1, cy1, 3, GxEPD_BLACK);
            display.drawCircle(cx1, cy1, 4, GxEPD_WHITE);
            display.drawLine(cx1 - 5, cy1, cx1 + 5, cy1, GxEPD_WHITE);
            display.drawLine(cx1, cy1 - 5, cx1, cy1 + 5, GxEPD_WHITE);
        }
    }


  }
}


void Uhr::handleButtonPress() {
  uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();
  if (IS_DOUBLE_TAP) {
    while (!sensor.getINT()) {
    }
  }

if (IS_BTN_RIGHT_UP) {
    vibrate();
    Welche = Welche + 1;
    if (Welche > 2) {
      Welche = 1;
    }
    RTC.read(currentTime);
    drawWatchFace();
    showWatchFace(true);
  } else if (IS_BTN_RIGHT_DOWN) {
    vibrate();
    Welche = Welche - 1;
    if (Welche < 1) {
      Welche = 2;
    }
    RTC.read(currentTime);
    drawWatchFace();
    showWatchFace(true);
  } else {
    Watchy::handleButtonPress();
  }
}

void Uhr::vibrate() {

  sensor.enableFeature(BMA423_WAKEUP, false);
  pinMode(VIB_MOTOR_PIN, OUTPUT);

  delay(50);
  digitalWrite(VIB_MOTOR_PIN, true);
  delay(50);
  digitalWrite(VIB_MOTOR_PIN, false);

  sensor.enableFeature(BMA423_WAKEUP, true);
}
