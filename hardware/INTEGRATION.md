# Integration with the firmware documentation

This change is confined to `hardware/` so firmware work can continue independently. It does not edit the root README, firmware sources, configuration examples or software release notes.

When the finalized firmware documentation is integrated, link to [the hardware entry point](README.md) from the root README and describe this as Axel's build variant based on tanvach/clocksync and tarohs/nisejjy. A suitable introduction is:

> This fork builds on tanvach/clocksync, itself based on SASAKI Taroh's nisejjy. It adds a compact Atom Lite coil build with printable mounting parts and an assembly guide. See hardware/README.md for the tested build, downloads and validation record. Firmware changes are described separately in the software release notes.

Use real Markdown links in the final root README. Keep inherited attribution and the Git history. Link the full [credit record](CREDITS.md), and distinguish firmware changes from hardware additions in release notes.

## Pin correction to carry into the main guide

At the inspected baseline the root README says GPIO32 is on the bottom header. For the original Atom Lite used here, connect G32 through the Grove port below USB-C. GND and G32 are the outer positions; 5V and G26 are unused. Verify against the [official pin map](https://docs.m5stack.com/en/core/Atom-Lite). The hardware guide already uses this connection.

## Release structure

Keep current, individually viewable STL/3MF/CAD files in `hardware/`; offer a complete ZIP as the convenient download. After the separate firmware work is finalized, create a release identifying its software commit and this hardware revision, attach the matching print ZIP, summarize the build and link the guide. The hardware has been printed, fitted and functionally tested by Axel with 20 turns and a 330 ohm series resistor.

Add real assembly photos after the physical build; current preview images show CAD and illustrative electronics. Maintain `VALIDATION.md` as observations arrive. Resolve the recorded license status separately without replacing upstream notices or declaring inherited code to be newly licensed.
