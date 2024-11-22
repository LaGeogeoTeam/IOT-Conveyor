#include <M5Unified.h>
#include "wifi/WifiManager.h"
#include "api/APIClient.h"
#include "json/JsonParser.h"
#include "rfid/RFIDManager.h"
#include "grbl/Module_GRBL_13.2.h"
#include <stdio.h>
#include "goplus2/GoPlus2.h"
#include <driver/rmt.h>
#include <math.h>
#undef min
#undef max
#include <thread>
#include <chrono>
#include <unordered_map>
using namespace std;

const char *ssid = "La Geogeo Team";
const char *password = "LaGeogeoTeam";
const char *baseURL = "http://146.59.232.167:8081/api/index.php/";
const char *token = "c3iwyi9dcQ2s";

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * You could use the DIP Switch for modify I2C address to 0x71
 */

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

TaskHandle_t Task1;
TaskHandle_t Task2;

// GRBL

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71

Module_GRBL _GRBL_A = Module_GRBL(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B = Module_GRBL(STEPMOTOR_I2C_ADDR_2);

// GOPLUS2

GoPlus2 goPlus;

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23
#define rrmt_item32_timemout_us 9500

int _hub1, hub1 = 0;
rmt_item32_t signals[1024];

size_t received = 0;

int flag = 0;
int num = 0;

int rfidArray[3][2] = {
        {1, 500},
        {2, 1000},
        {3, 1500}
};

APIClient apiClient(baseURL, token);

void doTask()
{
  if (num == 4)
  {
    num = 0;
  }
  else
  {
    num++;
  }
  Serial.println(num);
}

void Servo(int angle)
{
  Serial.print(angle);
  goPlus.Servo_write_plusewidth(SERVO_NUM0_PW, angle);
}

void stepMotor()
{
  _GRBL_A.setMotor(50000, 50000, 50000, 200);
  _GRBL_A.setMotor(0, 0, 0, 0);
}

void rfidReader(const int id)
{
  for(int cpt = 0; cpt < 3; cpt++){
    if(id == rfidArray[cpt][0]){
      Servo(rfidArray[cpt][1]);
    }
  }
  M5.update();
}

void setup()
{
  Serial.begin(115200);
  M5.begin();
  M5.Power.begin();

  // GRBL
  Wire.begin(21, 22);
  _GRBL_A.Init(&Wire);
  _GRBL_B.Init(&Wire);
  Serial.begin(115200);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(80, 10);
  M5.Lcd.println("Control motor");
  _GRBL_A.setMode("distance");

  // GOPLUS2
  goPlus.begin();

  goPlus.Servo_write_angle(SERVO_NUM0, 0);
  auto cfg = M5.config();
  cfg.external_spk = true;

  connectToWiFi(ssid, password);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setBrightness(100);
  M5.Lcd.setCursor(20, 20);
  initMFRC522(); // Init _MFRC522
  M5.Lcd.println("Please put the card\n\nUID:");
}

void loop()
{
  M5.update();
  stepMotor(); 
  reconnectWiFi(ssid, password);

  String uid = getCardUID();
  if (uid != "")
  { // Si une carte est détectée
    M5.Lcd.clear();
    M5.Lcd.println("Card Detected!");
    M5.Lcd.println("uid : " + uid);
    String response = apiClient.getRequest("products/ref/", uid);
    
    String warehouseId = getJsonValue(response, "fk_default_warehouse");
    M5.Lcd.clear();
    M5.Lcd.setCursor(20, 20);
    M5.Lcd.println("Warehouse Id: " + warehouseId);
    
    rfidReader(warehouseId.toInt());

    delay(200); // Pause pour éviter la lecture continue de la même carte
  }
}