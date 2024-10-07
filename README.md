# UNI-T Multimeter Digital Interface

## Optical plug-in module for the UT-61e (and similar) series of Multimeter.

![DMM](images/PXL_20241006_233730907.jpg)

This is a wireless (wifi, web/REST) interface that allows remote reading of the current multimeter value, including its mode.

Optionally, it can also interface with a wireless footswitch (pedal).  This can trigger a network message being sent (mqtt, etc) as well as an optional trigger sent to a speech synthesizer.  Both of those projects are also Arduino/esp32 based.

Parts needed are simple:

![DMM_parts](images/PXL_20241006_221243451.jpg)

- photo diode (https://www.amazon.com/gp/product/B07VNSX74J)
- esp32c3 mini (https://www.amazon.com/DWEII-ESP32-C3-Development-Bluetooth-Frequency/dp/B0CW62PPZ1)
- DC barrel jack, 5.5 x 2.1mm (https://www.amazon.com/gp/product/B07SV6KCT5)
- 200 ohm 1/8 watt resistor (power rating not imporant)
- (2) m2 x 6mm screws and nuts
- i2c oled display (https://www.amazon.com/gp/product/B0925SSKHB)
  

![esp32_view_1](images/PXL_20241006_223013554.jpg)

![box_inside_1](/images/PXL_20241006_231246798.jpg)

![box_outside_1](/images/PXL_20241006_234812085.jpg)

![box_outside_2](images/PXL_20241006_234911514.jpg)
