<h1 align="center">RE:Neon II</h1>

<div style="display: flex; justify-content: flex-end; align-items: center;">
    <img src="images/evolink.png" alt="Logo" style="height: 2em;margin-right:1.5em">
    <span style="vertical-align:middle">ELP-03 FROM OXTRE..</span>
</div>

## Overview

A amazing smart watch with in-16(Nixie tube).

<p align="center">
 <img border="1px" width="80%" src="./images/main.jpg" alt="main.jpg">
</p>

<p align="center">
 <img border="1px" width="40%" src="./images/show1.jpg" alt="show1.jpg">
 <img border="1px" width="40%" src="./images/show2.jpg" alt="show2.jpg">
</p>

## Nixie tube

Nixie tubes are vintage gas-discharge display devices that produce a mesmerizing orange-red glow. They were widely used in early digital instruments and clocks, and today, they’re prized for their retro aesthetic.

Nixie tubes are a beautiful marriage of physics and engineering. While obsolete for practical displays, their nostalgic charm ensures they’ll keep glowing for decades to come.

They glow through gas discharge when high voltage (~170V) is applied.

<p align="center">
 <img border="1px" width="80%" src="./images/tubes.jpg" alt="tubes.jpg">
</p>

<p align="center">
 <img border="1px" width="80%" src="./images/other tubes.jpg" alt="tubes.jpg">
</p>

After a lot of selection, I finally chose to use the `IN-16` for this project.

The beautiful glow in the dark:

<p align="center">
 <img border="1px" width="80%" src="./images/glow_in_the_dark.gif" alt="glow">
</p>

## Features

1. DISPLAY : IN-16 x2 (obviously)
2. RGB : ws2812b x2 + SEFUSE 5x20mm
3. POWER : TP4056D + TPS78233
4. MCU : esp32-Pico-D4
5. RTC : ds3231mz+
6. ACCE : LIS331DLHTR
7. key : 6x6x5mm botton
8. BAT : 502035 500ma

<p align="center">
 <img border="1px" width="80%" src="./images/shell.png" alt="shell.png">
</p>

<p align="center">
 <img border="1px" width="80%" src="./images/render.jpg" alt="render.jpg">
</p>

Carefully install the LED tube, which is done by soldering the wire on the WS2812 (LED), pasting the LED and the light guide post and then plugging it into the fuse housing.

<p align="center">
 <img border="1px" width="40%" src="./images/led_struct.jpg" alt="ledstruct">
</p>

This is the complete PCB board (does not include the tube base)

<p align="center">
 <img border="1px" width="40%" src="./images/real_pcb.jpg" alt="real_pcb.jpg">
</p>

## Program

Serial debugging and programming of ESP32 using `typec` interface (built-in CP2102)

## Use

Press the button or shake your arm to display the time

## Whats New

It's clear that we've made significant improvements in size and integration

Here is the actual circuit board of version 1.0:

<p align="center">
 <img border="1px" width="80%" src="./images/old_v1.0_img1.jpg" alt="old1">
</p>

<p align="center">
 <img border="1px" width="80%" src="./images/old_v1.0_img2.jpg" alt="old2">
</p>

## And More?

<p align="center">
 <img border="1px" width="30%" src="./images/pcb.png" alt="pcb.png">
 <img border="1px" width="30%" src="./images/pcb2.png" alt="pcb2.png">
 <img border="1px" width="30%" src="./images/pcb3.png" alt="pcb3.png">
</p>
