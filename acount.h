#include "errors.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>


struct bl_bfifo {
	uint8_t *buf;
	uint32_t buflen;
	uint32_t rdidx;
	uint32_t wridx;
	uint8_t isfull : 1;
};

typedef struct bl_bfifo bl_bfifo_t;

bl_err bl_bfifo_init(bl_bfifo_t *fifo, uint8_t *buf, uint32_t buflen);
uint32_t bl_bfifo_cur_length(bl_bfifo_t *fifo);
bl_err bl_bfifo_put(bl_bfifo_t *fifo, uint8_t *barr, uint32_t len);
bl_err bl_bfifo_get(bl_bfifo_t *fifo, uint8_t *barr, uint32_t len);
void _printfifo(bl_bfifo_t *fifo);
