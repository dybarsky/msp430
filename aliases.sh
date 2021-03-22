#!/bin/bash
alias msp-clean='pio run -t clean'
alias msp-build='pio run'
alias msp-flash='mspdebug rf2500 "prog .pio/build/lpmsp430g2553/firmware.elf"' 
