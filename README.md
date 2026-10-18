# ClockSync Evolved (v1.6.0)

**Designed specifically for the classic M5Stack Atom Lite (ESP32-PICO-D4).** ClockSync Evolved uses Wi-Fi time and a compact wire coil to sync nearby radio-controlled clocks. The default pins, RGB status LED, button controls and printable holder all target the Atom Lite. **DCF77 and Central European time are the defaults.**

This fork is maintained by [axelaberg91](https://github.com/axelaberg91/ClockSync-Evolved), based on [tanvach/clocksync](https://github.com/tanvach/clocksync) and [SASAKI Taroh's nisejjy](https://github.com/tarohs/nisejjy). It adds the **clocksync evolved** web interface and the printable coil build below.

For a standard dual-core ESP32 DevKit, see the [porting notes below](#other-esp32-boards) for the board settings, pins and physical mounting that may need to change.

Original code, documentation and assets added by **axelaberg91** use **CC BY-NC 4.0**. Commercial use of those contributions requires separate permission; axelaberg91 retains the right to sell their own work. **The inherited firmware's license remains unresolved.** See [licensing and scope](LICENSE.md) and the [hardware asset terms](hardware/LICENSE.md).

## Get started

1. **Build:** [download the print pack](hardware/print/clocksync-atom-lite-print-pack.zip?raw=true) and follow the short guide below.
2. **Install:** upload the firmware using **M5Stack 3.3.10 / M5Atom / Huge APP**. [Installation instructions](#firmware-setup).
3. **Connect Wi-Fi:** on an unconfigured device, join **clocksync**, password **clocksync**, and open `http://192.168.4.1`. Save your 2.4 GHz network, then rejoin it and open `http://clocksync.local` or the device IP.
4. **Use:** keep **DCF77 / GPIO32**, place your clock close to the coil and start its synchronization procedure. A short button press pauses/resumes; hold for 3 seconds to request time sync.

## Build the Atom Lite unit

**M5Stack Atom Lite · 20 turns of 0.5 mm enamelled copper wire · 330 ohm series resistor.**

![Atom placement and series wiring diagram](hardware/images/coil-preview.png)

### Download and print

**[Download the print pack](hardware/print/clocksync-atom-lite-print-pack.zip?raw=true)**, extract it and open `print/print-both.3mf` in your slicer. The pack contains both parts, editable CAD and these build instructions.

Individual files: [coil STL](hardware/coil/coil-75mm-integrated-holder.stl) · [connector clip STL](hardware/connector/dupont-housing-clip.stl) · [both parts as 3MF](hardware/print/print-both.3mf).

The coil former is **75 x 75 x 15 mm**. It includes the Atom snap holder and a side clip for the resistor. Print it as one part; no separate holder or glue is needed. The small connector clip is the second part.

### What you need

| Item | Amount |
| --- | --- |
| M5Stack Atom Lite and USB-C cable | 1 each |
| Enamelled copper wire, 0.5 mm | About 4.8 m including connection leads |
| Axial resistor, 330 ohm / 0.25 W | 1 |
| Jumper leads with complete single-pin Dupont female housings | 2 |
| Heat-shrink tubing | For the resistor and solder joints |

### Assemble and connect

1. Snap the Atom into the holder with USB-C and Grove facing inward.
2. Wind **20 turns** of 0.5 mm enamelled wire around the coil former and bring both ends inside through the shared opening.
3. Remove enamel from the ends, solder the circuit below and insulate the joints. Snap the resistor into the inside clip.
4. Fully insert the Dupont females on **G32 and GND**, then fit the guide over their rear ends. Leave **5V and G26** unused.

```text
G32 (GPIO32) ---- coil (20 turns) ---- 330 ohm ---- GND
```

Connect USB power, select **DCF77 / GPIO32**, and start the clock's synchronization procedure close to the coil.

## Firmware setup

In Arduino IDE, install **M5Stack by M5Stack official, version 3.3.10**, then select:

| Setting | Value |
| --- | --- |
| Tools → Board | M5Stack → **M5Atom** |
| Flash capacity | **4 MB** (Atom Lite) |
| Tools → Partition Scheme | **Huge APP (3MB No OTA/1MB SPIFFS)** |

No extra libraries are needed. Upload the sketch, then configure Wi-Fi through the setup portal. To compile Wi-Fi credentials instead, copy `secrets_example.h` to `secrets.h`; that file is excluded from Git.

DCF77 and **Central Europe (CET/CEST)** are selected by default. Transmission starts at the next whole minute after a confirmed NTP reply. A new reply is required after every boot, and transmission stops after 23 hours without one.

The web interface's **Wi-Fi settings** can save a new network for the next startup or forget the saved network. Saving or forgetting does not disconnect the current connection; restart to apply it.

For another clock protocol, see [Changing the time signal and time zone](#changing-the-time-signal-and-time-zone).

<details>
<summary>Board package and Wi-Fi setup</summary>

Add `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json` under **Additional Board Manager URLs** in Arduino IDE. Install **M5Stack by M5Stack official 3.3.10**, select **M5Atom** (not AtomS3), and use the settings above. [Command-line build instructions](docs/DEVELOPMENT.md#building-from-the-command-line) are available separately.

- With no saved network and no `secrets.h`, join **clocksync**, password **clocksync**, then open `http://192.168.4.1`. A captive portal may also open automatically.
- Enter a 2.4 GHz SSID and password. Setup saves the network only after a successful connection and closes about 15 seconds later. Rejoin that network and open `http://clocksync.local` or the device IP.
- To prepare a network change, open **Wi-Fi settings** and choose **Save for next startup**. The current connection stays active. New credentials are first tried after a restart.
- **Forget Wi-Fi** opens setup at the next startup, even when credentials were compiled into the firmware. It does not disconnect or restart the current session.
- After **10 minutes of continuous Wi-Fi disconnection**, setup becomes available while reconnect attempts continue. A recovered connection resets that timer. Wi-Fi recovery cannot extend the 23-hour time-sync limit.

</details>

## Changing the time signal and time zone

**Both settings are available in the web interface and saved across restarts.** Selecting a signal does not automatically change its time zone. DCF77 with Central European time is the default, including for clocks used in Sweden. After installing this firmware, subsequent signal and time-zone changes need no new upload or restart.

### Select the signal and time zone

1. Open `http://clocksync.local` or the device IP, then expand **Radio settings**.
2. Choose the protocol supported by your clock under **Time signal**, and select its matching **Time zone** using the table below. A signal change selects both the encoder and carrier frequency; the antenna pin stays as selected in the form.
3. Leave **Daylight saving flag** on **Automatic** for normal operation, then select **Save radio settings**.
4. The device stops any active transmission, applies a changed time zone, and starts a complete frame at the next whole minute when time is still trusted. A manual pause stays paused, and a time-zone change cannot extend the 23-hour time-sync limit.
5. Confirm **Time signal**, **Time zone**, and the local clock on the status page. If transmission was already paused, use **Resume transmission** when ready. A receiver may need several complete frames to synchronize.

| Time signal | Carrier | Serial command | Time zone in Radio settings |
| --- | --- | --- | --- |
| DCF77 / Germany | 77.5 kHz | `sd` | Central Europe (CET/CEST) |
| JJY / Fukushima | 40 kHz | `sj` | Japan (JST, UTC+9) |
| JJY / Fukuoka | 60 kHz | `sk` | Japan (JST, UTC+9) |
| WWVB / USA | 60 kHz | `sw` | A US time zone matching the receiver's DST rules; examples below |
| MSF / United Kingdom | 60 kHz | `sm` | United Kingdom (GMT/BST) |
| BSF / Taiwan | 77.5 kHz | `st` | China / Taiwan (UTC+8) |
| BPC / China | 68.5 kHz | `sc` | China / Taiwan (UTC+8) |

The same station commands work in Serial Monitor at **115200 baud**. They change the signal, not the saved time zone. Other station encoders are retained from upstream and have not received the same review as DCF77.

### Time-zone behavior

The selected time zone controls the web page's local clock and the local-time encoders. For DCF77, use Central Europe to match [the station's CET/CEST time code](https://www.ptb.de/cms/fileadmin/internet/publikationen/broschueren/About_Time_2019en.pdf). Older installations without a saved zone, and installations with an unknown saved zone ID, use the Central Europe default.

**WWVB is different:** its frame time is UTC, and the receiver applies its own selected display time zone, as described by [NIST](https://www.nist.gov/pml/time-and-frequency-division/time-distribution/radio-station-wwvb/help-wwvb-radio-controlled). This firmware uses the selected time zone for its WWVB DST flags and local web display. Match the receiver's DST rules; changing the zone does not offset the UTC fields in the WWVB frame.

<details>
<summary>US time-zone examples for WWVB</summary>

| Region | Web selection |
| --- | --- |
| Pacific / Los Angeles | US Pacific (PST/PDT) |
| Mountain / Denver | US Mountain (MST/MDT) |
| Central / Chicago | US Central (CST/CDT) |
| Eastern / New York | US Eastern (EST/EDT) |
| Phoenix / most of Arizona, without DST | Arizona (MST, no DST) |
| Hawaii, without DST | Hawaii (HST, no DST) |

</details>

The web setting **Daylight saving flag** overrides the DCF77/MSF flag; it does not change the time zone or adjust the encoded hour. Leave it on **Automatic** for normal operation. WWVB derives its flags from the selected time zone independently of that override. **UTC (no DST)** is also available for diagnostic use.

Advanced preset and default changes are covered in the [development notes](docs/DEVELOPMENT.md#defaults-and-time-zone-presets).

<details>
<summary>Button, status LED, saved settings and NTP timing</summary>

After a confirmed NTP sync, transmission starts at the next whole minute so the first frame is complete. It can take another minute for a receiver to decode that frame; many clocks require several frames.

- **Status LED**: Green = normal transmission. Yellow = waiting for time/alignment or a confirmed sync is more than 3 h 15 min old. Red = Wi-Fi disconnected while time may still be valid. Blinking red = the 23-hour guard or radio error has stopped transmission. User pause and the LED-off setting override all colors.
- **Button**: Short press pauses/resumes. Hold 3 seconds to request NTP sync.
- **Web UI**: Visit `http://clocksync.local` (or the device IP). Pause/resume, request sync, change radio settings, and prepare Wi-Fi changes for the next startup. Status polls every 2 seconds while the tab is visible. Wi-Fi settings load when their section is opened; they are not polled in the background. The clock updates locally at each displayed second boundary.
- **23-hour limit**: Counts from the last confirmed SNTP reply using monotonic time. Wi-Fi reconnection, manual time changes and pressing resume cannot extend it. After expiry, the transmitter stops and the LED blinks red. A new confirmed sync restores eligibility; a user pause stays paused.
- **Saved settings**: Transmission request, station, time zone, GPIO, drive, LED, DST override, NTP enable and provisioned Wi-Fi survive reboot. The `r` command resets radio settings, including the Central Europe time zone; it leaves provisioned Wi-Fi in its separate namespace. Time trust never survives reboot.
- **NTP interval**: A refresh is requested every 3 hours. Yellow warns after that interval plus 15 minutes; the strict stop remains at 23 hours.

</details>

For Serial commands, HTTP endpoints and protocol details, see [the technical reference](HOW_IT_WORKS.md#commands-and-http-api).

<details>
<summary>Troubleshooting</summary>

*   **Clock isn't syncing**:
    *   **Proximity**: Move the clock *closer* to the antenna. It usually needs to be touching.
    *   **Interference**: LED power supplies and monitors cause noise. Move away from them.
    *   **Drive strength**: Increase **Radio settings → Drive strength** if needed.
    *   **Antenna**: Ensure your antenna wire isn't broken and the resistor is secure.
*   **Clock shows wrong time**:
    *   **Timezone**: Check **Radio settings → Time zone**, then choose **Save radio settings**. See [Changing the time signal and time zone](#changing-the-time-signal-and-time-zone), including the WWVB-specific notes.
    *   **DST off by 1 hour (WWVB)**: Selecting **UTC (no DST)** keeps the DST bits at zero. Choose the US time zone whose DST rules match the receiver.
* **Time zone could not be applied**: Transmission stays stopped. Save radio settings again to retry, or restart. The status page and technical status distinguish the selected zone from the active one.
* **Compilation Error**: Confirm that **M5Stack 3.3.10 / M5Atom** is selected and all project headers are in the sketch folder. External M5Atom/FastLED libraries are not used by this version.

</details>

## Other ESP32 boards

<details>
<summary>Porting to a standard ESP32 DevKit</summary>

The **classic M5Stack Atom Lite** is the development and build target. Other boards require adaptation to their actual hardware. A standard DevKit configuration has not been verified by this project.

For a classic dual-core ESP32 DevKit, review the following settings. A [detailed board comparison](docs/DEVELOPMENT.md#board-comparison) is available in the development notes.

### What to change

1. **Board profile and flash layout:** select the actual board in Arduino IDE, using the appropriate ESP32 board package. Match its flash size and partition layout. The M5Atom profile and the printed Atom holder are specific to the Atom Lite.
2. **Antenna pin and wiring:** review `PIN_RADIO` in `clocksync.ino`. GPIO32 is the default; keep it only if the board exposes it and it is free. Connect the coil and series resistor to the labeled GPIO and GND. The Atom's Grove wire colours and connector positions do not describe a DevKit's headers.
3. **Status LED:** set `PIN_RGB_LED` to the pin of a compatible addressable RGB LED, or `-1` if none is fitted. For a simple LED, set `PIN_LED` instead and disable `PIN_RGB_LED`. The current `indicator()` output is active high; an active-low LED requires that output logic to be inverted. A simple LED cannot display the RGB status colours.
4. **Button:** set `PIN_BUTTON` to the connected active-low button pin, or `-1` to use only the web controls. GPIO34-39 need an external pull-up. Other button inputs use the internal pull-up in the current code. The Atom's GPIO39 button and the DevKit's BOOT button are separate hardware arrangements.
5. **Physical mounting:** the integrated holder fits the Atom Lite enclosure. A DevKit needs a suitable mount or a revised holder with clearances for its board, USB connector and pin headers.

For example, a classic dual-core DevKit with GPIO32 available and no connected status LED or button can start with these definitions in `clocksync.ino`:

```cpp
#define PIN_RADIO 32
#define PIN_LED -1
#define PIN_RGB_LED -1
#define PIN_BUTTON -1
```

This example disables the physical status indicators and button; the web controls remain available. It is a porting starting point, not a verified DevKit build.

The current radio-pin allowlist in `safePin()` and the web selector are **19, 21, 22, 23, 25, 26, 32 and 33**. If a port needs a different radio pin, update both `safePin()` in `clocksync.ino` and the antenna-pin selector in `web_ui.h` after checking that pin against the board's hardware. Optional frequency measurement uses `PIN_MEAS` (GPIO33 by default); review it if using the measurement jumper.

If the board already has saved radio settings, check the antenna pin in the web interface: the saved pin takes precedence over the compiled default.

The radio task is pinned to **core 1**. These notes apply to a classic **dual-core** ESP32; single-core devices need a separate review of task placement, GPIOs and peripherals. ESP32-S3 and other families also need their own board-specific review. Confirm the carrier frequency and modulation timing on the adapted hardware.

The **M5Stack board package** supplies the framework, compiler and board profile. This fork no longer needs the separate M5Atom or FastLED libraries for the Atom Lite's one status pixel and button. The web interface does not display a board-specific label.

A simple onboard LED can show on/off/blinking, but needs to be replaced or supplemented with an addressable RGB pixel to display the green/yellow/red status colors.

The standard DevKit hardware configuration has not been verified by this project. See [Atom Lite's official pin map](https://docs.m5stack.com/en/core/Atom-Lite) for the Atom connections.

</details>

## Limits and verification

DCF77 is the main development target; the other encoders are retained from upstream and have not received the same review. This fork does not generate DCF77 leap-second announcements or leap-second frames.

The v1.6.0 time-zone changes have not been compiled or tested on an Atom Lite. Earlier build results do not verify this revision or its emitted signal. See the [development and verification record](docs/DEVELOPMENT.md#verification-record-and-limits) for details.

## Documentation

- [Technical reference and API](HOW_IT_WORKS.md)
- [Hardware, editable CAD and dimensional references](hardware/README.md)
- [Release history](CHANGELOG.md)
- [Development notes, build records and source history](docs/DEVELOPMENT.md)

## Credits

- **Original Author**: SASAKI Taroh (tarohs) - [nisejjy](https://github.com/tarohs/nisejjy)
- **Upstream clocksync**: Tanvach - [tanvach/clocksync](https://github.com/tanvach/clocksync)
- **Fork improvements (v1.4.0-v1.6.0)**: [axelaberg91](https://github.com/axelaberg91/ClockSync-Evolved) - DCF77 encoding and timing fixes; confirmed-NTP and 23-hour transmission safeguards; Atom Lite button and RGB status handling; Wi-Fi setup, fallback and next-startup configuration; and the English **clocksync evolved** web interface with saved time-zone selection.
- **Reference**: [txtempus](https://github.com/hzeller/txtempus) by Henner Zeller (used for WWVB frame verification).
- **Printable hardware**: developed for Axel's build with Codex assistance, starting from Axel's original coil-former concept.
