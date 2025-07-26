# Temperature Monitor

A small GTKmm‑based utility that displays your system’s CPU core/package temperatures, NVIDIA GPU temperature, and battery status in a desktop window. Includes built‑in alerts if CPU exceeds 100 °C or GPU exceeds 75 °C.

---

## Features

- **CPU Temperatures**  
  Reads per‑core and package temperatures via `lm-sensors`.  
- **GPU Temperature**  
  Queries NVIDIA GPU temperature via `nvidia-smi`.  
- **Battery Status**  
  Shows charge level and state via `acpi`.  
- **Alerts**  
  Pops up a GTK window if CPU > 100 °C or GPU > 75 °C.

---

## Requirements

- **Linux** with:  
  - `lm-sensors` (`sensors`) configured  
  - NVIDIA drivers + `nvidia-smi` (if you have an NVIDIA GPU)  
  - `acpi` installed  
- **GTKmm 3** development libraries  
- **C++17** (or C++14/C++11) compiler  

---

## Installation

1. **Install dependencies**  
   ```bash
   sudo apt update
   sudo apt install build-essential libgtkmm-3.0-dev lm-sensors acpi
   sudo sensors-detect       # follow prompts to enable /etc/modules
   ```
2. (Optional) **Enable NVIDIA monitoring**  
   ```bash
   sudo apt install nvidia-driver-<version>
   ```

---

## Building

Clone or copy the source into a directory, then:

```bash
g++ -std=c++17 -Wall -Wextra -o temperature_monitor tempmonitor.c++ \
    $(pkg-config --cflags --libs gtkmm-3.0)
```

---

## Usage

```bash
./temperature_monitor
```

A window will appear showing:

- **CPU Temperatures** (per‑core + package)  
- **GPU Temperature** (°C)  
- **Battery Status**  

If thresholds are exceeded, an alert dialog will pop up.


---

## License

This project is licensed under the **Apache 2.0 License**. See [LICENSE](LICENSE) for details.

## Contact Me:

Ram Sundar Radhakrishnan
ramsundar289@gmail.com
