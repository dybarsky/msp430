#!/bin/bash
hdr_dir=src/include
src_dir=src/source
bin_dir=build

rm -rf $bin_dir
mkdir $bin_dir

# compile
echo 'compile'
echo ':servo'
msp430-gcc -g -mmcu=msp430g2553 -I${hdr_dir} -c ${src_dir}/servo.c -o ${bin_dir}/servo.o
echo ':main'
msp430-gcc -g -mmcu=msp430g2553 -I${hdr_dir} -c ${src_dir}/main.c -o ${bin_dir}/main.o
echo 'link'
msp430-gcc -g -mmcu=msp430g2553 -o ${bin_dir}/main.elf ${bin_dir}/*.o
# assembler output
#msp430-objdump -DS ${bin_dir}/main.elf > ${bin_dir}/main.lst
#msp430-objcopy -O ihex ${bin_dir}/main.elf ${bin_dir}/main.hex

# old version
#msp430-gcc -mmcu=msp430g2553 ${src_dir}/main.c
