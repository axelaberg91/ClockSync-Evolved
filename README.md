# clocksync (v1.5.0)

**ESP32 Fake Radio Clock Station**

`clocksync` allows an ESP32 to emulate various Low-Frequency (LF) time signal stations, allowing you to sync radio-controlled clocks (JJY, WWVB, DCF77, MSF, etc.) even if you are out of range of the actual transmitters.

This fork is maintained by [axelaberg91](https://github.com/axelaberg91/clocksync) and targets the M5Stack Atom Lite with DCF77 and Central European time by default. It is based on tanvach/clocksync and the original nisejjy project by SASAKI Taroh. Its web interface is named **clocksync evolved**.

## Source history

- [upstream-baseline](https://github.com/axelaberg91/clocksync/tree/upstream-baseline) preserves the original fork snapshot from `tanvach/clocksync`, commit `63d92cae27f5085cbd8474151474e6ea8660a867` (v1.3.1), before the changes credited to axelaberg91.
- [v1.5.0](https://github.com/axelaberg91/clocksync/tree/v1.5.0) marks the current clocksync evolved version. The default `master` branch contains the current fork code, with the original Git history retained.
- Local Wi-Fi credentials (`secrets.h` or `secret.h`) and compiled firmware files are excluded from version control. Use `secrets_example.h` as a template or configure Wi-Fi through the setup portal.

### What's New in v1.5.0

- The status interface, setup portal and device messages use English. The local clock keeps Central European time and now retains fractional seconds when refreshing its display.
- The normal web interface has **Wi-Fi settings**. **Save for next startup** stores a new SSID/password without changing the current connection or its reconnect settings. Credentials are first tried after restarting the device.
- **Forget Wi-Fi** clears the saved network after confirmation and makes the next startup open the setup AP. The reset also overrides compiled `secrets.h` credentials until a new network is saved; the current connection stays active until restart.
- Current and next-startup networks are shown separately. Saved passwords are never returned to the browser. The healthy-connection footer text has been removed.
- Status LEDs use distinct green, yellow and red channel values at low brightness.

### What's New in v1.4.0

- DCF77 uses a 77,500 Hz carrier request with 8-bit LEDC PWM. Technical status shows the actual configured frequency, which may differ slightly because of the hardware divider.
- The DCF77 frame carries the following minute, including date rollover, Sunday = 7, DST flags and the A1 change announcement.
- Transmission requires a confirmed SNTP reply after every boot. It stops after 23 hours without a new reply; Wi-Fi connectivity alone does not reset this limit.
- Wi-Fi reconnect attempts continue every 60 seconds. Network operations and HTTP run separately from the radio task.
- Short button press pauses/resumes the requested transmission state. Hold for 3 seconds to request time sync; release does not also toggle transmission.
- Status LED: green for normal transmission, yellow while waiting for NTP/frame alignment or after 3 h 15 min without a confirmed sync, red for disconnected Wi-Fi, blinking red for expired time or a radio error. A user pause or disabling the LED turns it off.
- A compact Swedish web interface shows transmitter status, connection, last confirmed sync, LED state and time remaining, with mobile layout and advanced settings.
- Built-in Wi-Fi provisioning: AP SSID/password `clocksync`, immediate when unconfigured or after 10 minutes of continuous Wi-Fi loss. A successful setup is stored in NVS and overrides compiled credentials.
- Target build: M5Stack board package 3.3.10, board M5Atom. RGB and debounced button control use the ESP32 core directly; no external M5Atom/FastLED libraries are required.

### What's New in v1.3.1

- **Prevent NVS flash wear**: Added `WiFi.persistent(false)` to stop the WiFi library from writing credentials to flash on every connect/disconnect cycle.
- **Buzzer default off**: Buzzer now defaults to off on fresh install and reset.
- **Tristate radio pin when TX off**: Radio output pin is now set to high-impedance (INPUT) when TX is disabled or when switching pins, instead of being driven LOW.

### What's New in v1.3

- **Fix WWVB DST bits**: Changed default TZ from `"UTC0"` to `"PST8PDT,M3.2.0,M11.1.0"`. With `"UTC0"`, DST status bits were always 0, causing clocks to stay on standard time after DST started. Updated documentation and status display to clarify that WWVB DST bits are derived from the TZ setting.
- **Remove dead `q` command**: Removed unused `q0/q1/q2` WWVB pending override command and related dead code (`wwvbPendingOverride`, `getWWVBPendFlag`).

## Features

- **Multi-Station Support**: Emulates JJY (Japan), WWVB (USA), DCF77 (Germany), MSF (UK), BSF (Taiwan), and BPC (China).
- **Web Control**: Simple web interface for status monitoring and configuration.
- **Home Assistant Integration**: REST API for scheduling transmission (e.g., run only at night).
- **Signal Timing**: Hardware LEDC generates the carrier. A dedicated radio task applies modulation from absolute wall-clock time, independently of web requests.

## Getting Started

### 1. Hardware Setup

**Supported Boards**:
- **M5Stack Atom Lite** (Recommended): Works out of the box. The RGB LED indicates status, and the button toggles transmission.
- **Other ESP32 boards**: Require board-specific antenna, LED and button pin settings. This version has been compiled for Atom Lite; ESP32-S3 compatibility has not been verified.

### Atom Lite versus a standard ESP32 DevKit

Here, "standard ESP32" means a classic, dual-core ESP32 DevKit/WROOM board. ESP32-S3, C3 and other chip families need their own review.

| Item | Atom Lite | Standard ESP32 DevKit |
| :--- | :--- | :--- |
| Board selection | **M5Atom** in M5Stack's package | Usually **ESP32 Dev Module** in Espressif's package; match the actual board |
| Processor | ESP32-PICO-D4, dual core, up to 240 MHz | Classic ESP32/WROOM uses the same processor architecture and LEDC peripheral |
| DCF77 and NTP logic | This project's normal implementation | The protocol, 23-hour guard and 77,500 Hz carrier request need no board-specific changes |
| Antenna output | GPIO32 on the Grove connector's white wire, through the coil and series resistor to GND | GPIO32 can be used if it is exposed and free; wire to the labeled GPIO, not a connector position |
| Status LED | Addressable RGB LED on GPIO27 via the core RGB driver (`PIN_RGB_LED`) | Set `PIN_RGB_LED` to a suitable addressable LED pin, or `-1`. For a simple LED, set `PIN_LED` and check its polarity |
| Button | Active-low button on GPIO39 (`PIN_BUTTON`), external pull-up on Atom Lite, 10 ms software debounce | Set `PIN_BUTTON` to the connected button pin or `-1` to disable it. GPIO34–39 require an external pull-up. BOOT is not automatically the application control button |
| Flash | 4 MB; use a partition scheme for 4 MB flash | Match the actual flash size, flash mode and partition layout; PSRAM is not required |
| Hardware initialization | Core GPIO/RGB functions; no M5Atom library | Adjust the LED/button pin definitions. Changing the board menu does not add missing physical hardware |

The **M5Stack board package** supplies the framework, compiler and board profile. This fork no longer needs the separate M5Atom or FastLED libraries for the Atom Lite's one status pixel and button. The web interface does not display a board-specific label.

A simple onboard LED can show on/off/blinking, but needs to be replaced or supplemented with an addressable RGB pixel to display the green/yellow/red status colors.

The standard DevKit hardware configuration has not been verified by this project. See [Atom Lite's official pin map](https://docs.m5stack.com/en/core/Atom-Lite) for the Atom connections.

**Wiring Diagram**:
```text
           ESP32 / M5Atom
          +-------------+
          |             |
          |     GND [ ]----(Resistor 220-330Ω)----+
          |             |                         |
          | GPIO 32 [ ]-----------------+         |
          |             |               |         |
          +-------------+               |         |
                                   [ Antenna ]    |
                                   [  Coil   ]    |
                                        |         |
                                        +---------+
```

1.  **Pin**: Connect one end of your antenna to `GPIO 32` (default).
    *   *Note*: GPIO 32 is available on the Atom Lite Grove connector (white signal wire). Use GND (black), not the red 5 V wire.
2.  **Ground**: Connect the other end of the antenna to a **Current Limiting Resistor** (220Ω - 330Ω), and then to `GND`.
    *   *Why a resistor?* It protects your ESP32 from drawing too much current, as the antenna coil has very low resistance.
3.  **Antenna Types**:
    *   **Ferrite Rod (Recommended)**: Scavenge one from an old radio clock or buy a ferrite antenna tuned to your target frequency. **Important**: You must match the frequency! (Tested range > 1m)
        *   **60 kHz**: For WWVB (USA), MSF (UK), JJY (Japan).
        *   **40 kHz**: For JJY (Japan).
        *   **77.5 kHz**: For DCF77 (Germany), BSF (Taiwan).
        *   **68.5 kHz**: For BPC (China).
    *   **Wire Loop**: A simple coil of wire (e.g., 30 turns of magnet wire around a water bottle). Short range, but works for any frequency.
4.  **Placement**: Place your target watch/clock **inside** or **immediately next to** the antenna coil. This is a low-power near-field emulator; range is typically < 10cm.

### 2. Software Configuration
1. **Wi-Fi setup**:
    * With no saved network and no `secrets.h`, the device immediately starts the AP **clocksync**, password **clocksync**. Connect to it and open `http://192.168.4.1` (a captive portal may also open automatically).
    * Enter the target 2.4 GHz SSID and PSK. The previous stored network is kept until the new connection succeeds. A successful network is saved as one NVS record and survives reboot.
    * To prepare a network change in advance, open **Wi-Fi settings** on the normal status page. Enter the target SSID and password, then choose **Save for next startup**. The current network remains connected, including when it needs to reconnect before restart. New credentials are saved without testing them; they are first used after a restart. The existing 10-minute AP fallback applies if the new connection fails.
    * **Forget Wi-Fi** asks for confirmation, clears the saved network, and makes the next startup open **clocksync** immediately. It does not restart or disconnect the device. This explicit reset suppresses compiled `secrets.h` credentials and stays in effect across restarts until a new network is saved through the setup portal or normal Wi-Fi settings.
    * The setup AP closes about 15 seconds after a confirmed connection. Rejoin your normal Wi-Fi and open `http://clocksync.local` or the IP shown on the setup page/serial monitor.
    * After **10 minutes of continuous Wi-Fi disconnection**, setup AP mode starts automatically while station reconnect attempts continue every 60 seconds. This also recovers from a changed SSID/password. A short disconnection that recovers resets the 10-minute timer.
    * Boot priority: **explicit Wi-Fi reset**, saved configuration, optional compiled `secrets.h`, then setup AP. To compile credentials, copy `secrets_example.h` to `secrets.h` and fill in the two defines. The secrets file is ignored by Git and is optional. Forgetting Wi-Fi does not remove credentials embedded in the firmware; it prevents their use at startup.
    * Wi-Fi passwords are never returned by the APIs. Network names are shown so you can distinguish the current and next-startup settings. The transmit guard is independent: setup AP mode does not validate time or reset the 23-hour limit.
2.  **Timezone (Critical)**:
    *   Open `clocksync.ino`.
    *   Find the line `#define TZ "..."`.
    *   Change the string to match your station. ESP32 requires a POSIX TZ string (not Olson names like `America/Los_Angeles`).
    *   **WWVB (USA)**: Set TZ to your US timezone. WWVB frame time is always UTC (via `gmtime()`), so there is no double-offset risk — TZ only affects the DST status bits.

        | Region | POSIX TZ string |
        | :--- | :--- |
        | Pacific (LA, SF, Seattle) | `PST8PDT,M3.2.0,M11.1.0` |
        | Mountain (Denver, Phoenix†) | `MST7MDT,M3.2.0,M11.1.0` |
        | Central (Chicago, Dallas) | `CST6CDT,M3.2.0,M11.1.0` |
        | Eastern (NYC, Miami) | `EST5EDT,M3.2.0,M11.1.0` |
        | Hawaii | `HST10` |

        †Arizona (no DST): use `MST7` instead.

    *   **Other stations**: These transmit local time directly, so TZ must match the station's native timezone:

        | Station | Region | POSIX TZ string |
        | :--- | :--- | :--- |
        | JJY | Japan | `JST-9` |
        | DCF77 | Germany / Central Europe | `CET-1CEST,M3.5.0/2,M10.5.0/3` |
        | MSF | United Kingdom | `GMT0BST,M3.5.0/1,M10.5.0` |
        | BSF | Taiwan | `CST-8` |
        | BPC | China | `CST-8` |

    *   **Lookup**: Find any POSIX TZ string by city name in [this table](https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv). On Mac/Linux: `tail -1 /etc/localtime`
3. **Dependencies & Upload**:
    * Target: **M5Stack by M5Stack official 3.3.10**, board **M5Atom** (the classic Atom Lite, not AtomS3).
    * Add `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json` under Additional Board Manager URLs. Install version **3.3.10** and select **Tools → Board → M5Stack → M5Atom**.
    * Use **Huge APP (3MB No OTA/1MB SPIFFS)** with the Atom Lite's **4 MB** flash. No external Arduino libraries are required.
    * From the directory containing the project:

        ```bash
        arduino-cli core update-index --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
        arduino-cli core install m5stack:esp32@3.3.10 --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
        arduino-cli compile --fqbn m5stack:esp32:m5stack_atom:PartitionScheme=huge_app clocksync
        arduino-cli upload -p COM_PORT --fqbn m5stack:esp32:m5stack_atom:PartitionScheme=huge_app clocksync
        ```

    * The older Espressif 2.0.17 build is a historical compatibility result. The current target and configuration instructions above use M5Stack 3.3.10.

### 3. Usage

After a confirmed NTP sync, transmission starts at the next whole minute so the first frame is complete. It can take another minute for a receiver to decode that frame; many clocks require several frames.

- **Status LED**: Green = normal transmission. Yellow = waiting for time/alignment or a confirmed sync is more than 3 h 15 min old. Red = Wi-Fi disconnected while time may still be valid. Blinking red = the 23-hour guard or radio error has stopped transmission. User pause and the LED-off setting override all colors.
- **Button**: Short press pauses/resumes. Hold 3 seconds to request NTP sync.
- **Web UI**: Visit `http://clocksync.local` (or the device IP). Pause/resume, request sync, change radio settings, and prepare Wi-Fi changes for the next startup. Status polls every 2 seconds while the tab is visible. Wi-Fi settings load when their section is opened; they are not polled in the background. The clock updates locally at each displayed second boundary.
- **23-hour limit**: Counts from the last confirmed SNTP reply using monotonic time. Wi-Fi reconnection, manual time changes and pressing resume cannot extend it. After expiry, the transmitter stops and the LED blinks red. A new confirmed sync restores eligibility; a user pause stays paused.
- **Saved settings**: Transmission request, station, GPIO, drive, LED, DST override, NTP enable and provisioned Wi-Fi survive reboot. The `r` command resets radio settings; it leaves provisioned Wi-Fi in its separate namespace. Time trust never survives reboot.
- **NTP interval**: A refresh is requested every 3 hours. Yellow warns after that interval plus 15 minutes; the strict stop remains at 23 hours.

## Command Reference

Control the device via Serial Monitor (115200 baud) or via HTTP (`http://clocksync.local/cmd?c=<command>`).

| Command | Description | Example |
| :--- | :--- | :--- |
| **Stations** | | |
| `sj` | Set station to **JJY (40 kHz)** (Fukushima) | `sj` |
| `sk` | Set station to **JJY (60 kHz)** (Fukuoka) | `sk` |
| `sw` | Set station to **WWVB (60 kHz)** (USA) | `sw` |
| `sd` | Set station to **DCF77 (77.5 kHz)** (Germany) | `sd` |
| `sm` | Set station to **MSF (60 kHz)** (UK) | `sm` |
| `st` | Set station to **BSF (77.5 kHz)** (Taiwan) | `st` |
| `sc` | Set station to **BPC (68.5 kHz)** (China) | `sc` |
| **Control** | | |
| `e1` / `e0` | **Enable / Disable Transmission** (Carrier & Logic) | `e1` |
| `pNN` | Set Radio Output Pin (GPIO `NN`) | `p25` |
| `g0` - `g3` | Set Drive Strength (0=Weakest, 3=Strongest) | `g2` |
| **Settings** | | |
| `y1` / `y0` | NTP Sync On / Off (23-hour guard still applies) | `y1` |
| `u` | Request a fresh NTP sync and enable NTP | `u` |
| `l1` / `l0` | LED On / Off | `l1` |
| `x0` - `x2` | DCF/MSF DST Override (0=Std, 1=DST, 2=Auto) | `x2` |
| **Manual Time** | | |
| `dYYMMDD` | Set Date; invalidates time trust until a new NTP reply | `d231225` |
| `tHHmmSS` | Set Time; invalidates time trust until a new NTP reply | `t123000` |
| **Debug** | | |
| `h` | Show Help | `h` |
| `f` | Frequency Self-Test (Requires jumper `PIN_RADIO` -> `PIN_MEAS`) | `f` |
| `r` | Restore default settings and request NTP sync | `r` |

### HTTP endpoints

- `GET /`: web interface stored in flash, with no external assets.
- `GET /api/status`: JSON status, including requested/actual transmission, NTP age and holdover remaining.
- `GET /status.txt`: diagnostic text (including actual configured carrier frequency).
- `POST /cmd`: form field `c` with a command above. Legacy `GET /cmd?c=...` is also accepted.
- `POST /api/settings`: form fields `station`, `pin`, `drive`, `led`, `dst`. Invalid values are rejected together.
- `GET /api/wifi`: setup status and CSRF token, available only through the setup AP interface; never returns the PSK.
- `POST /api/wifi`: form fields `ssid`, `psk`, `token` from the setup AP. Starts a nonblocking station connection and saves only after success.

## Supported Stations

| Station | Freq (kHz) | Location | Notes |
| :--- | :--- | :--- | :--- |
| **JJY** | 40 / 60 | Japan | Default is end-Low symbol shape. |
| **WWVB** | 60 | USA | UTC framing; DST bits derived from TZ setting. |
| **DCF77** | 77.5 | Germany | "Start-Low" encoding. |
| **MSF** | 60 | UK | Includes parity and DST bits. |
| **BSF** | 77.5 | Taiwan | Quaternary encoding (uncertified). |
| **BPC** | 68.5 | China | Quaternary encoding (uncertified). |

## Troubleshooting
*   **Clock isn't syncing**:
    *   **Proximity**: Move the clock *closer* to the antenna. It usually needs to be touching.
    *   **Interference**: LED power supplies and monitors cause noise. Move away from them.
    *   **Volume**: Increase "Volume" (Drive Strength) to `g3` using the Serial/Web command.
    *   **Antenna**: Ensure your antenna wire isn't broken and the resistor is secure.
*   **Clock shows wrong time**:
    *   **Timezone**: Check the `#define TZ` line in `clocksync.ino`. For WWVB, use a US POSIX timezone (see setup step 2).
    *   **DST off by 1 hour (WWVB)**: If TZ is set to `"UTC0"`, DST bits will always be 0 and your clock won't spring forward. Set TZ to your US timezone.
* **Compilation Error**: Confirm that **M5Stack 3.3.10 / M5Atom** is selected and all project headers are in the sketch folder. External M5Atom/FastLED libraries are not used by this version.

## Technical Details

For deep dives into the signal encoding and protocol specifications used by this emulator, see [HOW_IT_WORKS.md](./HOW_IT_WORKS.md).

## Credits

- **Original Author**: SASAKI Taroh (tarohs) - [nisejjy](https://github.com/tarohs/nisejjy)
- **Upstream clocksync**: Tanvach - [tanvach/clocksync](https://github.com/tanvach/clocksync)
- **Fork improvements (v1.4.0-v1.5.0)**: [axelaberg91](https://github.com/axelaberg91/clocksync) - DCF77 encoding and timing fixes; confirmed-NTP and 23-hour transmission safeguards; Atom Lite button and RGB status handling; Wi-Fi setup, fallback and next-startup configuration; and the English **clocksync evolved** web interface.
- **Reference**: [txtempus](https://github.com/hzeller/txtempus) by Henner Zeller (used for WWVB frame verification).

## Verification and limits

The earlier v1.4.0 credential variants were compiled successfully with **M5Stack 3.3.10 / M5Atom / Huge APP** on 2026-10-17: with `secrets.h` and from a separate source copy without it. Compiler dependencies confirm that the latter does not include `secrets.h`. Those builds used approximately **1.08 MB program storage and 53,520 bytes of static RAM**. The package produced pin-macro redefinition warnings from its own Atom variant header, with no compilation errors. No vendor files were modified to suppress those warnings. These figures do not measure the current v1.5.0 revision.

Host-side tests in `tests/logic_test.cpp` cover 13 DCF77 calendar/DST cases, all three parity fields, the exact 23-hour boundary, button gestures/debounce and RGB status priority. They all pass. They use fixed local-time fixtures and do not validate ESP32 libc timezone behavior or electrical output. The Wi-Fi form was checked in a browser with simulated responses, including successful submission and a 390-pixel layout without horizontal overflow.

A physical Atom Lite, antenna, receiver and frequency/timing measurements are still required to verify the emitted signal and operation under Wi-Fi/web load. This fork does not generate DCF77 leap-second announcements or leap-second frames. Other station encoders are retained from upstream and have not received the same review as DCF77.

The AP start/10-minute fallback, real Wi-Fi authentication, NVS persistence across power loss and AP shutdown must also be checked on a physical Atom. A successful build and simulated web responses do not verify those radio/network behaviors.

The v1.5.0 English interface, deferred Wi-Fi changes and explicit Wi-Fi reset have not been rebuilt or exercised on hardware. In particular, confirm that the old network stays active after saving, that the new network is selected after restart, and that forgetting Wi-Fi opens the setup AP even when the firmware contains compiled credentials.
