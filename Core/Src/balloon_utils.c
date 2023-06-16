#include "balloon_utils.h"

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
