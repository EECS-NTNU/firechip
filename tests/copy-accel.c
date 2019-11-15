#include "dma-ext.h"

#include <stdint.h>

#define COPYN 10

int16_t src[COPYN];
int16_t dst[COPYN];

int main(void)
{
	int i;

	for (i = 0; i < COPYN; i++)
		src[i] = (i << 8) | 0xfe;

	dma_write_cr(SEGMENT_SIZE, sizeof(int16_t) * COPYN);
	dma_write_cr(NSEGMENTS , 1);
	dma_transfer(dst, src);
	dma_sfence();
	asm volatile ("fence");

	if (dma_read_cr(RESP_STATUS) != NO_ERROR)
		return 1;

	for (i = 0; i < COPYN; i++) {
		if (dst[i] != src[i])
			return i + 1;
	}

	printf("Test successful\n");

	return 0;
}
