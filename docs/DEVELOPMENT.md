# Development notes

Original documentation contributions by **axelaberg91** use **CC BY-NC 4.0**; inherited material is not relicensed. See [LICENSE.md](../LICENSE.md).

For installation, wiring and normal operation, use [the main guide](../README.md). For architecture, encoders and APIs, use [the technical reference](../HOW_IT_WORKS.md).

## Source history

- [upstream-baseline](https://github.com/axelaberg91/ClockSync-Evolved/tree/upstream-baseline) preserves the original fork snapshot from `tanvach/clocksync`, commit `63d92cae27f5085cbd8474151474e6ea8660a867` (v1.3.1), before the changes credited to axelaberg91.
- [v1.5.0](https://github.com/axelaberg91/ClockSync-Evolved/tree/v1.5.0) preserves the v1.5.0 firmware snapshot. The default `master` branch contains the current fork code, with the original Git history retained.
- Local Wi-Fi credentials (`secrets.h` or `secret.h`) and compiled firmware files are excluded from version control. Use `secrets_example.h` as a template or configure Wi-Fi through the setup portal.

## Building from the command line

The target is **M5Stack 3.3.10 / M5Atom / Huge APP**, with 4 MB flash. Run these commands from the parent of the `clocksync` sketch directory, replacing `COM_PORT` with the device port:

```bash
arduino-cli core update-index --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
arduino-cli core install m5stack:esp32@3.3.10 --additional-urls https://static-cdn.m5stack.com/resource/arduino/package_m5stack_index.json
arduino-cli compile --fqbn m5stack:esp32:m5stack_atom:PartitionScheme=huge_app clocksync
arduino-cli upload -p COM_PORT --fqbn m5stack:esp32:m5stack_atom:PartitionScheme=huge_app clocksync
```

The older Espressif 2.0.17 build is a historical compatibility result. Current instructions target M5Stack 3.3.10. No separate M5Atom or FastLED libraries are needed.

## Defaults and time-zone presets

Normal configuration uses the web interface. To change the compiled default signal, replace `SN_DEFAULT` in `clocksync.ino`, for example `#define SN_DEFAULT (SN_JJY_E)`. A saved station takes precedence.

`time_zones.h` contains the accepted preset IDs, labels and POSIX rules. Preferences stores the ID, so adding or reordering presets does not reinterpret existing selections. `DEFAULT_TIME_ZONE` identifies the fallback preset. New installations and unknown stored IDs use Central Europe. Changes to the preset catalog require a firmware upload; selecting an existing preset does not.

## Verification record and limits

The earlier v1.4.0 credential variants were compiled successfully with **M5Stack 3.3.10 / M5Atom / Huge APP** on 2026-10-17: with `secrets.h` and from a separate source copy without it. Compiler dependencies confirm that the latter does not include `secrets.h`. Those builds used approximately **1.08 MB program storage and 53,520 bytes of static RAM**. The package produced pin-macro redefinition warnings from its own Atom variant header, with no compilation errors. No vendor files were modified to suppress those warnings. These figures do not measure v1.5.0 or v1.6.0.

Host-side tests in `tests/logic_test.cpp` cover 13 DCF77 calendar/DST cases, all three parity fields, the exact 23-hour boundary, button gestures/debounce and RGB status priority. Earlier development runs passed these cases. They use fixed local-time fixtures and do not validate ESP32 libc timezone behavior or electrical output. The Wi-Fi form was checked in a browser with simulated responses, including successful submission and a 390-pixel layout without horizontal overflow.

A physical Atom Lite, antenna, receiver and frequency/timing measurements are still required to verify the emitted signal and operation under Wi-Fi/web load. This fork does not generate DCF77 leap-second announcements or leap-second frames. Other station encoders are retained from upstream and have not received the same review as DCF77.

The AP start/10-minute fallback, real Wi-Fi authentication, NVS persistence across power loss and AP shutdown must also be checked on a physical Atom. A successful build and simulated web responses do not verify those radio/network behaviors.

The v1.5.0 English interface, deferred Wi-Fi changes and explicit Wi-Fi reset have not been rebuilt or exercised on hardware. In particular, confirm that the old network stays active after saving, that the new network is selected after restart, and that forgetting Wi-Fi opens the setup AP even when the firmware contains compiled credentials.

The v1.6.0 web time-zone changes have not been compiled or tested on hardware. The pause acknowledgement, rapid successive changes, persistence, DST transitions, unchanged holdover limit, and SNTP reconnect behavior still require verification on an Atom Lite.

## Board comparison

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
