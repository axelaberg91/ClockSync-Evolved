# Credits and provenance

## Firmware ancestry

This hardware build belongs to [axelaberg91/clocksync](https://github.com/axelaberg91/clocksync), a fork of [tanvach/clocksync](https://github.com/tanvach/clocksync).

- **Tan Vachiramon / tanvach** maintains the clocksync base used by this fork, including its Atom Lite support, web interface and protocol work. See the upstream repository and its history for the full contribution record.
- **SASAKI Taroh / tarohs** created [nisejjy](https://github.com/tarohs/nisejjy), the original project underlying this fork family.
- **Henner Zeller / hzeller** created [txtempus](https://github.com/hzeller/txtempus), credited by the upstream README as a WWVB frame verification reference. Its loop examples also informed the winding starting point.

Upstream software and ideas are credited to their respective authors. This hardware addition does not claim their work as new work by this fork.

## Hardware additions in this fork

The printable package was developed for Axel's Atom Lite build through an iterative CAD design process with Codex assistance. It adds the 75 mm coil former, integrated removable Atom holder, inward lead exit, eight supports, side resistor clip and rear guide for two complete Dupont female housings. Axel supplied the initial coil-former concept and original STL, and specified the packaging and assembly requirements.

Dimensional references:

- [M5Stack Atom Lite documentation and pin map](https://docs.m5stack.com/en/core/Atom-Lite).
- [M5Stack official Atom Lite enclosure model](https://github.com/m5stack/M5_Hardware/tree/master/Products/C008_Atom-Lite/Structures), used to check fit; the reference mesh itself is not included in this package.
- [Pololu single-pin connector housing](https://www.pololu.com/product/1900) and its [dimension drawing](https://a.pololu-files.com/picture/0J1727.1200.png), used for nominal complete female housing dimensions.
- [Grove connector drawing hosted by Seeed Studio](https://statics3.seeedstudio.com/images/opl/datasheet/3470130P1.pdf), used for connector reference dimensions.

These references identify sources of dimensions; they do not imply manufacturer endorsement or certification of this adapter.

## License status

At the inspected firmware baseline (`63d92cae27f5085cbd8474151474e6ea8660a867`), this fork contains no LICENSE file. This hardware addition preserves upstream files and notices and does not assign a new license to inherited software. No separate license has yet been selected for the new CAD and guide. Attribution is recorded here; it is not a replacement for a license grant. Resolve and document licensing before advertising broad reuse or redistribution permissions.
