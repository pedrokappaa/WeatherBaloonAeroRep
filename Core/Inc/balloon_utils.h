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
11 [F] [ ] 12			11 [ ] [B] 12
13 [K] [ ] 14			13 [ ] [C] 14
15 [ ] [ ] 16			15 [ ] [D] 16
						17 [ ] [ ] 18
						19 [ ] [ ] 20

1  [M] [ ]  2			1  [ ] [ ]  2
3  [N] [ ]  4			3  [ ] [ ]  4
5  [O] [ ]  6			5  [ ] [ ]  6
7  [ ] [ ]  8			7  [ ] [ ]  8
9  [ ] [ ] 10			9  [ ] [ ] 10
11 [ ] [ ] 12			11 [ ] [ ] 12
13 [ ] [G] 14			13 [ ] [ ] 14
15 [ ] [H] 16			15 [ ] [ ] 16
17 [ ] [I] 18			17 [ ] [ ] 18
19 [ ] [J] 20			19 [ ] [ ] 20
21 [ ] [ ] 22			21 [ ] [ ] 22
23 [ ] [ ] 24			23 [ ] [ ] 24
25 [ ] [ ] 26			25 [ ] [ ] 26
27 [ ] [ ] 28			27 [ ] [ ] 28
29 [ ] [ ] 30			29 [L] [ ] 30
	 CN9				31 [ ] [ ] 32
						33 [ ] [ ] 34
     	 	 	 	 	 	 CN10

A - SPI1_SCLK	(SDCARD)
B - SPI1_MISO	(SDCARD)
C - SPI1_MOSI	(SDCARD)
D - SPI1_CS		(SDCARD)
E - 3V3			(SDCARD)
F - GND			(SDCARD)
G - SPI4_SCK	(LORA)
H - SPI4_CS		(LORA)
I - SPI4_MISO	(LORA)
J - SPI4_MOSI	(LORA)
K - GND			(LORA)
L - ADC1_CH0	()
M - ADC1_CH3	()
N - ADC1_CH10	()
O - ADC1_CH13	()


************************************/

#ifndef INC_BALLOON_UTILS_H_
#define INC_BALLOON_UTILS_H_
#define MINIMUM_VOLTAGE_DATALOGGER 7.5
#define MINIMUM_VOLTAGE_SYSTEM 7.5
#include <stdbool.h>

enum PowerSource {STM, LORA, STRATO_MAIN, STRATO_BCKP};

bool set_power_source(int source);
bool clear_power_source(int source);

float convert_adc_to_physical_value(int adc_read, int adc_read_a, float point_a, int adc_read_b, float point_b);

#endif /* INC_BALLOON_UTILS_H_ */


