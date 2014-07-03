#!/bin/bash
mspdebug rf2500 "gdb" &
sleep 3s
msp430-gdb
