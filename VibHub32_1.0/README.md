# VibHub ESP32 Board 2.x

## 2.0a (Shipped 2019)

ESP32 based dev boards shipped in 2019
Replaced by 2.1

## 2.1 rev.A (Unreleased)

Complete board redesign

Add: Lower BOM cost
Changed: Smaller board size
Changed: Moved all buttons to top side of board
Changed: Removed duplicate vibe ports, replaced with combined footprint, can have 4 ports of any combination of 2.5mm and USB
Removed: Extra debug headers and LEDs

Bug: Missing trace that prevents USB-UART chip from powering


## 2.1 rev.B (Unreleased)

Unreleased
50% board redesign due to new battery holder to replace previous that became unavailable

Add: Second RGB LED
Add: More mounting holes
Add: True auto power off while on battery
Add: Side firing LEDs on driver outputs (Only works if drivers are in "forward")
Changed: Replaced electrolytic cap with tantalum to reduce component side height clearance

Bug: High leakage current through TX line while running on battery


## 2.1 rev.C (In Production)

First batch of 30 dev boards currently in production as of 2020-03-04, should be shipping by mid March

Bug: Designator and pin 1 marking missing on D1


## 2.1 rev.C2 (Design In Progress)

Tentative revision for next batch
Identical to rev.C with only silkscreen modifications


## 2.1 rev.D (Planning)

Future TODO stuff

TODO: Replace DRV8833 footprint with QFN16 after supply of HTSSOP16 parts has been used. The HTSSOP16/QFN16 combo footprint works but is more difficult to reflow than needed.