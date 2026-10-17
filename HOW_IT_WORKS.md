# Technical Reference: Signal Encodings & Architecture

This document details the low-level signal generation techniques and station protocol specifications used by `clocksync`. It is designed to provide sufficient detail for a skilled engineer to reimplement the logic on another platform.

Fork-specific DCF77 reliability, time safeguards, controls, Wi-Fi configuration and web interface improvements are credited to **axelaberg91**. See the README credits for the upstream authors and reference projects.

## Architecture

### Carrier Generation
The ESP32 generates the Low Frequency (LF) carrier using the `LEDC` (LED Control) peripheral.
*   **Peripheral**: `LEDC_TIMER_0`, `LEDC_CHANNEL_0`.
*   **Frequency**: 40 kHz, 60 kHz, 68.5 kHz, or 77.5 kHz (depending on station).
*   **Resolution**: 8-bit duty resolution.

### Amplitude Modulation (AM)
The system simulates AM by dynamically adjusting the PWM duty cycle of the carrier.
*   **High Power (P_high)**: Duty cycle `128/255` (~50%). This represents the maximum fundamental frequency component (square wave).
*   **Low Power (P_low)**: Duty cycle `14/255` (~5.5%). This effectively attenuates the radiated power to simulate the "dipped" carrier state.
*   **Note**: This is not true analog AM, but "PWM-based attenuation". It works effectively for near-field magnetic coupling because the receiver's AGC treats the reduced duty cycle as a lower signal strength.

### Timing and time validity

LEDC generates the carrier continuously in hardware. A dedicated FreeRTOS task on core 1 samples `gettimeofday()` each scheduler tick and derives the current 100 ms modulation slot from absolute time. It writes the duty only when it changes. HTTP handling, Wi-Fi reconnects, serial logging and preference saves run outside that task.

This avoids accumulated tick drift and main-loop delays. It is not a hard real-time guarantee: scheduler latency, interrupts and flash operations can still affect modulation edges and need measurement on hardware.

The radio task owns encoder buffers and calendar state. Web status uses separate snapshots and never updates the frame being transmitted. Configuration changes, a newly valid clock or a wall-clock step greater than 50 ms stop the current output and wait for the next full minute before starting a complete frame.

Only the SNTP notification callback records time trust. Its age is measured with `esp_timer_get_time()`; Wi-Fi state and manual clock edits cannot refresh it. At 23 hours since the last confirmed reply, or before the first reply after boot, the carrier is stopped and the GPIO is set to input. A user pause is kept separate from this eligibility check. The RGB LED reports Wi-Fi/time health and actual transmission eligibility, not modulation pulses. Green means normal transmission, yellow means waiting/alignment or a sync age of at least 3 h 15 min, red means Wi-Fi is disconnected, and blinking red means time expiry or a radio error stopped transmission. User pause and the LED-off setting take precedence. The core RGB/RMT driver updates only when the visible color changes.

---

## WWVB Implementation Logic: `txtempus` vs. `nisejjy`

This project explicitly follows the logic of the reference implementation **[txtempus]** (by Henner Zeller) for WWVB, deviating from the original **nisejjy** logic.

### The Problem (`nisejjy` legacy)
The original `nisejjy` implementation (and many simple Arduino emulators) often attempted to encode **Local Time** into the WWVB frame.
*   **Issue**: WWVB is defined to broadcast **UTC** (Coordinated Universal Time). The receiver logic (the clock itself) handles the Timezone and DST offsets.
*   **Symptom**: Clocks would receive "Local Time" disguised as UTC, then apply *another* offset, resulting in the display being wrong by (GMT Offset + DST).

### The Solution (`txtempus` / `clocksync`)
`clocksync` adopts the `txtempus` philosophy:
1.  **UTC Base**: The framing bits (Minute, Hour, Day, Year) are populated using `gmtime()` (UTC).
2.  **DST Flags**: Bit 57 ("DST Tomorrow") and Bit 58 ("DST Now") are derived from the POSIX TZ setting via `localtime_r()`. For correct WWVB operation, `#define TZ` must be set to a US timezone (e.g., `"PST8PDT,M3.2.0,M11.1.0"`).
3. **Scope**: This describes the retained upstream WWVB implementation. Only DCF77 has been reviewed in this fork; this is not a complete reproduction of a national time service.

