# Changelog

Release notes for ClockSync Evolved. The current user guide is in [README.md](README.md).

## v1.6.0

- A **Time zone** selector in Radio settings saves a preset across restarts. Signal and zone remain independent choices.
- A time-zone change waits for the radio task to stop before changing libc's local-time rules. Transmission restarts at the next whole minute when eligible; a manual pause stays paused and the 23-hour guard is unchanged.
- NTP setup no longer rewrites the time zone during reconnects or sync requests. The status page shows the active zone, with selected/active IDs and pending/error details in the API and technical status.
- The README explains changing signals and time zones, including WWVB's UTC frame and receiver DST settings.

## v1.5.0

- The status interface, setup portal and device messages use English. The local clock keeps Central European time and now retains fractional seconds when refreshing its display.
- The normal web interface has **Wi-Fi settings**. **Save for next startup** stores a new SSID/password without changing the current connection or its reconnect settings. Credentials are first tried after restarting the device.
- **Forget Wi-Fi** clears the saved network after confirmation and makes the next startup open the setup AP. The reset also overrides compiled `secrets.h` credentials until a new network is saved; the current connection stays active until restart.
- Current and next-startup networks are shown separately. Saved passwords are never returned to the browser. The healthy-connection footer text has been removed.
- Status LEDs use distinct green, yellow and red channel values at low brightness.

## v1.4.0

- DCF77 uses a 77,500 Hz carrier request with 8-bit LEDC PWM. Technical status shows the actual configured frequency, which may differ slightly because of the hardware divider.
- The DCF77 frame carries the following minute, including date rollover, Sunday = 7, DST flags and the A1 change announcement.
- Transmission requires a confirmed SNTP reply after every boot. It stops after 23 hours without a new reply; Wi-Fi connectivity alone does not reset this limit.
- Wi-Fi reconnect attempts continue every 60 seconds. Network operations and HTTP run separately from the radio task.
- Short button press pauses/resumes the requested transmission state. Hold for 3 seconds to request time sync; release does not also toggle transmission.
- Status LED: green for normal transmission, yellow while waiting for NTP/frame alignment or after 3 h 15 min without a confirmed sync, red for disconnected Wi-Fi, blinking red for expired time or a radio error. A user pause or disabling the LED turns it off.
- A compact Swedish web interface shows transmitter status, connection, last confirmed sync, LED state and time remaining, with mobile layout and advanced settings.
- Built-in Wi-Fi provisioning: AP SSID/password `clocksync`, immediate when unconfigured or after 10 minutes of continuous Wi-Fi loss. A successful setup is stored in NVS and overrides compiled credentials.
- Target build: M5Stack board package 3.3.10, board M5Atom. RGB and debounced button control use the ESP32 core directly; no external M5Atom/FastLED libraries are required.

## v1.3.1

- **Prevent NVS flash wear**: Added `WiFi.persistent(false)` to stop the WiFi library from writing credentials to flash on every connect/disconnect cycle.
- **Buzzer default off**: Buzzer now defaults to off on fresh install and reset.
- **Tristate radio pin when TX off**: Radio output pin is now set to high-impedance (INPUT) when TX is disabled or when switching pins, instead of being driven LOW.

## v1.3

- **Fix WWVB DST bits**: Changed default TZ from `"UTC0"` to `"PST8PDT,M3.2.0,M11.1.0"`. With `"UTC0"`, DST status bits were always 0, causing clocks to stay on standard time after DST started. Updated documentation and status display to clarify that WWVB DST bits are derived from the TZ setting.
- **Remove dead `q` command**: Removed unused `q0/q1/q2` WWVB pending override command and related dead code (`wwvbPendingOverride`, `getWWVBPendFlag`).

Original fork release notes by axelaberg91 follow [LICENSE.md](LICENSE.md). Inherited upstream release notes are preserved; their license status remains unresolved.
