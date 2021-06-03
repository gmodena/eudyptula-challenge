#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define SYS_EUDYPTULA 447

int main(int argc, char *argv[])
{
	assert(syscall(SYS_EUDYPTULA, 0x0, 0x01) != 0);
	assert(syscall(SYS_EUDYPTULA, 0xc140, 0x882fdd04) == 0);

	return 0;
}
