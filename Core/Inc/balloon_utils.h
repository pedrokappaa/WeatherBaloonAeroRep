/*
 * balloon_utils.h
 *
 *  Created on: 17/05/2023
 *      Author: António Costa, João Gomes, Pedro Andrade @ Aero UMinho
 */

/******** STM32 GPIO PINOUT *********

     CN8					 CN7
1  [ ] [ ]  2			1  [ ] [ ]  2
3  [ ] [ ]  4			3  [ ] [ ]  4
5  [ ] [ ]  6			5  [ ] [ ]  6
7  [ ] [ ]  8			7  [ ] [ ]  8
9  [ ] [ ] 10			9  [ ] [ ] 10
11 [ ] [ ] 12			11 [ ] [ ] 12
13 [ ] [ ] 14			13 [ ] [ ] 14
15 [ ] [ ] 16			15 [ ] [ ] 16
						17 [ ] [ ] 18
						19 [ ] [ ] 20

1  [ ] [ ]  2			1  [ ] [ ]  2
3  [ ] [ ]  4			3  [ ] [ ]  4
5  [ ] [ ]  6			5  [ ] [ ]  6
7  [ ] [ ]  8			7  [ ] [ ]  8
9  [ ] [ ] 10			9  [ ] [ ] 10
11 [ ] [ ] 12			11 [ ] [ ] 12
13 [ ] [ ] 14			13 [ ] [ ] 14
15 [ ] [ ] 16			15 [ ] [ ] 16
17 [ ] [ ] 18			17 [ ] [ ] 18
19 [ ] [ ] 20			19 [ ] [ ] 20
21 [ ] [ ] 22			21 [ ] [ ] 22
23 [ ] [ ] 24			23 [ ] [ ] 24
25 [ ] [ ] 26			25 [ ] [ ] 26
27 [ ] [ ] 28			27 [ ] [ ] 28
29 [ ] [ ] 30			29 [ ] [ ] 30
	 CN9				31 [ ] [ ] 32
						33 [ ] [ ] 34
     	 	 	 	 	 	 CN10


************************************/

#ifndef INC_BALLOON_UTILS_H_
#define INC_BALLOON_UTILS_H_
#include <stdbool.h>

enum PowerSource {STM, LORA, STRATO_MAIN, STRATO_BCKP};

bool set_power_source(int source);
bool clear_power_source(int source);

#endif /* INC_BALLOON_UTILS_H_ */


