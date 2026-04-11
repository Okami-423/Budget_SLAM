# 🛰️ Budget SLAM Scanner

A **low-cost DIY SLAM-style scanning system** built using ESP32, dual ToF sensors, and a custom WebSocket-based visualization UI.

> Designed as a fast prototype to explore real-time spatial mapping using minimal hardware.

---

## 🎥 Project Overview

- Real-time 2D environment scanning  
- Browser-based radar-style visualization  
- No external software required  
- Fully wireless (WiFi-based)  

---

## 🌐 Web Interface

The UI provides:

- 📡 Live scan visualization  
- 📊 Stats (points, min/max distance)  
- 🎛️ Controls (scale, grid, sweep, etc.)  
- 🧾 Logging system  

---

## 🚀 How It Works

1. ESP32 scans environment using servo + sensors  
2. Data is streamed via WebSocket  
3. Browser renders a **radar-style map in real-time**  

---

## ⚡ Quick Start

### 1. Upload Firmware
- Go to `version1/`
- Flash ESP32 code  

### 2. Open UI
- Open `index.html` in browser  

### 3. Connect
- Enter ESP32 IP  
- Click **Connect**
  
---

## 🎯 Why This Project?

- Explore **low-cost SLAM concepts**  
- Learn **sensor fusion basics**  
- Build a **LiDAR-like system without LiDAR**  

---

## 🔥 Future Roadmap

- 🧠 SLAM algorithms (GMapping / Cartographer)  
- 🤖 Mobile robot integration  
- 🗺️ Persistent mapping  
- 📍 Object detection  

---

## 💡 Inspiration

This project mimics basic functionality of:
- LiDAR scanners  
- Radar visualization systems  
- Autonomous robot perception  

---

## ⭐ Contribute

Feel free to:
- Improve UI  
- Optimize scanning  
- Add mapping algorithms  
