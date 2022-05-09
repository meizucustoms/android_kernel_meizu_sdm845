CC = $(CROSS_COMPILE)gcc
CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -I../../../../include/uapi/
CFLAGS += -I../../../../include/
CFLAGS += -I../../../../usr/include/

TEST_PROGS := memfd_test

all: $(TEST_PROGS)

include ../lib.mk

build_fuse: fuse_mnt fuse_test

fuse_mnt.o: CFLAGS += -I/usr/include/fuse -D_FILE_OFFSET_BITS=64
fuse_mnt: LDFLAGS += -lfuse -pthread

run_fuse: build_fuse
	@./run_fuse_test.sh || echo "fuse_test: [FAIL]"

clean:
	$(RM) memfd_test fuse_test
