# TNM-IOT-FIRMWARE

**A production-grade IoT gateway firmware for the ESP8266, enabling cloud-connected smart home automation through a master-slave architecture.**

Built with C on ESP-IDF/FreeRTOS | ~2,000 lines of firmware code | Version 1.0.0

---

## Overview

TNM-IOT-FIRMWARE is a real-time embedded firmware that transforms an ESP8266 microcontroller into an IoT gateway hub. It bridges cloud services (via MQTT) with physical hardware peripherals (via I2C), enabling remote control and monitoring of relays, fans, buttons, and sensors through a custom master-slave communication protocol.

### Key Highlights

- **Real-Time Operating System** — Built on FreeRTOS with event-driven architecture and concurrent task management
- **Cloud-Connected** — Full MQTT client with JSON payload serialization for bidirectional device-to-cloud communication
- **Master-Slave Architecture** — ESP8266 master coordinates with a slave MCU over I2C to control hardware peripherals
- **Modular Design** — Clean separation of concerns across 7 independent firmware modules
- **Time-Synchronized** — NTP integration for accurate event scheduling and timer-based automation
- **Configurable** — Kconfig-based build configuration for device counts, network credentials, and hardware parameters

---

## Architecture

```
                    ┌──────────────────────────────┐
                    │        Cloud / Mobile App     │
                    └──────────────┬───────────────┘
                                   │ MQTT
                                   ▼
┌──────────────────────────────────────────────────────────────┐
│  ESP8266 Master (This Firmware)                              │
│                                                              │
│  ┌─────────┐  ┌──────┐  ┌─────┐  ┌─────┐  ┌──────────────┐ │
│  │  WiFi   │  │ MQTT │  │ NTP │  │ JSON│  │    Models    │ │
│  │ Module  │──│Client│──│Sync │──│ Gen │──│ (State Mgmt) │ │
│  └─────────┘  └──────┘  └─────┘  └─────┘  └──────────────┘ │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐    │
│  │                   I2C Master Controller               │    │
│  └──────────────────────────┬───────────────────────────┘    │
└─────────────────────────────┼────────────────────────────────┘
                              │ I2C Bus (GPIO 4/5)
                              ▼
┌──────────────────────────────────────────────────────────────┐
│  Slave MCU                                                   │
│  ┌────────┐  ┌────────┐  ┌──────────┐  ┌─────────────────┐  │
│  │ Relays │  │  Fans  │  │ Buttons  │  │    Sensors      │  │
│  │ (2-5)  │  │ (2-5)  │  │  (1-4)   │  │    (1-5)        │  │
│  └────────┘  └────────┘  └──────────┘  └─────────────────┘  │
└──────────────────────────────────────────────────────────────┘
```

---

## Technical Stack

| Layer | Technology |
|-------|-----------|
| **MCU** | ESP8266 — Xtensa LX106 @ 80/160 MHz, 160 KB RAM, 2 MB Flash |
| **RTOS** | FreeRTOS — Preemptive multitasking, event groups, task priorities |
| **Language** | C (C99) |
| **Framework** | ESP-IDF (ESP8266 RTOS SDK) |
| **Build System** | CMake + Xtensa cross-compiler toolchain |
| **Networking** | lwIP TCP/IP stack, WiFi 802.11 b/g/n (WPA2-PSK) |
| **Protocols** | MQTT 3.1.1, I2C (100 kHz), SNTP |
| **Libraries** | cJSON (serialization), mbedTLS (crypto), esp_mqtt |

---

## Project Structure

