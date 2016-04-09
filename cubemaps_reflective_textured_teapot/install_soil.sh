git clone https://github.com/smibarber/libSOIL.git
cd libSOIL/

# For 64-bit machine, Change line 7 in libSOIL/Makefile from
# CFLAGS += -c -O2 -Wall -std=gnu99 -m64
# to:
# CFLAGS += -c -O2 -Wall -std=gnu99 -m64

make
make install
