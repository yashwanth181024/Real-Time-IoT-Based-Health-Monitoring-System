# Real-Time IoT-Based Health Monitoring System

A low-cost, real-time health monitoring system built with an **Arduino UNO**, a **pulse sensor**, and a **DHT11 temperature sensor**. Vital signs are displayed live on a 16x2 LCD and simultaneously streamed over USB serial to a PC, where a Python script timestamps and logs every reading to a CSV file for analysis in Excel.

> Mini Project — Master of Computer Applications, Dr. Ambedkar Institute of Technology, Bengaluru (VTU, Belagavi)
> Author: Yashwanth G S (1DA25MC056) · Guide: Dr. Indumathi S K

---

## Overview

Conventional vital-sign checks (heart rate, temperature) are done manually and intermittently, producing sparse, hard-to-analyze data. This project builds a simple, reproducible pipeline that:

- Continuously senses **heart rate (BPM)** via a PPG-based pulse sensor
- Continuously senses **body/ambient temperature** via a DHT11 sensor
- Displays both values live on a **16x2 LCD**
- Streams readings over **USB serial (9600 baud)** as CSV frames
- Logs every reading with a **timestamp** to `health_data.csv` using a Python script

---

## Features

- ✅ Real-time BPM and temperature acquisition
- ✅ On-device LCD display (no phone/network required)
- ✅ Simple, human-readable CSV serial protocol
- ✅ Automatic, timestamped CSV logging — no data lost on script interruption
- ✅ Fully open, low-cost (~₹1,130–₹1,580 in hardware)
- ✅ Graceful handling of malformed frames and sensor read failures

---

## System Architecture

```
[Pulse Sensor] --A0--\
                       > [Arduino UNO] --USB Serial--> [Python Logger] --> health_data.csv
[DHT11 Sensor] --D10-/         |
                                v
                          [16x2 LCD]
```

The Arduino handles sensing, BPM computation, and display; all timestamping, validation, and persistent storage logic lives in the Python script on the PC.

### Circuit Diagram

![Circuit Diagram](docs/circuit_diagram.png)

---

## Hardware Components

| Component | Specification | Qty |
|---|---|---|
| Arduino UNO | ATmega328P, 5V logic | 1 |
| Pulse Sensor | PPG-based analog heart-rate module | 1 |
| DHT11 Sensor | Digital temperature sensor, single-wire | 1 |
| 16x2 LCD | HD44780-compatible, 4-bit mode | 1 |
| Potentiometer | 10KΩ (LCD contrast) | 1 |
| Resistor | 220Ω (LCD backlight) | 1 |
| Breadboard + Jumper Wires | — | 1 set |
| USB Cable | USB-A to USB-B | 1 |

## Pin Connections

| Component | Pin | Connected To |
|---|---|---|
| Pulse Sensor | Red (VCC) | 5V |
| Pulse Sensor | Black (GND) | GND |
| Pulse Sensor | Yellow/Purple (Signal) | A0 |
| DHT11 | VCC | 5V |
| DHT11 | DATA | D10 |
| DHT11 | GND | GND |
| 16x2 LCD | VSS | GND |
| 16x2 LCD | VDD | 5V |
| 16x2 LCD | VO (Contrast) | 10K Pot (wiper) |
| 16x2 LCD | RS, E | Digital pins |
| 16x2 LCD | RW | GND |
| 16x2 LCD | D4–D7 | Digital pins |
| 16x2 LCD | A (LED+) | 5V via 220Ω resistor |
| 16x2 LCD | K (LED−) | GND |

---

## Software Requirements

| Software / Library | Purpose |
|---|---|
| Arduino IDE | Compile and upload firmware |
| `PulseSensorPlayground` library | Read pulse sensor, compute BPM |
| `DHT` sensor library | Interface with DHT11 |
| Python 3 + `pyserial` | Read serial data on PC |
| `csv`, `datetime` (Python stdlib) | Timestamp and structure logged data |
| Microsoft Excel (or any spreadsheet tool) | View/analyze `health_data.csv` |

Install the Python dependency:

```bash
pip install pyserial
```

---

## Data Format

Each serial frame is a single CSV line:

```
Time_ms,BPM,Temperature_C
```

