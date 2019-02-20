Version update, Feb 2019
Henrik Mauroy
hmauroy@gmail.com
hmauroy.github.io

1) Included Serial handling by Nick Gammaond:
 http://www.gammon.com.au/serial
2) Added control of lower vane on floor models like Mitsubishi MFZ-series. Byte 16 controls this.
3) Input over serial interface: Data format 'A'[mode][temp][fanSpeed][upperVane][lowerVane][inOff] 
   e.g. A1232001 (A,Heat,23deg,speed 2, upperVane auto, lowerVane closed, ON)
4) Changed how settings are handled to make it easier for inexperienced arduino coders to 
   use arrays containing only numbers for doing the varoious controls.