Some reverse engineering of the BG7TBL 2017-11-26 tracking generator.

The communication between the ATmega8 and the PLL (ADF4350) was monitored using a DS logic plus and pulseview (see the captured traces in Switch-on-seq.sr).

The hex files for the ATmega were recovered from the board using the following commands:

sudo avrdude -p m8 -c avrispmkII -U flash:r:flash.hex:i
sudo avrdude -p m8 -c avrispmkII -U lfuse:r:lfusefile.hex:i
sudo avrdude -p m8 -c avrispmkII -U hfuse:r:hfusefile.hex:i
sudo avrdude -p m8 -c avrispmkII -U eeprom:r:eeprom.hex:i