> **Reference**: [github.com/hzeller/txtempus](https://github.com/hzeller/txtempus)

---

## Data Encoding Schemes

The stations use different methods to encode values (Time, Date) into bits.

### 1. BCD (Binary-Coded Decimal)
Most stations (WWVB, JJY, DCF77, MSF) use BCD. Each decimal digit is transmitted separately as a 4-bit binary sequence.
*   *Example*: Minute `42` is sent as:
    *   Tens place `4` -> `0100`
    *   Ones place `2` -> `0010`
*   This makes it easy for simple logic circuits (like 1970s hardware clocks) to decode directly to a 7-segment display digit.

### 2. Binary
Some fields (like "Year" in BPC or "DOW" in DCF77) use straight binary encoding for the entire value, rather than splitting by digits.

### 3. Quaternary (Base-4)
Used by **BPC** and **BSF**.
*   Instead of sending 0 or 1 per second, these stations send one of **4 symbols** per second.
*   This literally doubles the data rate (2 bits per second).
*   **Signaling**: The pulse width determines the value:
    *   `00` = 0.1s width
    *   `01` = 0.2s width
    *   `10` = 0.3s width
    *   `11` = 0.4s width
### 4. Pulse Width Encoding (PWM / Shifts)
Used by **WWVB**, **DCF77**, **MSF**, and **JJY**.
*   This is the physical layer encoding: how a single "bit" (0, 1, or Marker) is physically signaled on the carrier.
*   **Concept**: We drop the carrier power (to "Low") for a specific duration *within* the 1-second window.
*   **Start-Low** (WWVB, DCF77, MSF): The second *starts* with reduced power. The duration of this "Low" state defines the symbol.
    *   *Short Low* (e.g., 0.2s) = Binary 0
    *   *Long Low* (e.g., 0.5s) = Binary 1
    *   *Very Long Low* (e.g., 0.8s) = Marker
*   **End-Low** (JJY): The second starts High and ends with a "Low" pulse. The logic is inverted or shifted, but the principle of "duration = value" remains the same.

---

## Protocol Specifications

The signal is constructed of **10 ticks** (100ms each) per second.
Legend: `H` = High Power (~50% duty), `L` = Low Power (~6% duty).

### 1. WWVB (USA) - 60 kHz
*   **Encoding**: Pulse Width Modulation (Start-Low).
*   **Frame**: 60 Seconds. Bit at second 59 marks the start of the *next* minute.

| Symbol | Duration (Low) | Pattern (10 ticks) | Description |
| :--- | :--- | :--- | :--- |
| **0** | 0.2 s | `LLHHHHHHHH` | Binary Zero |
| **1** | 0.5 s | `LLLLLHHHHH` | Binary One |
| **Marker** | 0.8 s | `LLLLLLLLHH` | Frame/Field Marker |

**Frame Layout** (MSB First):
*   **S0, S9, S19...S59**: Fixed Markers. S59 marks the start of the frame.
*   **Minutes**: BCD (Bits 1-8). Tens sent first.
*   **Hours**: BCD (Bits 12-18). Tens sent first.
*   **Day of Year**: BCD (Bits 22-33). Hundreds sent first.
*   **DUT1**: Sign & Correction (Bits 36-43, 56).
*   **Year**: BCD (Bits 45-53). Tens sent first.
*   **DST**: Bit 57 (DST Pending), Bit 58 (DST Active).
*   **Leap Second**: Bit 59 (LS Pending).

### 2. DCF77 (Germany) - 77.5 kHz
*   **Encoding**: Start-Low.
*   **Frame**: One minute with 59 data bits. Second 59 has no amplitude reduction; the carrier remains high.
*   **Time**: Encodes the following minute: add 60 seconds to the UTC frame-start timestamp, then convert to local time. This handles midnight, month/year rollover and DST. Sunday is encoded as 7.
*   **A1**: Bit 16 announces a DST transition during the hour preceding it. Bits 17/18 describe the time encoded for the next minute.

| Symbol | Duration (Low) | Pattern |
| :--- | :--- | :--- |
| **0** | 0.1 s | `LHHHHHHHHH` |
| **1** | 0.2 s | `LLHHHHHHHH` |
| **Marker** | 0.0 s (High) | `HHHHHHHHHH` | (Used at S59) |

**Frame Layout** (LSB First):
*   Bits are transmitted **Least Significant Bit** first.
*   BCD digits are sent **Ones** then **Tens**.

| Second | Field | Description |
| :--- | :--- | :--- |
| 16 | A1 | DST change announcement |
| 17 | Z1 | DST Active (Summer) |
| 18 | Z2 | DST Inactive (Winter) |
| 21-27 | Minutes | BCD (LSB at 21) |
| 28 | **Parity** | Even parity for Minutes |
| 29-34 | Hours | BCD (LSB at 29) |
| 35 | **Parity** | Even parity for Hours |
| 36-41 | Day | BCD (LSB at 36) |
| 42-44 | DOW | Day of Week (LSB at 42) |
| 45-49 | Month | BCD (LSB at 45) |
| 50-57 | Year | BCD (LSB at 50) |
| 58 | **Parity** | Even parity for Date |
| 59 | **Skip** | No modulation (Minute Marker) |

### 3. JJY (Japan) - 40 / 60 kHz
*   **Encoding**: **End-Low**.
*   **Frame**: 60 seconds (Markers at 9, 19, 29, 39, 49, 59).

| Symbol | Description | Pattern (High -> Low) |
| :--- | :--- | :--- |
| **0** | Binary Zero | `HHHHHHHHLL` (0.8s High, 0.2s Low) |
| **1** | Binary One | `HHHHHLLLLL` (0.5s High, 0.5s Low) |
| **Marker** | Marker | `HHLLLLLLLL` (0.2s High, 0.8s Low) |

**Frame Layout** (MSB First):
*   Bits within fields are MSB first.
*   BCD digits are sent **Tens** then **Ones**.

| Second | Field | Description |
| :--- | :--- | :--- |
| 1-3 | Minutes (Tens) | BCD value (e.g. 40 -> 4) |
| 5-8 | Minutes (Ones) | BCD value |
| 9 | **Marker** | Fixed P-Marker |
| 10-11 | Reserved | Fixed 0 |
| 12-13 | Hours (Tens) | BCD value |
| 15-18 | Hours (Ones) | BCD value |
| 19 | **Marker** | Fixed P-Marker |
| 20-21 | Reserved | Fixed 0 |
| 22-23 | DOY (Hundreds) | Day of Year (100s) |
| 25-28 | DOY (Tens) | Day of Year (10s) |
| 29 | **Marker** | Fixed P-Marker |
| 30-33 | DOY (Ones) | Day of Year (1s) |
| 36 | **Parity** | Even parity for Hours (12-18) |
| 37 | **Parity** | Even parity for Minutes (1-8) |
| 39 | **Marker** | Fixed P-Marker |
| 41-44 | Year (Tens) | Year since 2000 (Tens) |
| 45-48 | Year (Ones) | Year since 2000 (Ones) |
| 49 | **Marker** | Fixed P-Marker |
| 50-52 | DOW | Day of Week (0=Sun, 6=Sat) |
| 53-54 | LS1 / LS2 | Leap Second info (Fixed 0 in this impl) |
| 59 | **Marker** | Fixed M-Marker (Start of Frame) |

### 4. MSF (UK) - 60 kHz
*   **Encoding**: Start-Low, with unique parity symbols.
*   **Frame**: 60 Seconds.

| Symbol | Duration (Low) | Pattern |
| :--- | :--- | :--- |
| **0** | 0.1 s | `LHHHHHHHHH` |
| **1** | 0.2 s | `LLHHHHHHHH` |
| **Marker** | 0.5 s | `LLLLLHHHHH` | Minute Marker (S0) |
| **Parity** | Distinct shapes | `LLLHHHHHHH` (Parity version of 0/1 bits) |

**Frame Layout**:

| Second | Field | Description |
| :--- | :--- | :--- |
| 0 | **Marker** | Minute Marker |
| 01-16 | Reserved | Fixed 0 |
| 17-24 | Year | BCD (Tens: 17-20, Ones: 21-24) |
| 25-29 | Month | BCD (Tens: 25, Ones: 26-29) |
| 30-35 | Day | BCD (Tens: 30-31, Ones: 32-35) |
| 36-38 | DOW | Day of Week (0=Sun, 6=Sat) |
| 39-44 | Hours | BCD (Tens: 39-40, Ones: 41-44) |
| 45-51 | Minutes | BCD (Tens: 45-47, Ones: 48-51) |
| 54 | **Parity** | Year (17-24) |
| 55 | **Parity** | Month + Day (25-35) |
| 56 | **Parity** | DOW (36-38) |
| 57 | **Parity** | Time (39-51) |
| 58 | DST | Summer Time Flag |

### 5. BPC (China) - 68.5 kHz
*   **Encoding**: Quaternary (4-level pulse width).
*   **Structure**: One frame = 20 seconds. The frame is repeated 3 times per minute (at 00-19, 20-39, 40-59).
*   **Symbols**:
    *   `0` (00): 1 Pulse (0.1s Low)
    *   `1` (01): 2 Pulses (0.2s Low)
    *   `2` (10): 3 Pulses (0.3s Low)
    *   `3` (11): 4 Pulses (0.4s Low)
    *   `Marker`: 0 Pulses (No modulation)

**Frame Layout (20s Sub-frame)**:

| Offset | Field | Description |
| :--- | :--- | :--- |
| 0 | **Marker** | Sub-frame Sync |
| 1 | Reserved | |
| 3-4 | Hours | Hour % 12 (Base-4 encoded) |
| 5-7 | Minutes | Minute (Base-4) |
| 8-9 | DOW | Day of Week (Base-4) |
| 10 | Hour/Parity | AM/PM bit + Parity for Time |
| 11-13 | Day | Day (Base-4) |
| 14-15 | Month | Month (Base-4) |
| 16-18 | Year | Year (Base-4) |
| 19 | **Parity** | Parity for Date |

*Note: For `clocksync`, seconds 0-1 and 0-2 (in copies) are filled with Marker/Reserved according to standard.*

### 6. BSF (Taiwan) - 77.5 kHz
*   **Encoding**: Quaternary.
*   **Status**: Reverse-engineered.
*   **Layout**:

| Second | Field | Description |
| :--- | :--- | :--- |
| 41-43 | Minutes | BCD |
| 44-46 | Hours | BCD |
| 47-49 | Day | BCD |
| 50 | DOW | Day of Week (0=Sun, 6=Sat) |
| 51-52 | Month | BCD |
| 53-56 | Year | BCD (Year since 2000) |
| 46, 56 | Parity | Parity bits mixed in |

## Wi-Fi provisioning

`WiFiConfiguration` owns station reconnection, setup AP lifetime and provisioned credentials. Boot configuration is loaded in priority order: an explicit AP-only reset record, a valid NVS network record, optional compiled secrets, or immediate AP setup. NVS stores SSID/PSK together as one blob so power loss cannot leave a new SSID paired with an old password. The AP-only record uses the same key and size, with a distinct magic value and empty credentials. This keeps the reset decision and cleared credentials in one record, suppressing compiled secrets until a later successful configuration replaces that record.

The setup AP is `clocksync` with password `clocksync`, at `192.168.4.1`. It starts immediately without credentials, or after 600,000 ms of uninterrupted station disconnection. Reconnect attempts continue every 60 seconds in AP+STA mode. The setup API is restricted to clients reaching the AP interface and requires a per-AP token on writes. It never returns the PSK. A candidate network is saved only after the requested SSID is connected for at least one second. The AP remains available on failure and closes 15 seconds after success. Time trust and transmission gating remain independent of AP mode.

The station-side interface uses `GET /api/wifi/next` to show current and next-startup network names, the pending-restart state, and a per-boot write token. `POST /api/wifi/next` validates and stores credentials for the next boot without mutating the active `ssid_`/`psk_`, calling `WiFi.begin`, or changing reconnect behavior. `POST /api/wifi/reset` stores the AP-only record with empty credentials. Both writes require the token and a connection through the station interface. They are rejected while an immediate AP provisioning candidate is still being saved, so an in-flight provisioning attempt cannot silently overwrite a deferred change. No API returns the password.

The normal UI loads Wi-Fi configuration only when its section opens, keeps edits intact during background status updates, and requires a separate confirmation before forgetting Wi-Fi. Neither operation reboots the device. Restarting applies the saved configuration; an unreachable new network uses the usual 10-minute fallback. The setup portal still connects immediately and saves only after a successful connection.
