# Atom Lite DCF77 build

This is Axel's compact, printable hardware build for the [clocksync fork](https://github.com/axelaberg91/clocksync). It adds a 75 mm antenna former with an integrated Atom Lite snap holder and a separate clip for two complete Dupont female connectors.

**Built and tested by Axel:** the parts have been printed, fitted and functionally tested. The working build uses **20 turns of 0.5 mm enamelled copper wire and a 330 ohm series resistor**. See the [validation record](VALIDATION.md).

![Antenna former and integrated holder](images/coil-preview.png)

## Start here

1. Read the [parts, wiring and assembly guide](BUILD.md).
2. Download [the complete print pack](print/clocksync-atom-lite-print-pack.zip), extract it, and open `print-both.3mf` in your slicer.
3. Select your own printer and PLA profile, keep 100% scale, and check the preview before printing.
4. Follow the firmware instructions in the repository's [main README](../README.md). Firmware work is maintained separately; this hardware addition does not change firmware or its release number.

The combined 3MF contains two separate models in millimetres and their placement. It does not contain a printer profile or ready-to-run G-code.

## Files

| File | Purpose |
| --- | --- |
| [Print pack ZIP](print/clocksync-atom-lite-print-pack.zip) | Both parts, editable sources, assembly guide and checksums |
| [Combined 3MF](print/print-both.3mf) | Both parts placed separately for slicing |
| [Coil STL](coil/coil-75mm-integrated-holder.stl) / [3MF](coil/coil-75mm-integrated-holder.3mf) | One part: coil former, Atom holder and resistor clip |
| [Connector clip STL](connector/dupont-housing-clip.stl) / [3MF](connector/dupont-housing-clip.3mf) | Rear guide for two complete one-pin Dupont females |
| [Coil OpenSCAD](coil/coil-75mm-integrated-holder.scad) / [clip OpenSCAD](connector/dupont-housing-clip.scad) | Editable CAD geometry |
| [Validation status](VALIDATION.md) | What was checked and what still needs testing |
| [Hardware changes](CHANGELOG.md) | Current design and superseded alternatives |
| [Credits and provenance](CREDITS.md) | Upstream authors and dimensional references |

Use the integrated holder version supplied here. A separate glue-on Atom holder, bare-contact adapters and front dummy plugs are not required for this build.

The editable CAD uses constructive geometry with some fixed coordinates; changing a top-level dimension alone may not update every related feature. Recheck fit, mesh and slicing after an edit.