A header line is printed once on startup. Frames that aren't well-formed (not exactly 3 comma-separated numeric fields) are discarded by the logger rather than corrupting the log.

---

## Getting Started

### 1. Wire the hardware
Assemble the pulse sensor, DHT11, and LCD on a breadboard per the [pin connections](#pin-connections) above and the [circuit diagram](#circuit-diagram).

### 2. Flash the Arduino firmware
1. Open [`firmware/health_monitor.ino`](firmware/health_monitor.ino) in the Arduino IDE
2. Install the `PulseSensorPlayground` and `DHT sensor library` via Library Manager
3. Select **Board: Arduino UNO** and the correct COM port
4. Upload the sketch

### 3. Run the Python logger
1. Update the `PORT` variable in [`logger/log_data.py`](logger/log_data.py) to match your Arduino's serial port (e.g. `COM5`, `/dev/ttyUSB0`)
2. Run:
   ```bash
   python logger/log_data.py
   ```
3. Readings are printed to the console and appended to `health_data.csv`
4. Press `Ctrl+C` to stop logging safely

### 4. Analyze the data
Open `health_data.csv` in Excel or any spreadsheet tool to inspect or graph the logged BPM and temperature trends. A [sample output](data/health_data.csv) is included in this repo.

---

## Results

- LCD correctly displayed live BPM and temperature values
- Data frames transmitted reliably at the expected ~2-second interval
- Python logger created a complete, timestamped CSV record with no data loss
- Temperature readings were stable (~25.9–26.0°C indoors); BPM showed expected beat-to-beat variability typical of unfiltered analog pulse sensors

| Date | Time | BPM | Temperature (°C) |
|---|---|---|---|
| 09-08-2026 | 00:39:21 | 61 | 26 |
| 09-08-2026 | 00:39:23 | 59 | 25.9 |
| 09-08-2026 | 00:41:14 | 95 | 26 |
| 09-08-2026 | 00:41:24 | 42 | 26 |

See [`data/health_data.csv`](data/health_data.csv) for a fuller sample log.

---

## Applications

- Personal health & fitness tracking
- Elderly/home care monitoring
- Academic teaching platform for embedded systems / IoT
- Foundational building block for telemedicine
- Sports/athletic performance monitoring
- Auxiliary monitoring in low-acuity settings (hostels, health centres)

## Advantages

- Low cost (< ₹2,000 in hardware)
- Standalone, local operation — no Wi-Fi/cloud dependency
- Automatic, timestamped logging
- Simple, transparent plain-text CSV format
- Open and easily extensible codebase

## Limitations

- Wired USB tether limits mobility
- DHT11 accuracy (±2°C) and pulse sensor sensitivity to motion/placement
- Single-user, single-device only
- No automated threshold-based alerting
- No signal smoothing/filtering on raw BPM

---

## Future Scope

- [ ] Wireless connectivity via ESP8266/ESP32
- [ ] Signal smoothing / moving-average filtering for BPM
- [ ] Threshold-based alerts (visual/audible/push notification)
- [ ] Extended sensor suite (SpO2, ECG, DS18B20)
- [ ] Live dashboard/visualization of logged data
- [ ] Cloud sync and multi-user support (e.g. Firebase)
- [ ] Companion mobile app (BLE/Wi-Fi)
- [ ] Custom PCB and enclosure for a wearable form factor

---

## Repository Structure

```
.
├── firmware/
│   └── health_monitor.ino     # Arduino sketch
├── logger/
│   └── log_data.py            # Python serial logger
├── data/
│   └── health_data.csv        # Sample logged output
├── docs/
│   └── circuit_diagram.png    # Wiring / circuit diagram
├── LICENSE
└── README.md
```

---

## References

Key references include the PulseSensor.com getting-started guide, the Aosong DHT11 datasheet, Arduino UNO documentation, the pySerial docs, and the HD44780 LCD controller datasheet. See the full project report for the complete reference list.

## Author

**Yashwanth G S**
Master of Computer Applications, Dr. Ambedkar Institute of Technology, Bengaluru
GitHub: [@Yashwanth181024](https://github.com/Yashwanth181024)

## License

This project is licensed under the [MIT License](LICENSE).
