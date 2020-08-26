#include "errors.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>


struct sk_bfifo {
	uint8_t *buf;
	uint32_t buflen;
	uint32_t rdidx;
	uint32_t wridx;
	uint8_t isfull : 1;
};

typedef struct sk_bfifo sk_bfifo_t;

sk_err sk_bfifo_init(sk_bfifo_t *fifo, uint8_t *buf, uint32_t buflen);
uint32_t sk_bfifo_cur_length(sk_bfifo_t *fifo);
sk_err sk_bfifo_put(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len);
sk_err sk_bfifo_get(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len);
void _printfifo(sk_bfifo_t *fifo);
