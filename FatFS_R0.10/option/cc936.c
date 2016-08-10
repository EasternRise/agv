/*------------------------------------------------------------------------*/
/* Unicode - OEM code bidirectional converter  (C)ChaN, 2009              */
/*                                                                        */
/* CP936 (Simplified Chinese GBK)                                         */
/*------------------------------------------------------------------------*/

#include "../ff.h"


#if !_USE_LFN || _CODE_PAGE != 936
#error This file is not needed in current configuration. Remove from the project.
#endif

WCHAR ff_convert ( /* Converted code, 0 means conversion error */
WCHAR src, /* Character code to be converted */
UINT dir /* 0: Unicode to OEMCP, 1: OEMCP to Unicode */
)
{
  return src;
}



WCHAR ff_wtoupper (	/* Upper converted character */
	WCHAR chr		/* Input character */
)
{
	return chr;
}
