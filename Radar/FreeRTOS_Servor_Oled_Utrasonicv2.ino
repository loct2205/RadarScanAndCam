#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
//#include <HardwareSerial.h>


#define servo_pin 12
#define servofreq 50
#define resolution 12
#define servochannel 0

#define trigPin 32
#define echoPin 33

#define servo2_pin 13
#define servo2freq 50
//#define resolution 12
#define servo2channel 1

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define i2c_Address 0x3c  //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 21     //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//HardwareSerial Serial2(2);
QueueHandle_t myQueueHandle;
void TaskServo1(void *pvParameters);
void TaskServo2(void *pvParameters);
TaskHandle_t analog_read_task_handle;

void setup() {
  Serial.begin(115200);
  //Serial2.begin(115200, SERIAL_8N1, 16, 17); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  ledcSetup(servochannel, servofreq, resolution);
  ledcSetup(servo2channel, servo2freq, resolution);
  ledcAttachPin(servo_pin, servochannel);
  ledcAttachPin(servo2_pin, servo2channel);
  display.begin(i2c_Address, true);  // Address 0x3C default
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();
  xTaskCreatePinnedToCore(
    TaskServo1, "Task Servo1", 10000, NULL, 2, NULL, 0);


  xTaskCreatePinnedToCore(
    TaskServo2, "Task Servo2", 1048, NULL, 1, NULL, 1);


  myQueueHandle = xQueueCreate(16, sizeof(uint16_t));
}

