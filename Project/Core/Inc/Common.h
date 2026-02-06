#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include "stdbool.h"
#include "strings.h"

typedef enum {
	EVT_NONE=0,
	EVT_SINGLE_CLICK ,
	EVT_DOUBLE_CLICK ,
	EVT_HOLD_START,
	EVT_HOLD_END ,
	 EVT_FACET_CW,
	    EVT_FACET_CCW,
	    EVT_GRAIN_CW,
	    EVT_GRAIN_CCW

} EVENT_CODES_ENUM;


#endif /* INC_COMMON_H_ */
