#include "acount.h"

static inline bool _sk_bfifo_is_full(sk_bfifo_t *fifo)
{
	return (1 == fifo->isfull);
}

static inline bool _sk_bfifo_is_empty(sk_bfifo_t *fifo)
{
	return (!_sk_bfifo_is_full(fifo)) && (fifo->rdidx == fifo->wridx);
}

sk_err sk_bfifo_init(sk_bfifo_t *fifo, uint8_t *buf, uint32_t buflen)
{
	if ((NULL == fifo) || (NULL == buf) || (0 == buflen))
		return SK_EWRONGARG;

	*fifo = (sk_bfifo_t) {
		.buf = buf,
		.buflen = buflen,
		.rdidx = 0,
		.wridx = 0,
		.isfull = false
	};
	return SK_EOK;
}

// empty slots = buflen - X, where X is number of elements currently in buffer
// when wridx < rdidx    X = wridx - rdidx
// when wridx > rdidx    X = buflen - (rdidx - wridx) = buflen + wridx - rdidx
uint32_t sk_bfifo_cur_length(sk_bfifo_t *fifo)
{
  if (NULL == fifo)
		return SK_EUNKNOWN;

  // if(fifo->rdidx < 0 || fifo->wridx < 0)
  //   return SK_EWRONGARG;

  if(fifo->wridx < fifo->rdidx)
    return fifo->buflen - (fifo->rdidx - fifo->wridx);
  else if(fifo->wridx > fifo->rdidx)
    return  fifo->wridx - fifo->rdidx;
  else if(_sk_bfifo_is_full(fifo))
    return  fifo->buflen;
  else
    return 0;
}

static sk_err _sk_bfifo_write(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len)
{
  if ((NULL == fifo) || (NULL == barr) || (0 == len))
		return SK_EWRONGARG;

  for(uint32_t i = 0; i < len; i++)
    fifo->buf[(fifo->wridx + i) / fifo->buflen] = barr[i];

  fifo->wridx = (fifo->wridx + len) / fifo->buflen;

  if(fifo->wridx == fifo->rdidx)
    fifo->isfull = true;

  return SK_EOK;
}

sk_err sk_bfifo_put(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len)
{
  if ((NULL == fifo) || (NULL == barr) || (0 == len))
		return SK_EWRONGARG;

	// if (len > fifo->buflen)
	// 	return SK_ERANGE;

  uint32_t curlenght = sk_bfifo_cur_length(fifo);
  if(curlenght < 0)
    return curlenght;

  if (_sk_bfifo_is_full(fifo) || len + curlenght > fifo->buflen)
		return SK_EFULL;

  if(SK_EOK != _sk_bfifo_write(fifo, barr, len))
    return SK_EUNKNOWN;

  return SK_EOK;
}

static sk_err _sk_bfifo_read(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len)
{
  if ((NULL == fifo) || (NULL == barr) || (0 == len))
		return SK_EWRONGARG;

  for(uint32_t i = 0; i < len; i++)
    barr[i] = fifo->buf[(fifo->rdidx + i) / fifo->buflen];

  fifo->rdidx = (fifo->rdidx + len) / fifo->buflen;
  fifo->isfull = false;

  return SK_EOK;
}

sk_err sk_bfifo_get(sk_bfifo_t *fifo, uint8_t *barr, uint32_t len)
{
  if ((NULL == fifo) || (NULL == barr) || (0 == len))
		return SK_EWRONGARG;

  uint32_t curlenght = sk_bfifo_cur_length(fifo);

  if(curlenght < 0)
    return curlenght;

  if(curlenght == 0)
    return SK_EEMPTY;

  if(curlenght < len)
    return SK_ERANGE;

  if(SK_EOK != _sk_bfifo_read(fifo, barr, len))
    return SK_EUNKNOWN;

  return SK_EOK;
}

void _printfifo(sk_bfifo_t *fifo)
{
	if (NULL != fifo) {
		fprintf(stderr, "\t> (addr %p) BUF=%p LEN=%llu RWIDX=%llu WRIDX=%llu ISFULL=%c\n",
						fifo, fifo->buf, (unsigned long long)fifo->buflen,
						(unsigned long long)fifo->rdidx, (unsigned long long)fifo->wridx,
						(fifo->isfull ? 'T' : 'F'));
	} else {
		fprintf(stderr, "\t!! NULL\n");
	}
}
