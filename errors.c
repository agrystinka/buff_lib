#include "errors.h"
#include <stdint.h>
#include <stdio.h>

#define _log printf
// static inline void _log(*char str, ...)
// {
//   printf(str);
// }

void sk_catch_err(sk_err err)
{
  switch(err){
    case SK_ENOTIMPLEMENTED:
      _log("Some feature is not implemented yet");
      break;
    case SK_EUNAVAILABLE:
      _log("Could not be done");
      break;
    case SK_EWRONGARG:
      _log("Wrong argument");
      break;
    case SK_ENENARG:
      _log("SK_ENENARG");
      break;
    case SK_ERANGE:
      _log("To much args to read or write");
      break;
    case SK_EFULL:
      _log("FIFO buffer is already empty");
      break;
    case SK_EEMPTY:
      _log("FIFO buffer is empty");
      break;
    default:
      _log("Unknown error");
    }
}
