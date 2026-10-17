# Validation record

Axel confirms that the parts have been **printed, fitted and functionally tested**. This record combines the builder's physical test report with the earlier digital design checks; it does not claim an independent laboratory test.

## Working build

| Item | Configuration |
| --- | --- |
| Board | M5Stack Atom Lite |
| Coil former | 75 x 75 x 15 mm, integrated holder, design v6 |
| Connector guide | Complete Dupont female housing rear clip, design v2 |
| Wire | 0.5 mm enamelled copper wire |
| Turns | 20, confirmed by Axel |
| Series resistor | 330 ohm, confirmed by Axel |
| Intended signal | DCF77, 77.5 kHz, output on GPIO32 |
| Physical result | Printed parts, fit and operation confirmed by Axel |

The suggested layer arrangement is 15 + 5. The physical test report does not record the actual layer arrangement, printer settings, receiver model, measured range, test date or exact firmware commit. Firmware is being finalized separately; add its commit when the combined release is prepared. The recommended printer context for reproducing this build is Prusa CORE One with PLA.

## Digital checks

| Check | Result |
| --- | --- |
| Coil size | 75 x 75 x 15 mm |
| Closed connected meshes | One solid per part; coil 7,038 triangles, connector clip 172 triangles |
| Integrated holder union | About 454 mm3 shared material volume |
| Atom clearance | Compared with the official enclosure model; snap arm movement zones checked |
| Resistor position | Side clip with 1.2 mm walls; clear 20 mm centre cable corridor |
| Slicing | PrusaSlicer 2.9.6, 0.4 mm nozzle; combined plate at 0.12 mm layers, 4 perimeters, 20% gyroid, no support or stability warnings |
| Connector compatibility | Axel's complete females seat fully in the Grove port; housings from other suppliers may vary |

Detailed geometry reports: [coil](coil/geometry-checks.json), [connector](connector/geometry-checks.json). STL geometry is unchanged from the delivered parts. Individual 3MF files retain the same geometry with English names and metadata; the combined plate contains both parts.

The 30-turn capacity and alternative 220 ohm resistor are design or upstream options, not the reported tested configuration. No measured range or long-term durability result is asserted. Add assembly photos and further observations when available.
