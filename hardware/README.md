# Atom Lite print pack

## Build the Atom Lite unit

**Printed, fitted and tested by Axel:** M5Stack Atom Lite, **20 turns of 0.5 mm enamelled copper wire**, and a **330 ohm series resistor**.

![Coil former with integrated Atom holder](images/coil-preview.png)

### Download and print

Open `print/print-both.3mf` in your slicer. Both parts, editable CAD and these build instructions are included.

Individual files: [coil STL](coil/coil-75mm-integrated-holder.stl) · [connector clip STL](connector/dupont-housing-clip.stl) · [both parts as 3MF](print/print-both.3mf).

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

## Credits

Hardware developed for Axel's build with Codex assistance. Software: [tanvach/clocksync](https://github.com/tanvach/clocksync), based on [SASAKI Taroh's nisejjy](https://github.com/tarohs/nisejjy); upstream reference: [txtempus](https://github.com/hzeller/txtempus).

For firmware setup and the full project, see [the project homepage](https://github.com/axelaberg91/clocksync). CAD sources and geometry reports are included in `coil/` and `connector/`.
