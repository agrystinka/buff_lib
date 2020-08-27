#include "acount.h"
#include "errors.h"
#include <stdint.h>
#include <stdio.h>

bl_err printarr(uint8_t *barr, uint32_t len)
{
  if(NULL == barr || len <= 0)
    return BL_EWRONGARG;

  for(int i = 0; i < len; i++)
    printf("%d, ", (unsigned int)barr[i]);
  printf("\n");

  return BL_EOK;
}

int main(void)
{
	bl_bfifo_t tb = {0};
	uint8_t buf[16] = {0};
	printf("Init %d\n", (unsigned int)bl_bfifo_init(&tb, &buf[0], sizeof(buf)));
	_printfifo(&tb);
  printf("Length %d\n", bl_bfifo_cur_length(&tb));
  printarr(tb.buf, tb.buflen);

  uint8_t buf_add[4] = {1, 5, 6, 9};
  bl_err tmp = bl_bfifo_put(&tb, &buf_add[0], 4);
  if(BL_EOK != tmp)
    bl_catch_err(tmp);
  else{
    printf("Fine 1\n");
    _printfifo(&tb);
    printf("Length %d\n", bl_bfifo_cur_length(&tb));
    printarr(tb.buf, tb.buflen);
  }

  uint8_t buf_add2[16] = {0};
  tmp = bl_bfifo_put(&tb, buf_add2, sizeof(buf_add2));
  if(BL_EOK != tmp)
    bl_catch_err(tmp);
  else{
    printf("Fine 2\n");
    _printfifo(&tb);
    printf("Length %d\n", bl_bfifo_cur_length(&tb));
    printarr(tb.buf, tb.buflen);
  }

  uint8_t buff_read[3] = {0};
  tmp = bl_bfifo_get(&tb, buff_read, sizeof(buff_read));
  if(BL_EOK != tmp)
    bl_catch_err(tmp);
  else{
    printf("Fine 3\n");
    _printfifo(&tb);
    printf("Length %d\n", bl_bfifo_cur_length(&tb));
    printarr(buff_read, sizeof(buff_read));
  }

	return 0;
}
