# clocksync — Atom Lite DCF77

Sync a radio-controlled clock from Wi-Fi time using an M5Stack Atom Lite and a small wire coil. Control it from your browser or the Atom's button.

This fork adds a **printable coil former with an integrated Atom holder, resistor clip and Dupont connector guide**, with the working build described below. The software builds on [tanvach/clocksync](https://github.com/tanvach/clocksync), originally based on [SASAKI Taroh's nisejjy](https://github.com/tarohs/nisejjy).

## Get started

1. **Build:** [download the print pack](hardware/print/clocksync-atom-lite-print-pack.zip?raw=true) and follow the short guide below.
2. **Install:** [set up Wi-Fi and upload the firmware](#firmware-setup).
3. **Use:** open `http://clocksync.local` or the device's IP, select **DCF77** and **GPIO32**, and place your clock close to the coil. The Atom button toggles transmission.

## Build the Atom Lite unit

**Printed, fitted and tested by Axel:** M5Stack Atom Lite, **20 turns of 0.5 mm enamelled copper wire**, and a **330 ohm series resistor**.

![Coil former with integrated Atom holder](hardware/images/coil-preview.png)

### Download and print

**[Download the print pack](hardware/print/clocksync-atom-lite-print-pack.zip?raw=true)**, extract it and open `print/print-both.3mf` in your slicer. The pack contains both parts, editable CAD and these build instructions.

Individual files: [coil STL](hardware/coil/coil-75mm-integrated-holder.stl) · [connector clip STL](hardware/connector/dupont-housing-clip.stl) · [both parts as 3MF](hardware/print/print-both.3mf).

For **Prusa CORE One / PLA**, start with a **0.4 mm nozzle, 0.12 mm layers, 4 perimeters, 20% gyroid, no supports**, at **100% scale**. Use your own printer/material profile. The 3MF contains geometry and placement, not G-code.

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

1. Let the print cool, then snap the Atom into the holder with USB-C and Grove facing the large open area inside the coil. The button remains accessible.
2. Leave about 20 cm at each end and wind **20 turns** around the outside of the core. A convenient arrangement is **15 turns in the first layer, then 5 in the second**. Keep the same rotational direction around the coil. Route both leads through the single exit to the inside.
3. With USB disconnected, remove enamel from the wire ends, solder the series circuit below and insulate the joints. Let the sleeved resistor cool before snapping it into the side clip. The clip accepts a sleeved diameter of about 3.0-3.4 mm.

```text
Grove G32 (GPIO32) ---- coil ---- 330 ohm resistor ---- Grove GND
```

Use **G32 and GND, the two outer Grove positions below USB-C**. Leave the middle pins, **5V and G26**, unused. Identify pins from the [Atom Lite pin map](https://docs.m5stack.com/en/core/Atom-Lite), not left/right in a picture.

4. Fully seat the two complete Dupont females first, then snap the printed guide over their **rear ends**. Hook each housing under its inner lip, then press its outer edge into the clip. The front ends stay free so both contacts can bottom out. Check that the guide does not push either female back; grip the housings when unplugging.
5. Check continuity through the coil and resistor, connect USB power, select **DCF77 (77.5 kHz)** and **GPIO32**, then start the receiving clock's synchronization procedure close to the coil. Adjust its position and orientation as needed.

Axel's complete Dupont housings fit the Grove port; other housings can vary. Keep the 330 ohm resistor in series. Cable plugs and bends need space beyond the printed 15 mm height.

## Firmware setup

The current sketch uses Arduino IDE and a local `secrets.h` for Wi-Fi credentials. For this DCF77 build, use the POSIX time zone **`CET-1CEST,M3.5.0/2,M10.5.0/3`**. After uploading, open `http://clocksync.local` or the device's IP to configure and control it.

<details>
<summary>Installation, dependencies and other time zones</summary>

1.  **WiFi Secrets**:
    *   Find the file `secrets_example.h` in the file list.
    *   **Duplicate** it and rename the copy to `secrets.h`.
    *   Open `secrets.h` and fill in your WiFi details:
        ```cpp
        #define WIFI_SSID "Your_WiFi_Name"
        #define WIFI_PASS "Your_WiFi_Password"
        ```
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
3.  **Dependencies & Upload**:
    *   **Using Arduino IDE**:
        1.  **Add Board URL**: Go to `File` -> `Preferences` -> `Additional boards manager URLs` and add `https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json`.
        2.  **Install Board**: Go to `Tools` -> `Board` -> `Boards Manager`, search for **M5Stack** and install it.
        3.  **Install Library**: Go to `Sketch` -> `Include Library` -> `Manage Libraries...`. Search for and install **M5Atom** by M5Stack.
        4.  **Select & Upload**: Select **M5Stack-ATOM** (or "ESP32 Dev Module") from the `Tools` -> `Board` menu, connect your device, and click Upload (→).
    *   **Using arduino-cli**:
        ```bash
        arduino-cli core update-index --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
        arduino-cli core install m5stack:esp32 --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
        arduino-cli lib install M5Atom
        arduino-cli compile --fqbn m5stack:esp32:m5stack_atom clocksync.ino
        arduino-cli upload -p /dev/ttyUSB0 --fqbn m5stack:esp32:m5stack_atom clocksync.ino
        ```

</details>

<details>
<summary>Commands and other supported stations</summary>

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
| `y1` / `y0` | NTP Sync On / Off | `y1` |
| `l1` / `l0` | LED On / Off | `l1` |
| `x0` - `x2` | DCF/MSF DST Override (0=Std, 1=DST, 2=Auto) | `x2` |
| **Manual Time** | | |
| `dYYMMDD` | Manually set Date (Year, Month, Day) | `d231225` |
| `tHHmmSS` | Manually set Time (Hour, Min, Sec) | `t123000` |
| **Debug** | | |
| `h` | Show Help | `h` |
| `f` | Frequency Self-Test (Requires jumper `PIN_RADIO` -> `PIN_MEAS`) | `f` |
| `status` | (HTTP only) Get text status summary | |

## Supported Stations

| Station | Freq (kHz) | Location | Notes |
| :--- | :--- | :--- | :--- |
| **JJY** | 40 / 60 | Japan | Default is end-Low symbol shape. |
| **WWVB** | 60 | USA | UTC framing; DST bits derived from TZ setting. |
| **DCF77** | 77.5 | Germany | "Start-Low" encoding. |
| **MSF** | 60 | UK | Includes parity and DST bits. |
| **BSF** | 77.5 | Taiwan | Quaternary encoding (uncertified). |
| **BPC** | 68.5 | China | Quaternary encoding (uncertified). |

</details>

If your clock does not sync, check the output pin, station, connector seating, enamel removal and continuity through the coil and resistor. Then move or rotate the clock close to the coil. The tested build uses 20 turns and 330 ohm.

## More details

<details>
<summary>CAD sources and design checks</summary>

- [Coil OpenSCAD](hardware/coil/coil-75mm-integrated-holder.scad) · [connector clip OpenSCAD](hardware/connector/dupont-housing-clip.scad).
- [Coil geometry report](hardware/coil/geometry-checks.json) · [connector geometry report](hardware/connector/geometry-checks.json).
- The coil and clip are each one closed connected mesh. STL geometry matches the delivered designs. The combined plate was sliced in PrusaSlicer 2.9.6 without support or stability warnings.
- Physical printing, fit and operation were confirmed by Axel with 20 turns and 330 ohm. Print settings above are a starting point for reproducing the build.
- CAD has some fixed coordinates: recheck fit and slicing after editing dimensions.

</details>

<details>
<summary>How the signal works and upstream firmware changes</summary>

See [HOW_IT_WORKS.md](HOW_IT_WORKS.md) for protocol and timing details. Inherited firmware release notes (v1.3.1 and v1.3):

- **Prevent NVS flash wear**: Added `WiFi.persistent(false)` to stop the WiFi library from writing credentials to flash on every connect/disconnect cycle.
- **Buzzer default off**: Buzzer now defaults to off on fresh install and reset.
- **Tristate radio pin when TX off**: Radio output pin is now set to high-impedance (INPUT) when TX is disabled or when switching pins, instead of being driven LOW.

### What's New in v1.3

- **Fix WWVB DST bits**: Changed default TZ from `"UTC0"` to `"PST8PDT,M3.2.0,M11.1.0"`. With `"UTC0"`, DST status bits were always 0, causing clocks to stay on standard time after DST started. Updated documentation and status display to clarify that WWVB DST bits are derived from the TZ setting.
- **Remove dead `q` command**: Removed unused `q0/q1/q2` WWVB pending override command and related dead code (`wwvbPendingOverride`, `getWWVBPendFlag`).

</details>

## Credits

Thanks to **[Tan Vachiramon](https://github.com/tanvach/clocksync)** for the clocksync base, **[SASAKI Taroh](https://github.com/tarohs/nisejjy)** for the original nisejjy project, and **[Henner Zeller](https://github.com/hzeller/txtempus)** for txtempus, an upstream signal-verification reference. The printable parts were developed for Axel's build with Codex assistance, starting from Axel's original coil-former concept.

<details>
<summary>Dimensional references and license status</summary>

Hardware references: [M5Stack Atom Lite enclosure](https://github.com/m5stack/M5_Hardware/tree/master/Products/C008_Atom-Lite/Structures), [Pololu connector housing](https://www.pololu.com/product/1900) and [Grove connector drawing](https://statics3.seeedstudio.com/images/opl/datasheet/3470130P1.pdf). Reference meshes are not included.

The inherited repository has no LICENSE file. Existing attribution is preserved; this addition does not relicense upstream code. A separate license for the new CAD has not been selected.

</details>
