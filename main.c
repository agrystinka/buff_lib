#include "acount.h"
#include "errors.h"
#include <stdint.h>
#include <stdio.h>

sk_err printarr(uint8_t *barr, uint32_t len)
{
  if(NULL == barr || len <= 0)
    return SK_EWRONGARG;

  for(int i = 0; i < len; i++)
    printf("%d, ", (unsigned int)barr[i]);
  printf("\n");

  return SK_EOK;
}

int main(void)
{
	sk_bfifo_t tb = {0};
	uint8_t buf[16] = {0};
	printf("Init %d\n", (unsigned int)sk_bfifo_init(&tb, buf, sizeof(buf)));
	_printfifo(&tb);

  uint8_t buf_add[4] = {1, 5, 6, 9};
  if(SK_EOK != sk_bfifo_put(&tb, buf_add, sizeof(buf_add)))
    printf("Err\n");
  else
    printf("FINE\n");

  // uint8_t buf_add2[16] = {0};
  // if(SK_EOK != sk_bfifo_put(&tb, buf_add2, sizeof(buf_add2)))
  //   printf("Err\n");
  // else
  //   printf("FINE\n");

  uint8_t buff_read[3] = {0};
  sk_err tmp = sk_bfifo_get(&tb, buff_read, sizeof(buff_read));
  if(SK_EOK != tmp)
    sk_catch_err(tmp);
  else
    printarr(buff_read, sizeof(buff_read));
    //printf("Err\n");
  printarr(tb.buf, tb.buflen);

	return 0;
}
