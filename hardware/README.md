# Atom Lite print pack

**License:** original designs and assets by **axelaberg91** are **CC BY-NC 4.0**. Non-commercial sharing and adaptation are allowed with attribution; commercial use requires separate permission. The rights holder may sell their own designs and prints. See [LICENSE.md](LICENSE.md) and the [full license](CC-BY-NC-4.0.txt).

## Build the Atom Lite unit

**M5Stack Atom Lite · 20 turns of 0.5 mm enamelled copper wire · 330 ohm series resistor.**

![Atom placement and series wiring diagram](images/coil-preview.png)

### Download and print

Open `print/print-both.3mf` in your slicer. Both parts, editable CAD and these build instructions are included.

Individual files: [coil STL](coil/coil-75mm-integrated-holder.stl) · [connector clip STL](connector/dupont-housing-clip.stl) · [both parts as 3MF](print/print-both.3mf).

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

## Credits

Hardware developed for Axel's build with Codex assistance. Software: [tanvach/clocksync](https://github.com/tanvach/clocksync), based on [SASAKI Taroh's nisejjy](https://github.com/tarohs/nisejjy); upstream reference: [txtempus](https://github.com/hzeller/txtempus).

For firmware setup and the full project, see [the project homepage](https://github.com/axelaberg91/ClockSync-Evolved). CAD sources and geometry reports are included in `coil/` and `connector/`.
