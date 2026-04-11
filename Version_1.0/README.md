# 🔵 Budget SLAM (Version 1)

A low-cost 2D scanning system built using **ESP32 + dual VL53L0X sensors + servo**, capable of generating a 360° distance map and streaming it in real-time over WebSockets.

---

## 🚀 Features

- 🔄 180° servo sweep → full 360° scan using dual sensors  
- 📡 Real-time data streaming via WebSocket  
- 📊 Live point + full scan transmission  
- ⚡ Continuous ranging mode for fast updates  
- 🎯 Configurable resolution & speed  
- 💸 Built with low-cost components  

---

## 🧠 Working Principle

- A servo sweeps from **0° → 180°**
- Two **VL53L0X sensors**:
  - Front sensor → direct angle
  - Back sensor → `(angle + 180) % 360`
- This effectively produces a **full 360° scan**
- Data is:
  - Sent live (`point updates`)
  - Sent as full scan after each sweep

---

## 🔌 Hardware Used

- ESP32
- 2 × VL53L0X ToF Sensors
- Servo Motor (SG90 / MG90)
- Jumper wires

---

## ⚙️ Pin Configuration

| Component | Pin |
|----------|-----|
| Servo    | GPIO 13 |
| Sensor 0 XSHUT | GPIO 26 |
| Sensor 1 XSHUT | GPIO 27 |
| I2C SDA | GPIO 21 |
| I2C SCL | GPIO 22 |

---

## 🧾 Data Format

### 📍 Point Data (live)
```json
{"t":"p","a":90,"d":450}
