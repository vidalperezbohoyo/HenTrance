// External libraries (Install them!)
#include <SoftwareSerial.h>
#include <STM32RTC.h>
#include "STM32LowPower.h"
#include <L298NX2.h>

// Proyect includes
#include "Configuration.hpp"
#include "Pins.hpp"

STM32RTC& rtc = STM32RTC::getInstance();
SoftwareSerial sw_serial(PB6, PB7); // RX, TX

L298NX2 motors(PIN_IN1_MOTORS, PIN_IN2_MOTORS, PIN_IN3_MOTORS, PIN_IN4_MOTORS);

Controller controller;
Door door;

bool door_open = true;


volatile bool manual_button_up_pressed = false;
volatile bool manual_button_down_pressed = false;


void setup() 
{
  // Setup pins
  pinMode(PIN_UP_MANUAL_BTN, INPUT_PULLUP);
  pinMode(PIN_DOWN_MANUAL_BTN, INPUT_PULLUP);

  pinMode(PIN_ENDSTOP_GOING_UP_RIGHT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_UP_LEFT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_DOWN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_ENDSTOP_GOING_DOWN_LEFT, INPUT_PULLUP);

  LowPower.attachInterruptWakeup(PIN_UP_MANUAL_BTN, ISR_onManualUpButtonPressed, FALLING, SLEEP_MODE);
  LowPower.attachInterruptWakeup(PIN_DOWN_MANUAL_BTN, ISR_onManualDownButtonPressed, FALLING, SLEEP_MODE);

  sw_serial.begin(9600);
  delay(5000);
  setTime(); // Set time as compilation time
  //while(1) testPins();
  testLDR();
}

void testPins()
{
  int manual_up = digitalRead(PIN_UP_MANUAL_BTN);
  int manual_down = digitalRead(PIN_DOWN_MANUAL_BTN);
  int endstop_up_right = digitalRead(PIN_ENDSTOP_GOING_UP_RIGHT);
  int endstop_up_left = digitalRead(PIN_ENDSTOP_GOING_UP_LEFT);
  int endstop_down_right = digitalRead(PIN_ENDSTOP_GOING_DOWN_RIGHT);
  int endstop_down_left = digitalRead(PIN_ENDSTOP_GOING_DOWN_LEFT);


  sw_serial.print("UP: "); sw_serial.print(manual_up); sw_serial.print(" ");
  sw_serial.print("DOWN: "); sw_serial.print(manual_down); sw_serial.print(" ");
  sw_serial.print("ENDSTOP UP RIGHT: "); sw_serial.print(endstop_up_right); sw_serial.print(" ");
  sw_serial.print("ENDSTOP UP LEFT: "); sw_serial.print(endstop_up_left); sw_serial.print(" ");
  sw_serial.print("ENDSTOP DOWN RIGHT: "); sw_serial.print(endstop_down_right); sw_serial.print(" ");
  sw_serial.print("ENDSTOP DOWN LEFT: "); sw_serial.print(endstop_down_left); sw_serial.print(" ");
  sw_serial.println("");

}

void testLDR()
{
  while(true)
  {
    uint32_t brightness = getBrightness();
    sw_serial.println(brightness);
  }
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
  Key k = controller.get();
  if (k == Controller::Key::UP)
  {
    sw_serial.println("Pressed: Controller::Key::UP");
    door.open();
  }
  else if (k == Controller::Key::DOWN)
  {
    sw_serial.println("Pressed: Controller::Key::DOWN");  
    door.close();
  }
  else
  {
    // Timeout deep sleep logic
    sw_serial.println("Deep sleep timeout reached");

    uint32_t brightness = getBrightness();

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

uint32_t getBrightness()
{
  analogRead(LDR_PIN); // Pull the brightness
  delay(200);
  return analogRead(LDR_PIN); // Clear brightness reading
}

void close()
{
  sw_serial.println("Closing door...");
  motors.backward();

  bool done = false;
  do
  {
    
    bool left_pressed = (digitalRead(PIN_ENDSTOP_GOING_DOWN_LEFT) == LOW);
    bool right_pressed = (digitalRead(PIN_ENDSTOP_GOING_DOWN_RIGHT) == LOW);

    if (left_pressed)
    {
      motors.stopA();
    }
    if (right_pressed)
    {
      motors.stopB();
    }

    done = !motors.isMovingA() && !motors.isMovingB();
  
  } while (!done);

  sw_serial.println("Door closed!");

  door_open = false;
}

void open()
{
  sw_serial.println("Opening door...");
  motors.forward();
  
  bool done = false;
  do 
  {  
    bool left_pressed = (digitalRead(PIN_ENDSTOP_GOING_UP_LEFT) == LOW);
    bool right_pressed = (digitalRead(PIN_ENDSTOP_GOING_UP_RIGHT) == LOW);

    if (left_pressed)
    {
      motors.stopA();
    }
    if (right_pressed)
    {
      motors.stopB();
    }

    done = !motors.isMovingA() && !motors.isMovingB();
  
  } while (!done);
  sw_serial.println("Door open!");
  
  door_open = true;
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

void ISR_onManualUpButtonPressed()
{
  manual_button_up_pressed = true;
}

void ISR_onManualDownButtonPressed()
{
  manual_button_down_pressed = true;
}


// Pins definition

// Constants

// Library objects

// Global variables

// ISR_ButtonUp()
// ISR_ButtonDown()

// isPressed(pin)

// getBrightness()

// isAM()

// openDoor() / closeDoor()

// deepSleep()