```
TNM-IOT-FIRMWARE/
├── firmware/
│   └── main/
│       ├── firmware.c              # Application entry point & boot sequence
│       ├── models/                 # Device state management layer
│       │   ├── master.c/h          #   Central state structure (MASTER)
│       │   ├── slave.c/h           #   Slave hardware abstraction (SLAVE)
│       │   ├── relay.c/h           #   Relay device model (on/off + timers)
│       │   ├── fan.c/h             #   Fan device model (speed 0-4 + timers)
│       │   ├── button.c/h          #   Button input model (long-press support)
│       │   ├── sensor.c/h          #   Sensor model (type + value tracking)
│       │   └── timer.c/h           #   Scheduler model (min/hr/day/month/week)
│       ├── wifi/                   # WiFi STA mode with retry logic
│       ├── mqtt/                   # MQTT client — pub/sub & command parsing
│       ├── I2c/                    # I2C master driver & custom ASCII protocol
│       ├── system/                 # System init, chip ID, client ID generation
│       ├── ntp/                    # SNTP time synchronization (pool.ntp.org)
│       ├── json/                   # JSON payload builders (endpoints + module)
│       └── Kconfig.projbuild       # Build-time configuration menu
├── dependencies/
│   └── ESP8266_RTOS_SDK/           # Espressif SDK
├── examples/                       # Reference implementations (GPIO, I2C, SNTP)
└── LICENSE
```

---

## Module Breakdown

### 1. State Management (`models/`)
Hierarchical data model representing the entire device state:

```
MASTER
├── chipid          (unique device ID from MAC address)
├── mqtt_clientid   (TNMESP12 + chipid)
├── SYSTEM
│   ├── CLOCK       (epoch time from NTP)
│   └── FLAG        (system state flags)
└── SLAVE
    ├── RELAY[]     (state, change tracking, 5 timers each)
    ├── FAN[]       (state, speed 0-4, 5 timers each)
    ├── BUTTON[]    (trigger detection, long-press, relay/fan mappings)
    └── SENSOR[]    (type, value, change tracking)
```

### 2. WiFi Module (`wifi/`)
- Station (STA) mode initialization with configurable SSID/password
- Event-driven state machine: `START → AUTH → CONNECTED` or `DISCONNECTED → RETRY`
- Automatic retry logic (configurable max attempts, default: 5)
- FreeRTOS event group synchronization for blocking until connected

### 3. MQTT Client (`mqtt/`)
- Connects to broker at `iot.thenextmove.in` with authentication
- Subscribes to command topics and parses incoming control messages
- Publishes device state as minified JSON payloads (QoS 0)
- Command protocol: `WRELAY=1100` sets Relay 1 ON, Relay 2 ON, Relay 3 OFF, Relay 4 OFF

### 4. I2C Master (`I2c/`)
- Custom ASCII command protocol over I2C bus (GPIO 4 SCL, GPIO 5 SDA)
- Slave address `0x68`, 20-byte response buffer, NULL-terminated messages
- Example exchange:
  ```
  Master → Slave:  "RSLAVENAME"      # Request device info
  Slave  → Master: "SLAVE2R1FMS4B"   # 2 Relays, 1 Fan, 1 Sensor, 4 Buttons

  Master → Slave:  "WRELAY=1100"     # Set relay states
  ```
- Constant-time string comparison to prevent timing-based attacks

### 5. NTP Sync (`ntp/`)
- SNTP client polling `pool.ntp.org` for UTC time
- Background FreeRTOS task (priority 9) refreshing epoch time every second
- Time validation with retry logic (up to 10 attempts on startup)

### 6. JSON Serialization (`json/`)
- Generates structured JSON payloads using cJSON library
- Three payload types:
  - **Relay Endpoint** — state, timers, online status
  - **Fan Endpoint** — state, speed value, timers
  - **Module Info** — chip ID, version, all endpoint paths, hardware info

Example module payload:
```json
{
  "chipid": "d9bc4d",
  "type": "MASTERWIFI12T",
  "version": "1.0.0",
  "clock": 1709335580,
  "timer_number": 5,
  "offline": false,
  "endpoints": ["d9bc4d/relay/1/", "d9bc4d/relay/2/", "d9bc4d/fan/1/", "d9bc4d/sensor/1/"],
  "hardware": { "name": "SLAVE2R1FMS4B", "version": "18" }
}
```

