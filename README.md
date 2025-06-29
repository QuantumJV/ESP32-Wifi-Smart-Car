# 🚗 ESP32 Smart Wi-Fi Controlled Car

A feature-rich IoT-based RC car using the ESP32 microcontroller. This project allows you to control the car via a local Wi-Fi web interface and includes a variety of components like an LED matrix display, obstacle detection, buzzer alerts, and blinking LEDs.

---

## 📌 Features

- ✅ Control movement: Forward, Backward, Left, Right, Stop
- ✅ Wi-Fi Web Server interface to control everything
- ✅ Obstacle detection using Ultrasonic Sensor
- ✅ LED Matrix Display (8x32) with:
  - Custom scrolling text
  - Brightness control
- ✅ Passive Buzzer for alerts
- ✅ Blinking LEDs as indicators
- ✅ Smooth user interaction via browser on any phone/laptop

---

## 🧠 How It Works

- The **ESP32** creates a web server after connecting to Wi-Fi.
- You can connect from any browser on the same network to control the car.
- A **motor driver (L293D)** controls the car’s direction.
- An **ultrasonic sensor (HC-SR04)** detects obstacles and stops the car if something is in front.
- An **LED matrix (8x32)** displays scrolling text, which can be customized via the web interface.
- A **3-pin passive buzzer** plays a tone on command or during obstacle alerts.

---

## 🔌 Wiring Diagram

| Component       | ESP32 Pin   | Notes                             |
|----------------|-------------|-----------------------------------|
| Motor ENA      | GPIO 22     | Motor A Enable                    |
| Motor IN1      | GPIO 21     | Motor A Input 1                   |
| Motor IN2      | GPIO 19     | Motor A Input 2                   |
| Motor ENB      | GPIO 17     | Motor B Enable                    |
| Motor IN3      | GPIO 16     | Motor B Input 1                   |
| Motor IN4      | GPIO 4      | Motor B Input 2                   |
| LED Matrix CLK | GPIO 18     | Use with SPI                      |
| LED Matrix DATA| GPIO 23     |                                   |
| LED Matrix CS  | GPIO 5      |                                   |
| Ultrasonic TRIG| GPIO 27     |                                   |
| Ultrasonic ECHO| GPIO 26     | Use voltage divider (5V → 3.3V)   |
| Buzzer (SIG)   | GPIO 14     | 3-pin passive buzzer (tone)       |
| LED 1          | GPIO 12     |                                   |
| LED 2          | GPIO 13     |                                   |

### VCC and GND Wiring

| Component              | VCC Connection       | GND Connection        | Notes                                      |
|------------------------|----------------------|------------------------|--------------------------------------------|
| ESP32 Board            | 3.3V Regulator Output| Common Ground          | ESP32 runs on 3.3V                         |
| LED Matrix Display     | 5V Regulator Output  | Common Ground          | MAX7219 module requires 5V                 |
| Ultrasonic Sensor (HC-SR04) | 5V Regulator Output  | Common Ground      | Use voltage divider on ECHO to 3.3V logic  |
| Buzzer (Passive 3-pin) | 5V Regulator Output  | Common Ground          | VCC and GND used; signal to GPIO14         |
| L293D Motor Driver     | External Motor Power | Common Ground          | Separate 6–9V motor supply recommended      |
| LEDs (Indicators)      | Via GPIO Pull-Up     | Common Ground          | Driven directly from ESP32 GPIO            |

> ⚠️ **Important:** Connect **all GNDs together** (ESP32, regulators, sensors, motor driver) to maintain a common ground reference.  
> ✅ Use **AMS1117-3.3V** to power ESP32 and **AMS1117-5V** (or other 5V step-down) for display/sensors.

**Note:**
- Supply ESP32 with **3.3V regulated** from AMS1117 or USB.
- Use **external 6V–9V battery** to power motors.
- Use a **voltage divider** for ECHO pin of ultrasonic sensor to avoid damaging ESP32.

---

## 🧰 Components Used

- ESP32 Dev Board
- L293D Motor Driver Module
- 8x32 LED Matrix Display (MAX7219-based)
- 3-pin Passive Buzzer
- 2x DC Motors
- HC-SR04 Ultrasonic Sensor
- 2x LEDs (12, 13)
- Jumper Wires
- Breadboard / Chassis
- AMS1117 3.3V Regulator Module (for power)
- Power Supply: 9V Battery or 10.5V Adapter

---

## 🌐 Web Interface

Access the ESP32's IP address from your phone browser. Interface includes:
- Directional control buttons (Forward, Backward, Left, Right, Stop)
- Input box to set scrolling text on LED matrix
- Brightness control input for LED display
- Buzz button to trigger sound
- Auto obstacle detection and warning display

---

## 💻 Libraries Required

Install these libraries from Arduino Library Manager:
- `MD_Parola`
- `MD_MAX72XX`
- `WiFi`
- `WebServer`
- `SPI`

---


---

## 🧑‍💻 Author

**Jaival Soni**  
B.Tech Computer Science  Engineering

---

## 📢 License

Open-source for educational use. Attribution appreciated 😊

[MIT License](LICENSE)

Let me know if you'd like to:

- Add setup GIFs or extra screenshots  
- Include platform badges (ESP8266, Arduino IDE, MIT License, etc.)  
- Convert this for GitHub Pages or documentation site integration
