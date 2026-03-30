#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <ESP32Servo.h>

const char* SSID     = "SSID";
const char* PASSWORD = "PASSWORD";

#define SERVO_PIN   13
#define XSHUT_S0    26
#define XSHUT_S1    27

#define ADDR_S0     0x30
#define ADDR_S1     0x31

#define STEP_DEG    2      // degrees per step (1=max detail, 2=faster)
#define STEP_DELAY  15     // ms per step
#define MAX_DIST    2000   // mm - discard beyond this

VL53L0X s0, s1;
Servo   scanServo;
WebSocketsServer ws(81);

uint16_t scanData[360];
bool     clientConnected = false;

int currentAngle = 0;
int sweepDir     = 1;    

void initSensors() {
  pinMode(XSHUT_S0, OUTPUT); digitalWrite(XSHUT_S0, LOW);
  pinMode(XSHUT_S1, OUTPUT); digitalWrite(XSHUT_S1, LOW);
  delay(20);

  digitalWrite(XSHUT_S0, HIGH); delay(10);
  if (!s0.init()) { Serial.println("S0 init FAILED"); while(1); }
  s0.setAddress(ADDR_S0);
  s0.setTimeout(100);
  s0.startContinuous(20);
  Serial.println("S0 ready at 0x30");

  digitalWrite(XSHUT_S1, HIGH); delay(10);
  if (!s1.init()) { Serial.println("S1 init FAILED"); while(1); }
  s1.setAddress(ADDR_S1);
  s1.setTimeout(100);
  s1.startContinuous(20);
  Serial.println("S1 ready at 0x31");
}

void onWsEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) {
    clientConnected = true;
    Serial.printf("Client #%u connected\n", num);
  } else if (type == WStype_DISCONNECTED) {
    clientConnected = false;
    Serial.printf("Client #%u disconnected\n", num);
  }
}

void sendPoint(int angle, uint16_t dist) {
  if (!clientConnected) return;
  char buf[40];
  snprintf(buf, sizeof(buf), "{\"t\":\"p\",\"a\":%d,\"d\":%d}", angle, dist);
  ws.broadcastTXT(buf);
}

void sendScan() {
  if (!clientConnected) return;
  String json = "{\"t\":\"s\",\"pts\":[";
  bool first = true;
  for (int i = 0; i < 360; i++) {
    if (scanData[i] == 0) continue;
    if (!first) json += ",";
    json += "["; json += i; json += ","; json += scanData[i]; json += "]";
    first = false;
  }
  json += "]}";
  ws.broadcastTXT(json);
}

void sweepStep() {
  scanServo.write(currentAngle);
  delay(STEP_DELAY);

  uint16_t d0 = s0.readRangeContinuousMillimeters();
  if (!s0.timeoutOccurred() && d0 > 30 && d0 < MAX_DIST) {
    scanData[currentAngle] = d0;
    sendPoint(currentAngle, d0);
  }

  int b1 = (currentAngle + 180) % 360;
  uint16_t d1 = s1.readRangeContinuousMillimeters();
  if (!s1.timeoutOccurred() && d1 > 30 && d1 < MAX_DIST) {
    scanData[b1] = d1;
    sendPoint(b1, d1);
  }

  currentAngle += sweepDir * STEP_DEG;

  if (currentAngle > 180) {
    currentAngle = 180;
    sweepDir     = -1;
    sendScan();
  } else if (currentAngle < 0) {
    currentAngle = 0;
    sweepDir     = 1;
    sendScan();
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  Wire.setClock(400000);

  memset(scanData, 0, sizeof(scanData));

  scanServo.attach(SERVO_PIN, 500, 2400);
  scanServo.write(0);
  delay(500);

  initSensors();

  Serial.printf("Connecting to %s", SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println();
  Serial.print("IP: "); Serial.println(WiFi.localIP());

  ws.begin();
  ws.onEvent(onWsEvent);
  Serial.println("WebSocket on port 81 - open IP in browser");
}

void loop() {
  ws.loop();      
  sweepStep();    
}
