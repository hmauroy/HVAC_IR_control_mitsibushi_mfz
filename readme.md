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
   
   
 * Mitsubishi HVAC protocol added by Vincent Cruvellier.
 *  delaymicrosecond() modified to avoid limit.
 * Version update Dec, 2014
 * Panasonic HVAC protocol added by Mathieu Thomas. 
 * Version update Dec, 2015
 * Mitsubishi HVAC protocol enhancement by Vincent Cruvellier.
 *   added    void sendHvacMitsubishiFD(
      HvacMode                  HVAC_Mode,           // Example HVAC_HOT  HvacMitsubishiMode
      int                       HVAC_Temp,           // Example 21  (°c)
      HvacFanMode               HVAC_FanMode,        // Example FAN_SPEED_AUTO  HvacMitsubishiFanMode
      HvacVanneMode             HVAC_VanneMode,      // Example VANNE_AUTO_MOVE  HvacMitsubishiVanneMode
      HvacAreaMode              HVAC_AreaMode,       // Example AREA_AUTO
      HvacWideVanneMode         HVAC_WideMode,       // Example WIDE_MIDDLE
      int                       HVAC_PLASMA,          // Example true to Turn ON PLASMA Function
      int                       HVAC_CLEAN_MODE,      // Example false 
      int                       HVAC_ISEE,            // Example False
      int                       OFF                   // Example false to Turn ON HVAC / true to request to turn off
    );
 * sendHvacMitsubishiFD Not tested, i don't have this material -- if someone can confirm then please push a note.
 * 