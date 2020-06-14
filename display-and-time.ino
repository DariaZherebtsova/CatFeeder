#include <Time.h>
#include <TimeLib.h>
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);
#include <Servo.h> //используем библиотеку для работы с сервоприводом
Servo servo; //объявляем переменную servo типа Servo

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define s_hour 21
#define s_min 54
#define s_sec 0
#define s_day 4
#define s_month 4
#define s_year 2020


int now_min;
int now_hour;
boolean f_feeding;
char timeStr[10];

void setup() {
    delay(300);
    Serial.begin(9600);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
//    display.display();
//    delay(2000); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Hello, world!"));

    time.begin();
//    time.settime(s_sec,s_min,s_hour,s_day,s_month,s_year);
    servo.attach(10); //привязываем привод к порту 10
    f_feeding = false;
    servo.write(90); //закрыто
}
void loop(){
    if( millis()%1000==0 ){ // если прошла 1 секунда
      Serial.println(time.gettime("d-m-Y, H:i:s, D")); // выводим время
      now_min = atoi(time.gettime("i"));
      now_hour = atoi(time.gettime("H"));
//      Serial.println(now_hour);
//      Serial.println(now_min);

      display.clearDisplay();
      display.setCursor(10,10);             // Start at top-left corner
      display.println(time.gettime("H:i:s"));
      display.display(); 

      if ((now_hour == 3) && (now_min == 45) && !f_feeding) {
        servo.write(0); //открыто
        delay(2000);
        servo.write(90); //закрыто
        f_feeding = true;
      }
    
      delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
    }
}
