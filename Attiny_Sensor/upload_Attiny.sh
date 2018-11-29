avrdude -c USBasp-clone \
        -p t85 \
        -U flash:w:$1 

# 8 MHz / 8 
#        -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  \

# 8 MHz
#        -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  \