void loop() {
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskServo1(void *pvParameters) {
  (void)pvParameters;
  int center_x = display.width() / 2; 
  int center_y = 0;
  int Line_radius = 48; 
  int Line_x;
  int Line_y;
  int label_radius = 55;
  int label_x;
  int label_y;
  int pixel_x1;
  int pixel_y1;
  int pixel_x2;
  int pixel_y2;
  int servo_min = 102;
  int servo_max = 509;

  int distance = 0;
  int distance_min = 3;
  int distance_max = 50;
  for (;;) {
    for (int i = 0; i <= 180; i++) {  
      Serial.println("Vong lap 1");
      int angle = i;
      int servoPulse = map(angle, 0, 180, servo_min, servo_max);
      ledcWrite(servochannel, servoPulse);
      distance = getDistance();

      if (distance >= distance_max) {
        //radarscan
        pixel_x1 = Line_radius * cos(radians(i - 4)) + center_x;
        pixel_y1 = Line_radius * sin(radians(i - 4)) + center_y;

        pixel_x2 = Line_radius * cos(radians(i + 4)) + center_x;
        pixel_y2 = Line_radius * sin(radians(i + 4)) + center_y;

        display.fillTriangle(center_x, center_y, pixel_x1, pixel_y1, pixel_x2, pixel_y2, 1);
        //drawRadarTable
        display.drawLine(0, 0, 125, 0, 1);
        for (int i = 12; i <= Line_radius; i = i + 12) {
          display.drawCircle(center_x, center_y, i, 1);
        }

        for (int i = 30; i <= 150; i = i + 30) {
          Line_x = cos(radians(i)) * (Line_radius + 5) + center_x;
          Line_y = sin(radians(i)) * (Line_radius + 5) + center_y;
          display.drawLine(center_x, center_y, Line_x, Line_y, 1);

          label_x = cos(radians(i)) * label_radius + center_x;
          label_y = sin(radians(i)) * label_radius + center_y;

          switch (i) {
            case 90: label_x = label_x - 6; break;
            case 120: label_x = label_x - 16; break;
            case 150: label_x = label_x - 16; break;
          }

          display.setTextSize(1);
          display.setTextColor(1);
          display.setCursor(label_x, label_y);
          display.print(i);
        }
        display.display();
        display.clearDisplay();  
      } else {
        //objectscan
        int circle_radius = 5;
        int object_radius = map(distance, distance_min, distance_max, 0, Line_radius);
        Line_x = object_radius * cos(radians(i)) + center_x;
        Line_y = object_radius * sin(radians(i)) + center_y;

        pixel_x1 = (object_radius - circle_radius) * cos(radians(i - 4)) + center_x;
        pixel_y1 = (object_radius - circle_radius) * sin(radians(i - 4)) + center_y;

        pixel_x2 = (object_radius - circle_radius) * cos(radians(i + 4)) + center_x;
        pixel_y2 = (object_radius - circle_radius) * sin(radians(i + 4)) + center_y;

        display.fillTriangle(center_x, center_y, pixel_x1, pixel_y1, pixel_x2, pixel_y2, 1);
        display.drawCircle(Line_x, Line_y, circle_radius, 1);
        display.drawLine(0, 0, 125, 0, 1);
        for (int i = 12; i <= Line_radius; i = i + 12) {
          display.drawCircle(center_x, center_y, i, 1);
        }

        for (int i = 30; i <= 150; i = i + 30) {
          Line_x = cos(radians(i)) * (Line_radius + 5) + center_x;
          Line_y = sin(radians(i)) * (Line_radius + 5) + center_y;
          display.drawLine(center_x, center_y, Line_x, Line_y, 1);

          label_x = cos(radians(i)) * label_radius + center_x;
          label_y = sin(radians(i)) * label_radius + center_y;

          switch (i) {
            case 90: label_x = label_x - 6; break;
            case 120: label_x = label_x - 16; break;
            case 150: label_x = label_x - 16; break;
          }

          display.setTextSize(1);
          display.setTextColor(1);
          display.setCursor(label_x, label_y);
          display.print(i);
        }
        display.display();
        display.clearDisplay();
        xQueueSend(myQueueHandle, &i, 0);
        //Serial2.println("1");
      }
      vTaskDelay(20);
    }

    for (int i = 180; i >= 0; i--) {  
      Serial.println("Vong lap 2");
      int angle = i;
      int servoPulse = map(angle, 0, 180, servo_min, servo_max);
      ledcWrite(servochannel, servoPulse);

      distance = getDistance();
      if (distance >= distance_max) {
        pixel_x1 = Line_radius * cos(radians(i - 4)) + center_x;
        pixel_y1 = Line_radius * sin(radians(i - 4)) + center_y;

        pixel_x2 = Line_radius * cos(radians(i + 4)) + center_x;
        pixel_y2 = Line_radius * sin(radians(i + 4)) + center_y;

        display.fillTriangle(center_x, center_y, pixel_x1, pixel_y1, pixel_x2, pixel_y2, 1);
        display.drawLine(0, 0, 125, 0, 1);
        //display.display();
        for (int i = 12; i <= Line_radius; i = i + 12) {
          display.drawCircle(center_x, center_y, i, 1);
        }

        for (int i = 30; i <= 150; i = i + 30) {
          Line_x = cos(radians(i)) * (Line_radius + 5) + center_x;
          Line_y = sin(radians(i)) * (Line_radius + 5) + center_y;
          display.drawLine(center_x, center_y, Line_x, Line_y, 1);

          label_x = cos(radians(i)) * label_radius + center_x;
          label_y = sin(radians(i)) * label_radius + center_y;

          switch (i) {
            case 90: label_x = label_x - 6; break;
            case 120: label_x = label_x - 16; break;
            case 150: label_x = label_x - 16; break;
          }

          display.setTextSize(1);
          display.setTextColor(1);
          display.setCursor(label_x, label_y);
          display.print(i);
        }
        display.display();
        display.clearDisplay();  
      } else {
        //objectscan
        int circle_radius = 5;
        int object_radius = map(distance, distance_min, distance_max, 0, Line_radius);
        Line_x = object_radius * cos(radians(i)) + center_x;
        Line_y = object_radius * sin(radians(i)) + center_y;

        pixel_x1 = (object_radius - circle_radius) * cos(radians(i - 4)) + center_x;
        pixel_y1 = (object_radius - circle_radius) * sin(radians(i - 4)) + center_y;

        pixel_x2 = (object_radius - circle_radius) * cos(radians(i + 4)) + center_x;
        pixel_y2 = (object_radius - circle_radius) * sin(radians(i + 4)) + center_y;

        display.fillTriangle(center_x, center_y, pixel_x1, pixel_y1, pixel_x2, pixel_y2, 1);
        display.drawCircle(Line_x, Line_y, circle_radius, 1);
        display.drawLine(0, 0, 125, 0, 1);
        for (int i = 12; i <= Line_radius; i = i + 12) {
          display.drawCircle(center_x, center_y, i, 1);
        }

        for (int i = 30; i <= 150; i = i + 30) {
          Line_x = cos(radians(i)) * (Line_radius + 5) + center_x;
          Line_y = sin(radians(i)) * (Line_radius + 5) + center_y;
          display.drawLine(center_x, center_y, Line_x, Line_y, 1);

          label_x = cos(radians(i)) * label_radius + center_x;
          label_y = sin(radians(i)) * label_radius + center_y;

          switch (i) {
            case 90: label_x = label_x - 6; break;
            case 120: label_x = label_x - 16; break;
            case 150: label_x = label_x - 16; break;
          }

          display.setTextSize(1);
          display.setTextColor(1);
          display.setCursor(label_x, label_y);
          display.print(i);
        }
        display.display();
        display.clearDisplay();
        xQueueSend(myQueueHandle, &i, 0);
        //Serial2.println("1");
      }
      vTaskDelay(20);
    }
  }
}

void TaskServo2(void *pvParameters) {
  (void)pvParameters;
  int servo2_min = 102;
  int servo2_max = 509;
  uint16_t value;

  for (;;) {
    if (xQueueReceive(myQueueHandle, &value, portMAX_DELAY)) {
      int angle = value;
      int servoPulse = map(angle, 0, 180, servo2_min, servo2_max);
      ledcWrite(servo2channel, servoPulse);
    }
    vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}

int getDistance() { 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  int duration = pulseIn(echoPin, HIGH);
  int distanceCM = duration * 0.0343 / 2;
  return distanceCM;
}
