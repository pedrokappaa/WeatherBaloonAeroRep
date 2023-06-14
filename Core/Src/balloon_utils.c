#include "balloon_utils.h"


 float convert_adc_to_physical_value(int adc_read, int adc_read_a, float point_a, int adc_read_b, float point_b)
{
	/* convert_adc_to_physical_value: Convert a digital value read from an ADC to it's correspondent physical value.
	 * This function assumes two points in a lane. mapping (adc_read_a,point_a) and (adc_read_b,point_b).
	 * and will determine the value (adc_read,point_c) between point_a and point_b
	 * NOTE: point_b > point_c > point_a
	 *
	 * parameters list:
	 * 			(IN) adc_read: 	digital value read from ADC (12 bits value): ranges from 0 to 4096
	 * 			(IN) adc_read_a: digital read of the known point A in the final scale
	 * 			(IN) point_a:	float in the final scale for the read adc_read_a
	 * 			(IN) adc_read_b:	digital read of the known point A in the final scale
	 * 			(IN) point_b:	float in the final scale for the read adc_read_b
	 *
	 * 			(OUT)point_c:	converted value!
	 */

	 float point_c=0;
	 float delta_y=0,m=0,n=0;
	 int delta_x=0;

	 delta_y = (point_b) - (point_a);
	 delta_x = adc_read_b - adc_read_b;

	 m = delta_y/delta_x;
	 n = point_b - m*adc_read_a;

	 point_c = m*adc_read+n;

	 return point_c;
}

bool set_power_source(int source)
{
	switch(source)
	{
	case STM:
		// SW_STM = 1;
		return true;
		break;
	case LORA:
		// SW_LORA = 1;
		return true;
		break;
	case STRATO_MAIN:
		// SW_STRATO_MAIN = 1;
		return true;
		break;
	case STRATO_BCKP:
		// SW_STRATO_BCKP = 1;
		return true;
		break;
	default:
		return false;
		break;
	}
}

bool clear_power_source(int source)
{
	switch(source)
	{
	case STM:
		// SW_STM = 0;
		return true;
		break;
	case LORA:
		// SW_LORA = 0;
		return true;
		break;
	case STRATO_MAIN:
		// SW_STRATO_MAIN = 0;
		return true;
		break;
	case STRATO_BCKP:
		// SW_STRATO_BCKP = 0;
		return true;
		break;
	default:
		return false;
		break;
	}
}
