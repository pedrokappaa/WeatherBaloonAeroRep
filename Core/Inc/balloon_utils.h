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
7  [E] [ ]  8			7  [ ] [ ]  8
9  [ ] [ ] 10			9  [ ] [A] 10
11 [F] [ ] 12			11 [ ] [N] 12
13 [K] [ ] 14			13 [ ] [C] 14
15 [ ] [ ] 16			15 [ ] [D] 16
						17 [ ] [ ] 18
						19 [ ] [ ] 20

1  [M] [ ]  2			1  [ ] [ ]  2
3  [ ] [ ]  4			3  [ ] [ ]  4
5  [ ] [ ]  6			5  [ ] [ ]  6
7  [ ] [ ]  8			7  [G] [ ]  8
9  [ ] [ ] 10			9  [ ] [ ] 10
11 [ ] [ ] 12			11 [H] [ ] 12
13 [ ] [ ] 14			13 [I] [ ] 14
15 [ ] [ ] 16			15 [ ] [B] 16
17 [ ] [ ] 18			17 [ ] [ ] 18
19 [ ] [ ] 20			19 [ ] [ ] 20
21 [ ] [ ] 22			21 [ ] [ ] 22
23 [ ] [ ] 24			23 [ ] [ ] 24
25 [ ] [ ] 26			25 [ ] [ ] 26
27 [ ] [ ] 28			27 [ ] [ ] 28
29 [ ] [ ] 30			29 [L] [ ] 30
	 CN9				31 [ ] [ ] 32
						33 [ ] [ ] 34
     	 	 	 	 	 	 CN10

A - SPI1_SCLK	(SDCARD)		PA5
B - SPI1_MISO	(SDCARD)		PG9
C - SPI1_MOSI	(SDCARD)		PA7
D - SPI1_CS		(SDCARD)		PD14
E - 3V3			(SDCARD)
F - GND			(SDCARD)
G - GPIO_STM	(STM)			PB1
H - GPIO_DL		(DATALOGGER)	PF4
I - GPIO_DL2	(DATALOGGER)	PB6
K - GND			(DATALOGGER)
L - ADC1_CH0	(V_LC)			PA0
M - ADC1_CH3	(V_AA)			PA3
N - ADC1_CH6	(V_9V)			PA6



************************************/

#ifndef INC_BALLOON_UTILS_H_
#define INC_BALLOON_UTILS_H_
#define MINIMUM_VOLTAGE_DATALOGGER 7.5 //TESTAR VALORES
//#define MINIMUM_VOLTAGE_SYSTEM 7.5
#include <stdbool.h>

enum PowerSource {STM, LORA, STRATO_MAIN, STRATO_BCKP};

bool set_power_source(int source);
bool clear_power_source(int source);

float convert_adc_to_physical_value(int adc_read, int adc_read_a, float point_a, int adc_read_b, float point_b);

#endif /* INC_BALLOON_UTILS_H_ */