### 7. System Init (`system/`)
- Derives unique chip ID from WiFi MAC address (last 3 bytes → hex string)
- Generates MQTT client ID: `TNMESP12` + chip ID
- Bootstraps all device models with default configurations

---

## Boot Sequence

```
app_main()
  │
  ├─ initilize_master()         → Build state model, derive chip/client IDs
  ├─ nvs_flash_init()           → Initialize persistent storage (NVS)
  ├─ i2c_master_init()          → Configure I2C bus on GPIO 4/5
  ├─ wifi_init_sta()            → Connect to WiFi (blocks until success/failure)
  │   ├─ [On Connect]
  │   │   ├─ ntp_app_start()    → Spawn background time-sync task
  │   │   └─ mqtt_app_start()   → Connect to MQTT broker, subscribe to topics
  │   └─ [On Failure]
  │       └─ Set WIFI_FAIL_BIT
  ├─ I2C handshake              → Query slave device info ("RSLAVENAME")
  └─ MQTT publish               → Send relay/fan/module status as JSON
```

---

## Build & Flash

### Prerequisites
- ESP8266 RTOS SDK (included in `dependencies/`)
- Xtensa LX106 cross-compiler toolchain
- Python 3.x (for ESP-IDF tools)

### Configure
```bash
cd firmware
idf.py menuconfig
```
Configurable parameters via Kconfig:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `MODEL_NAME` | MASTERWIFI12T | Device model identifier |
| `ESP_WIFI_SSID` | — | WiFi network name |
| `ESP_WIFI_PASSWORD` | — | WiFi password |
| `ESP_MAXIMUM_RETRY` | 5 | Max WiFi reconnection attempts |
| `RELAY_MAX_NUMBER` | 5 | Maximum relay devices |
| `FAN_MAX_NUMBER` | 5 | Maximum fan devices |
| `TIMER_NUMBER` | 5 | Schedulable timers per device |

### Build & Flash
```bash
idf.py -p /dev/cu.usbserial-XXXX flash monitor
```

Press `Ctrl-]` to exit the serial monitor.

---

## Communication Protocols

### MQTT (Device ↔ Cloud)
| Direction | Format | Example |
|-----------|--------|---------|
| **Upstream** | JSON payload | `{"enpoint":"d9bc4d/relay/1/","state":1,"type":"tnm.endpoint.switch",...}` |
| **Downstream** | ASCII command | `WRELAY=1100` (Relay 1 ON, Relay 2 ON, Relay 3 OFF, Relay 4 OFF) |

### I2C (Master ↔ Slave)
| Command | Description | Response |
|---------|-------------|----------|
| `RSLAVENAME` | Query slave identity | `SLAVE2R1FMS4B` |
| `RRELAY` | Read relay states | Binary state string |
| `WRELAY=XXXX` | Write relay states | ACK |

---

## Skills Demonstrated

This project showcases proficiency in:

- **Embedded Systems Programming** — Bare-metal C on resource-constrained MCU (160 KB RAM)
- **RTOS Development** — FreeRTOS task management, event groups, synchronization primitives
- **IoT Protocol Implementation** — MQTT client, I2C master driver, SNTP time sync
- **Network Programming** — WiFi stack configuration, TCP/IP via lwIP, state machine design
- **System Architecture** — Modular firmware design with clean separation of concerns
- **Hardware-Software Integration** — GPIO configuration, I2C bus management, peripheral control
- **Data Serialization** — JSON payload generation with cJSON for cloud communication
- **Build Systems** — CMake cross-compilation, Kconfig build configuration
- **Memory Management** — Manual allocation/deallocation in constrained environment
- **Security Awareness** — Constant-time comparison, WPA2 encryption, authenticated MQTT

---

## License

See [LICENSE](LICENSE) for details.
