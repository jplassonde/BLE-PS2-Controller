# BLE-PS2-Controller

This is an example of a failed project. Last fall (2018), I wanted to play Old School Musical with a Playstation 2 controller with the vibration on and I almost lost my sanity in the process.

# Overview
On the PSoC side, one core get the input from the PS2 controller while the other process the BLE stack. The core communicate through an IPC pipe and a shared .bss section. 

On the PC side, a user mode driver extend the DirectInput device capabilities to include rudimentary force feedback.

# Additional informations
The initial idea was to interface the Playstation 2 controller through BLE and implement the force feedback (rumble) through the HID report, using the Physical Interface Device class definition. After a week of work I still could not get the force feedback capabilities detected properly by Windows, while the same HID report on a virtual hid device I built using MS vhidmini2 as a starting point worked perfectly. At that point I assumed it was impossible to go that way with BLE and started working on a UMDF driver instead.

Once it was done, the forcefeedback worked on a few applications I tested (FFBtester, PS2 emulator, ...) but not with Steam. All I got from the latter was "reset", "set actuator on" spammed with no other event. Fine tuning the registry entry did not help, nothing else was showing up in the log (not even an attempt to check status), and again, a similar virtual hid device behaved properly. 

There may be something different with the way Windows handle/enumerate BLE devices and causes issues here. At that point I think the only way to make this work properly would be to make a KMDF filter driver for the whole device, but I don't think Windows would let me override the input.inf with a test-signed driver (or at least I have not found a way to do so).

In the end, I used X360ce (XBOX controller emulator), butchered down the FFBDriver and filtered the X360ce vibration on/off spam to get it to work. Launched my game outside Steam, and finally got the vibration. 

At that point I had to let it go and abandoned the project. I hate to let the computer win though and I will try to implement it via USB (using Cypress FX3) as soon as I have time to allocate to this. Maybe attacking the problem from another angle will yield some explanations.

The .inf file and the class factory code could be used as starting point for a similar application, but to anyone thinking about doing this on BLE: don't.
