#µSoundburst - Color Organ for your Arduino

---

Make anything- your robot or your whole room- light up with the color or music! This is a 'Color Organ' where instead of playing music with sound it plays color shapes and patterns.

I built mine using an Arduino Atmega328 (a TinyCircuits's [TinyLily](http://tiny-circuits.com/shop/6458/) board). It uses the onboard 10bit ADC to sample audio signals then uses FHT (magnitude only FFT) from [OpenMusicLabs](http://wiki.openmusiclabs.com/wiki/ArduinoFHT). I perform beat and pattern detection on each frequency bin then create and modulate LED display patterns based on the results!

###Algorithm Explanation
TODO!

###Build your own
1. Create an audio interface for your input like [this](http://www.instructables.com/files/deriv/FMP/7YUO/HAQ2TJOE/FMP7YUOHAQ2TJOE.LARGE.jpg).
2. Connect up your leds. I use WS2812 strips like these from [Adafruit](http://www.adafruit.com/products/1376) or [Sparkfun](https://www.sparkfun.com/products/12027).
3. add magic
4. Done!

No seriously, I'm not done with documentation. If you'd like a better howto let me know!