// External libraries (Install them!)
#include <SoftwareSerial.h>
#include <STM32RTC.h>
#include "STM32LowPower.h"

// Project includes
#include "Configuration.hpp"
#include "Pins.hpp"
#include "Controller.hpp"
#include "Door.hpp"


STM32RTC& rtc = STM32RTC::getInstance();
SoftwareSerial sw_serial(PB6, PB7); // RX, TX

Controller controller;
Door door;

void setup() 
{
  controller.init();

  pinMode(PIN_ENDSTOP_GOING_UP_RIGHT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_UP_LEFT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_DOWN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_DOWN_LEFT, INPUT_PULLUP);

  sw_serial.begin(9600);
  delay(5000);
  setTime(); // Set time as compilation time
}

void loop()
{
  // Wake UP!
  sw_serial.begin(9600); // Re-configure serial
  sw_serial.println(""); // Enter

  sw_serial.println("Waked up!");
  sw_serial.print("["); sw_serial.printf("%02d/%02d/%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear()); sw_serial.print(" | ");
  sw_serial.printf("%02d:%02d:%02d.%03d", rtc.getHours(), rtc.getMinutes(), rtc.getSeconds(), rtc.getSubSeconds()); sw_serial.print("]");

  // Check if something has been pressed
  Controller::Key k = controller.get();
  if (k == Controller::Key::Up)
  {
    sw_serial.println("Pressed: Controller::Key::UP");
    door.open();
  }
  else if (k == Controller::Key::Down)
  {
    sw_serial.println("Pressed: Controller::Key::DOWN");  
    door.close();
  }
  else
  {
    // Timeout deep sleep logic
    sw_serial.println("Deep sleep timeout reached");

    uint32_t brightness = analogRead(LDR_PIN);

    bool can_open = rtc.getHours() >= MIN_OPEN_HOUR && rtc.getHours() >= MAX_OPEN_HOUR;
    bool can_close = rtc.getHours() >= MIN_CLOSE_HOUR && rtc.getHours() >= MAX_CLOSE_HOUR;

    // Check time ranges
    if (can_open)
    {
      // Check brightness
      if (brightness >= BRIGHTNESS_OPEN_THRESHOLD)
      {
        if (door.isClose())
        {
          sw_serial.println("Ready to open the door with brightness (%d)", brightness);
          door.open();
        }
        else
        {
          sw_serial.println("The door is already open");
        }
      }
      else
      {
        sw_serial.println("The current brightness (%d) is not enough to open the door", brightness);
      } 
    }
    else if (can_close)
    {
      // Check brightness
      if (brightness <= BRIGHTNESS_CLOSE_THRESHOLD)
      {
        if (door.isOpen())
        {
          sw_serial.println("Ready to close the door with brightness (%d)", brightness);
          door.close();
        }
        else
        {
          sw_serial.println("The door is already closed");
        }
      }
      else
      {
        sw_serial.println("The current brightness (%d) is not too low to close the door", brightness);
      } 
    }
    else
    {
      sw_serial.println("The current hour is outside of the open/close ranges");
    }
  }

  sw_serial.println("Going to sleep...");
  LowPower.deepSleep(DEEP_SLEEP_TIME);
}

void setTime()
{
  const char* compile_date = __DATE__; // "Sep 26 2025"
  const char* compile_time = __TIME__; // "14:32:55"

  int day, year, hour, minute, second;
  char month_str[4];
  int month;

  sscanf(compile_date, "%3s %d %d", month_str, &day, &year);
  sscanf(compile_time, "%d:%d:%d", &hour, &minute, &second);

  if      (strcmp(month_str, "Jan") == 0) month = 1;
  else if (strcmp(month_str, "Feb") == 0) month = 2;
  else if (strcmp(month_str, "Mar") == 0) month = 3;
  else if (strcmp(month_str, "Apr") == 0) month = 4;
  else if (strcmp(month_str, "May") == 0) month = 5;
  else if (strcmp(month_str, "Jun") == 0) month = 6;
  else if (strcmp(month_str, "Jul") == 0) month = 7;
  else if (strcmp(month_str, "Aug") == 0) month = 8;
  else if (strcmp(month_str, "Sep") == 0) month = 9;
  else if (strcmp(month_str, "Oct") == 0) month = 10;
  else if (strcmp(month_str, "Nov") == 0) month = 11;
  else if (strcmp(month_str, "Dec") == 0) month = 12;

  sw_serial.printf("Parsed compile time: %02d/%02d/%04d %02d:%02d:%02d\r\n", day, month, year, hour, minute, second);
  rtc.begin();
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year - 2000); // STM32RTC uses since 2000
  rtc.setHours(hour);
  rtc.setMinutes(minute);
  rtc.setSeconds(second);
  rtc.setSubSeconds(0);
}


