# Hardware changes

## Initial hardware build

This package contains the existing coil **design v6** and complete-housing connector **design v2**. These are CAD iteration numbers, independent of the firmware version. No earlier CAD iterations are required to build this variant.

- 75 mm overall coil former diameter and 15 mm height.
- Integrated Atom Lite snap holder, offset toward one inside edge to leave room for USB-C and Grove leads.
- Eight radial supports and one inward-facing shared wire exit.
- Side-mounted 0.25 W resistor clip with 1.2 mm walls and room for heat-shrink tubing.
- Tested build: 20 turns of 0.5 mm enamelled wire and a 330 ohm series resistor. Suggested winding arrangement: 15 + 5; geometric capacity estimate: 30 turns in two layers.
- Rear connector guide for two complete Dupont female housings, leaving the insertion ends free.
- Separate STL, geometry-only 3MF and editable OpenSCAD files; combined print plate and ZIP package.

Supersedes the separate glue-on Atom holder and the early bare-contact adapter concept. The complete-housing rear guide does not use separate dummy plugs on the middle Grove pins.

Axel has printed, fitted and functionally tested the build. See the [validation record](VALIDATION.md).
