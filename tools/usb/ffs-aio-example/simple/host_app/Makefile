CC = gcc
LIBUSB_CFLAGS = -I/usr/include/libusb-1.0
LIBUSB_LIBS = -lusb-1.0
WARNINGS = -Wall -Wextra
CFLAGS = $(LIBUSB_CFLAGS) $(WARNINGS)
LDFLAGS = $(LIBUSB_LIBS)

all: test
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) test
