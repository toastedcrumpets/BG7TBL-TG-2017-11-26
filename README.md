# Reverse engineering of the BG7TBL 2017-11-26 tracking generator

I bought one of these tracking generators, but tried to save $20 by
buying one for a different spectrum analyzer (MS) than I
owned. Unfortunately, this means I had the wrong IF frequency and have
to hack it into a usable state!

See [this online discussion for more info](https://groups.io/g/HP-Agilent-Keysight-equipment/topic/24815717#100886).

## Original communication from the ATmega8 to the ADF4350 PLL

The communication between the ATmega8 and the PLL (ADF4350) was
monitored using a DS logic plus and pulseview (see the captured traces
in [Switch-on-seq.sr](Switch-on-seq.sr)).

## Original hex files

The hex files for the ATmega were recovered from the board using the
following commands:

```
sudo avrdude -p m8 -c avrispmkII -U flash:r:flash.hex:i
sudo avrdude -p m8 -c avrispmkII -U lfuse:r:lfusefile.hex:i
sudo avrdude -p m8 -c avrispmkII -U hfuse:r:hfusefile.hex:i
sudo avrdude -p m8 -c avrispmkII -U eeprom:r:eeprom.hex:i
```

## Ardunio project/HACK

To make the TG work with other spectrum analyzers you have to change the IF inside the TG. The way to do this is to reprogram the ATMega8A to set up the ADF4350 to create the correct IF. I have added a arduino project in the BG7TBL subfolder that does exactly this, you should be able to modify it for other spectrum analyzers.
