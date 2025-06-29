#include <WiFi.h>
#include <WebServer.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// Wi-Fi Configuration
#define SSID "Wifi_SSID"
#define PASSWORD "Wifi_Password"

// LED Matrix Configuration
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN 18
#define DATA_PIN 23
#define CS_PIN 5

// Motor Pins
#define ENA 22
#define IN1 21
#define IN2 19
#define ENB 17
#define IN3 16
#define IN4 4

// Ultrasonic Sensor
#define TRIG_PIN 27
#define ECHO_PIN 26

// Buzzer and LEDs
#define BUZZER_PIN 14
#define LED1_PIN 12
#define LED2_PIN 13

MD_Parola mx = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
WebServer server(80);

// Message as C-style string
char message[50] = "Created By Jaival Soni";
int brightness = 2;
bool scrolling = true;

void setup() {
  Serial.begin(115200);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Pins
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT); pinMode(LED1_PIN, OUTPUT); pinMode(LED2_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);

  // Matrix setup
  mx.begin();
  mx.setIntensity(brightness);
  mx.displayClear();
  startScroll(message);

  // Web routes
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/led", handleLED);
  server.on("/buzz", handleBuzz);
  server.on("/msg", handleMessage);
  server.on("/bright", handleBrightness);
  server.begin();
}

void loop() {
  server.handleClient();
  blinkLEDs();

  if (scrolling && mx.displayAnimate()) {
    mx.displayReset();
  }

  float dist = getDistance();
  if (dist > 0 && dist < 15) {
    stopMotors();
    scrolling = false;
    mx.displayClear();
    mx.displayText("Obstacle!", PA_CENTER, 100, 0, PA_PRINT, PA_NO_EFFECT);
    while (!mx.displayAnimate()) server.handleClient();
    tone(BUZZER_PIN, 1000);
    delay(300);
    noTone(BUZZER_PIN);
    startScroll(message);
  }
}

void startScroll(const char* msg) {
  mx.displayClear();
  mx.displayText(msg, PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  mx.displayReset();
  scrolling = true;
}

void handleRoot() {
  String html = "<html><body><h1>ESP32 Car Control</h1>"
                "<button onclick=\"location.href='/move?dir=fwd'\">Forward</button>"
                "<button onclick=\"location.href='/move?dir=back'\">Backward</button><br>"
                "<button onclick=\"location.href='/move?dir=left'\">Left</button>"
                "<button onclick=\"location.href='/move?dir=right'\">Right</button><br>"
                "<button onclick=\"location.href='/move?dir=stop'\">Stop</button><br><hr>"
                "<form action='/msg'><input name='text'><input type='submit' value='Set Text'></form>"
                "<form action='/bright'><input name='val' type='number'><input type='submit' value='Set Brightness'></form>"
                "<button onclick=\"location.href='/buzz'\">Buzz</button>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void handleMove() {
  String dir = server.arg("dir");
  if (dir == "fwd") moveForward();
  else if (dir == "back") moveBackward();
  else if (dir == "left") turnLeft();
  else if (dir == "right") turnRight();
  else stopMotors();
  server.send(200, "text/plain", "OK");
}

void handleLED() {
  digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
  digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
  server.send(200, "text/plain", "LED Toggled");
}

void handleBuzz() {
  tone(BUZZER_PIN, 1000);
  delay(300);
  noTone(BUZZER_PIN);
  server.send(200, "text/plain", "Buzz");
}

void handleMessage() {
  String newMsg = server.arg("text");
  newMsg.toCharArray(message, sizeof(message));
  startScroll(message);
  server.send(200, "text/plain", "Message Updated");
}

void handleBrightness() {
  brightness = server.arg("val").toInt();
  mx.setIntensity(brightness);
  server.send(200, "text/plain", "Brightness Updated");
}

void blinkLEDs() {
  static unsigned long last = 0;
  if (millis() - last > 500) {
    digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
    digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
    last = millis();
  }
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH, 30000);
  float distance = duration * 0.034 / 2;
  if (distance == 0 || distance > 400) return -1;
  return distance;
}

// Motor Control Functions
void moveForward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void moveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void turnRight() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH); digitalWrite(ENB, HIGH);
}

void stopMotors() {
  digitalWrite(ENA, LOW); digitalWrite(ENB, LOW);
}
