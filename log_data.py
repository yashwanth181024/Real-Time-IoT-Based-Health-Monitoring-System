"""
Real-Time IoT-Based Health Monitoring System
---------------------------------------------
Reads CSV-formatted BPM/temperature frames from the Arduino over serial,
timestamps each reading, and appends it to health_data.csv.

Frame format expected from Arduino:
    Time_ms,BPM,Temperature_C

Usage:
    python log_data.py

Press Ctrl+C to stop logging safely.
"""

import serial
import csv
from datetime import datetime

PORT = "COM5"       # Change this to your Arduino's serial port
                     # (e.g. "COM5" on Windows, "/dev/ttyUSB0" or "/dev/ttyACM0" on Linux/Mac)
BAUD_RATE = 9600
FILENAME = "health_data.csv"


def main():
    arduino = serial.Serial(PORT, BAUD_RATE, timeout=1)

    print("Connected to Arduino")
    print("Saving data to:", FILENAME)
    print("Press Ctrl+C to stop.")

    with open(FILENAME, "a", newline="") as file:
        writer = csv.writer(file)

        # Write header if file is empty
        if file.tell() == 0:
            writer.writerow(["Date", "Time", "Arduino_Time_ms", "BPM", "Temperature_C"])

        try:
            while True:
                line = arduino.readline().decode("utf-8", errors="ignore").strip()

                if not line:
                    continue

                # Ignore Arduino startup / non-numeric messages
                if not line[0].isdigit():
                    continue

                data = line.split(",")
                if len(data) != 3:
                    continue

                time_ms, bpm, temperature = data
                now = datetime.now()

                writer.writerow([
                    now.strftime("%Y-%m-%d"),
                    now.strftime("%H:%M:%S"),
                    time_ms, bpm, temperature
                ])
                file.flush()

                print(now.strftime("%H:%M:%S"),
                      "| BPM:", bpm,
                      "| Temperature:", temperature, "C")

        except KeyboardInterrupt:
            print("\nRecording stopped.")

        finally:
            arduino.close()


if __name__ == "__main__":
    main()
