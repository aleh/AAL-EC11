# EC-11 Rotary Encoder Helper Library

This is a little library that helps to work with EC-11 style of rotary encoders on Arduino. The dependancy on Arduino functions is very small, so it can be easily ported on other platforms. See `ec11.hpp` for the docs and `examples` folder for a little demo.

## What exactly is EC-11?

It's a family of devices that look like potentiometers and allow your microcontroller to receive rotary input. It works as a push button too. These are very cheap and available everywhere, but can provide very handy interface for you microcontroller project.

Here is one of mine:

![EC-11 Species Photo](./EC11-Photo.jpg)

And this is its pinout:

![EC-11 Pinout Diagram](./EC11-Pinout.png)

Two pins are for the internal push button (D and E); the other two (A and B) provide rotation signal relative to the third pin (C), which is normally connected to your power rail.

Note that rotary encoders can have more outputs and different purposes, but we assume only two outputs here and a simple case of manual control. Also note that when the encoder is rotated, then lines A and B are switched mechanically, which generates switching noise. Sometimes capacitors are added to filter it out, but for simple projects the noise can be ignored.

