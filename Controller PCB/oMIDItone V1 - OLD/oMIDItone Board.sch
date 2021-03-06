EESchema Schematic File Version 4
LIBS:oMIDItone Board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L teensy:Teensy-LC U1
U 1 1 5BE4B96D
P 3050 1850
F 0 "U1" H 3075 3087 60  0000 C CNN
F 1 "Teensy-LC" H 3075 2981 60  0000 C CNN
F 2 "teensy:Teensy_LC" H 3050 1300 60  0001 C CNN
F 3 "" H 3050 1300 60  0000 C CNN
	1    3050 1850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 5BE4ED9C
P 1950 2000
F 0 "#PWR0105" H 1950 1750 50  0001 C CNN
F 1 "GND" V 1955 1872 50  0000 R CNN
F 2 "" H 1950 2000 50  0001 C CNN
F 3 "" H 1950 2000 50  0001 C CNN
	1    1950 2000
	0    1    1    0   
$EndComp
Text GLabel 4100 1600 2    50   Input ~ 0
D11
Text GLabel 4100 900  2    50   Input ~ 0
D13
$Comp
L power:+5V #PWR0108
U 1 1 5BE506D5
P 1950 2100
F 0 "#PWR0108" H 1950 1950 50  0001 C CNN
F 1 "+5V" V 1965 2228 50  0000 L CNN
F 2 "" H 1950 2100 50  0001 C CNN
F 3 "" H 1950 2100 50  0001 C CNN
	1    1950 2100
	0    -1   -1   0   
$EndComp
NoConn ~ 1950 2500
NoConn ~ 1950 2600
NoConn ~ 1950 1900
NoConn ~ 4100 1300
NoConn ~ 4100 1100
Text GLabel 4100 2400 2    50   Input ~ 0
D03
Text GLabel 4100 2300 2    50   Input ~ 0
D04
Text GLabel 4100 2100 2    50   Input ~ 0
D06
Text GLabel 4100 1800 2    50   Input ~ 0
D09
Text GLabel 4100 1700 2    50   Input ~ 0
D10
NoConn ~ 4100 1500
Text GLabel 1950 1800 0    50   Input ~ 0
A9
Text GLabel 1950 900  0    50   Input ~ 0
D14
Text GLabel 1950 1000 0    50   Input ~ 0
D15
Text GLabel 1950 1100 0    50   Input ~ 0
A2
Text GLabel 1950 1200 0    50   Input ~ 0
D17
Text GLabel 1950 1300 0    50   Input ~ 0
D18
Text GLabel 1950 1400 0    50   Input ~ 0
A5
$Comp
L power:GND #PWR0141
U 1 1 5BE5BBC5
P 4100 2800
F 0 "#PWR0141" H 4100 2550 50  0001 C CNN
F 1 "GND" V 4105 2672 50  0000 R CNN
F 2 "" H 4100 2800 50  0001 C CNN
F 3 "" H 4100 2800 50  0001 C CNN
	1    4100 2800
	0    -1   -1   0   
$EndComp
Text GLabel 4100 2700 2    50   Input ~ 0
D00
Text GLabel 4100 2600 2    50   Input ~ 0
D01
Text GLabel 4100 2500 2    50   Input ~ 0
D02
Text GLabel 4100 2200 2    50   Input ~ 0
D05
Text GLabel 4100 2000 2    50   Input ~ 0
D07
Text GLabel 4100 1900 2    50   Input ~ 0
D08
Text GLabel 4100 1000 2    50   Input ~ 0
D26
Text GLabel 1950 1500 0    50   Input ~ 0
A6
Text GLabel 1950 1600 0    50   Input ~ 0
A7
Text GLabel 1950 1700 0    50   Input ~ 0
A8
Text GLabel 1950 2700 0    50   Input ~ 0
D24
Text GLabel 1950 2800 0    50   Input ~ 0
D25
NoConn ~ 4100 1400
$Comp
L power:GND #PWR0109
U 1 1 5BED405D
P 4100 1200
F 0 "#PWR0109" H 4100 950 50  0001 C CNN
F 1 "GND" V 4105 1072 50  0000 R CNN
F 2 "" H 4100 1200 50  0001 C CNN
F 3 "" H 4100 1200 50  0001 C CNN
	1    4100 1200
	0    -1   -1   0   
$EndComp
$Comp
L oMIDItone-Board-rescue:MountingHole_Pad-Mechanical H1
U 1 1 5BEE3E4E
P 2450 3600
F 0 "H1" V 2404 3750 50  0000 L CNN
F 1 "MountingHole_Pad" V 2495 3750 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad_Via" H 2450 3600 50  0001 C CNN
F 3 "~" H 2450 3600 50  0001 C CNN
	1    2450 3600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5BEE6FE0
P 2350 3600
F 0 "#PWR0110" H 2350 3350 50  0001 C CNN
F 1 "GND" V 2355 3472 50  0000 R CNN
F 2 "" H 2350 3600 50  0001 C CNN
F 3 "" H 2350 3600 50  0001 C CNN
	1    2350 3600
	0    1    1    0   
$EndComp
$Comp
L oMIDItone-Board-rescue:MountingHole_Pad-Mechanical H2
U 1 1 5BEE7080
P 2450 3800
F 0 "H2" V 2404 3950 50  0000 L CNN
F 1 "MountingHole_Pad" V 2495 3950 50  0000 L CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_Pad_Via" H 2450 3800 50  0001 C CNN
F 3 "~" H 2450 3800 50  0001 C CNN
	1    2450 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0142
U 1 1 5BEE7087
P 2350 3800
F 0 "#PWR0142" H 2350 3550 50  0001 C CNN
F 1 "GND" V 2355 3672 50  0000 R CNN
F 2 "" H 2350 3800 50  0001 C CNN
F 3 "" H 2350 3800 50  0001 C CNN
	1    2350 3800
	0    1    1    0   
$EndComp
Text GLabel 9700 1150 0    50   Input ~ 0
A2
Text GLabel 8150 950  0    50   Input ~ 0
D15
Text GLabel 6550 950  0    50   Input ~ 0
D14
Text GLabel 5250 1150 0    50   Input ~ 0
D26
Text GLabel 5250 2000 0    50   Input ~ 0
D10
Text GLabel 6550 1800 0    50   Input ~ 0
D17
Text GLabel 8150 1800 0    50   Input ~ 0
D18
Text GLabel 9700 2000 0    50   Input ~ 0
A5
Text GLabel 9650 2850 0    50   Input ~ 0
A6
Text GLabel 8100 2650 0    50   Input ~ 0
D07
Text GLabel 6500 2650 0    50   Input ~ 0
D08
Text GLabel 5200 2850 0    50   Input ~ 0
D09
Text GLabel 5200 3700 0    50   Input ~ 0
D06
Text GLabel 6500 3500 0    50   Input ~ 0
D25
Text GLabel 8100 3500 0    50   Input ~ 0
D24
Text GLabel 9650 3700 0    50   Input ~ 0
A7
Text GLabel 9650 4600 0    50   Input ~ 0
A8
Text GLabel 8100 4400 0    50   Input ~ 0
D02
Text GLabel 6500 4400 0    50   Input ~ 0
D05
Text GLabel 5200 4600 0    50   Input ~ 0
D04
Text GLabel 5200 5450 0    50   Input ~ 0
D03
Text GLabel 6500 5250 0    50   Input ~ 0
D01
Text GLabel 9650 5450 0    50   Input ~ 0
A9
Text GLabel 8100 5250 0    50   Input ~ 0
D00
$Comp
L power:+5V #PWR0140
U 1 1 5BE53874
P 7500 5250
F 0 "#PWR0140" H 7500 5100 50  0001 C CNN
F 1 "+5V" V 7515 5378 50  0000 L CNN
F 2 "" H 7500 5250 50  0001 C CNN
F 3 "" H 7500 5250 50  0001 C CNN
	1    7500 5250
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0139
U 1 1 5BE5386E
P 9100 5250
F 0 "#PWR0139" H 9100 5100 50  0001 C CNN
F 1 "+5V" V 9115 5378 50  0000 L CNN
F 2 "" H 9100 5250 50  0001 C CNN
F 3 "" H 9100 5250 50  0001 C CNN
	1    9100 5250
	0    1    1    0   
$EndComp
NoConn ~ 7500 5350
NoConn ~ 9100 5350
Text GLabel 8100 5350 0    50   Input ~ 0
D13
Text GLabel 8100 5450 0    50   Input ~ 0
D11
Text GLabel 6500 5450 0    50   Input ~ 0
D11
Text GLabel 6500 5350 0    50   Input ~ 0
D13
Wire Wire Line
	5150 5250 5200 5250
Wire Wire Line
	5150 4950 5150 5250
Wire Wire Line
	9500 4950 5150 4950
Wire Wire Line
	9500 5250 9500 4950
Wire Wire Line
	9650 5250 9500 5250
Wire Wire Line
	4850 5350 5200 5350
Wire Wire Line
	4850 5700 4850 5350
Wire Wire Line
	7500 5700 4850 5700
Wire Wire Line
	7500 5550 7500 5700
Wire Wire Line
	7750 5450 7500 5450
Wire Wire Line
	7750 5700 7750 5450
Wire Wire Line
	9200 5700 7750 5700
Wire Wire Line
	9200 5550 9200 5700
Wire Wire Line
	9100 5550 9200 5550
Wire Wire Line
	9350 5350 9650 5350
Wire Wire Line
	9350 5450 9350 5350
Wire Wire Line
	9100 5450 9350 5450
$Comp
L power:GND #PWR0138
U 1 1 5BE53851
P 5200 5550
F 0 "#PWR0138" H 5200 5300 50  0001 C CNN
F 1 "GND" V 5205 5422 50  0000 R CNN
F 2 "" H 5200 5550 50  0001 C CNN
F 3 "" H 5200 5550 50  0001 C CNN
	1    5200 5550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0137
U 1 1 5BE5384B
P 6500 5550
F 0 "#PWR0137" H 6500 5300 50  0001 C CNN
F 1 "GND" V 6505 5422 50  0000 R CNN
F 2 "" H 6500 5550 50  0001 C CNN
F 3 "" H 6500 5550 50  0001 C CNN
	1    6500 5550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0136
U 1 1 5BE53845
P 8100 5550
F 0 "#PWR0136" H 8100 5300 50  0001 C CNN
F 1 "GND" V 8105 5422 50  0000 R CNN
F 2 "" H 8100 5550 50  0001 C CNN
F 3 "" H 8100 5550 50  0001 C CNN
	1    8100 5550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0135
U 1 1 5BE5383F
P 9650 5550
F 0 "#PWR0135" H 9650 5300 50  0001 C CNN
F 1 "GND" V 9655 5422 50  0000 R CNN
F 2 "" H 9650 5550 50  0001 C CNN
F 3 "" H 9650 5550 50  0001 C CNN
	1    9650 5550
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J6
U 1 1 5BE53838
P 9750 5400
F 0 "J6" H 10028 5405 50  0000 L CNN
F 1 "oMIDItonePort" V 10050 5400 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 9950 5450 50  0001 C CNN
F 3 "" H 9950 5450 50  0001 C CNN
	1    9750 5400
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U13
U 1 1 5BE53831
P 8600 5400
F 0 "U13" H 8600 5775 50  0000 C CNN
F 1 "MCP-4151" H 8600 5684 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8600 5750 50  0001 C CNN
F 3 "" H 8600 5750 50  0001 C CNN
	1    8600 5400
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K6
U 1 1 5BE5382A
P 5300 5400
F 0 "K6" H 5578 5451 50  0000 L CNN
F 1 "KF0602D" H 5578 5360 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5400 5650 50  0001 C CNN
F 3 "" H 5400 5650 50  0001 C CNN
	1    5300 5400
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U12
U 1 1 5BE53823
P 7000 5400
F 0 "U12" H 7000 5775 50  0000 C CNN
F 1 "MCP-4151" H 7000 5684 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7000 5750 50  0001 C CNN
F 3 "" H 7000 5750 50  0001 C CNN
	1    7000 5400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0134
U 1 1 5BE5381D
P 7500 4400
F 0 "#PWR0134" H 7500 4250 50  0001 C CNN
F 1 "+5V" V 7515 4528 50  0000 L CNN
F 2 "" H 7500 4400 50  0001 C CNN
F 3 "" H 7500 4400 50  0001 C CNN
	1    7500 4400
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0133
U 1 1 5BE53817
P 9100 4400
F 0 "#PWR0133" H 9100 4250 50  0001 C CNN
F 1 "+5V" V 9115 4528 50  0000 L CNN
F 2 "" H 9100 4400 50  0001 C CNN
F 3 "" H 9100 4400 50  0001 C CNN
	1    9100 4400
	0    1    1    0   
$EndComp
NoConn ~ 7500 4500
NoConn ~ 9100 4500
Text GLabel 8100 4500 0    50   Input ~ 0
D13
Text GLabel 8100 4600 0    50   Input ~ 0
D11
Text GLabel 6500 4600 0    50   Input ~ 0
D11
Text GLabel 6500 4500 0    50   Input ~ 0
D13
Wire Wire Line
	5150 4400 5200 4400
Wire Wire Line
	5150 4100 5150 4400
Wire Wire Line
	9500 4100 5150 4100
Wire Wire Line
	9500 4400 9500 4100
Wire Wire Line
	9650 4400 9500 4400
Wire Wire Line
	4850 4500 5200 4500
Wire Wire Line
	4850 4850 4850 4500
Wire Wire Line
	7500 4850 4850 4850
Wire Wire Line
	7500 4700 7500 4850
Wire Wire Line
	7750 4600 7500 4600
Wire Wire Line
	7750 4850 7750 4600
Wire Wire Line
	9200 4850 7750 4850
Wire Wire Line
	9200 4700 9200 4850
Wire Wire Line
	9100 4700 9200 4700
Wire Wire Line
	9350 4500 9650 4500
Wire Wire Line
	9350 4600 9350 4500
Wire Wire Line
	9100 4600 9350 4600
$Comp
L power:GND #PWR0132
U 1 1 5BE537FA
P 5200 4700
F 0 "#PWR0132" H 5200 4450 50  0001 C CNN
F 1 "GND" V 5205 4572 50  0000 R CNN
F 2 "" H 5200 4700 50  0001 C CNN
F 3 "" H 5200 4700 50  0001 C CNN
	1    5200 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0131
U 1 1 5BE537F4
P 6500 4700
F 0 "#PWR0131" H 6500 4450 50  0001 C CNN
F 1 "GND" V 6505 4572 50  0000 R CNN
F 2 "" H 6500 4700 50  0001 C CNN
F 3 "" H 6500 4700 50  0001 C CNN
	1    6500 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0130
U 1 1 5BE537EE
P 8100 4700
F 0 "#PWR0130" H 8100 4450 50  0001 C CNN
F 1 "GND" V 8105 4572 50  0000 R CNN
F 2 "" H 8100 4700 50  0001 C CNN
F 3 "" H 8100 4700 50  0001 C CNN
	1    8100 4700
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0129
U 1 1 5BE537E8
P 9650 4700
F 0 "#PWR0129" H 9650 4450 50  0001 C CNN
F 1 "GND" V 9655 4572 50  0000 R CNN
F 2 "" H 9650 4700 50  0001 C CNN
F 3 "" H 9650 4700 50  0001 C CNN
	1    9650 4700
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J5
U 1 1 5BE537E1
P 9750 4550
F 0 "J5" H 10028 4555 50  0000 L CNN
F 1 "oMIDItonePort" V 10050 4550 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 9950 4600 50  0001 C CNN
F 3 "" H 9950 4600 50  0001 C CNN
	1    9750 4550
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U11
U 1 1 5BE537DA
P 8600 4550
F 0 "U11" H 8600 4925 50  0000 C CNN
F 1 "MCP-4151" H 8600 4834 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8600 4900 50  0001 C CNN
F 3 "" H 8600 4900 50  0001 C CNN
	1    8600 4550
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K5
U 1 1 5BE537D3
P 5300 4550
F 0 "K5" H 5578 4601 50  0000 L CNN
F 1 "KF0602D" H 5578 4510 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5400 4800 50  0001 C CNN
F 3 "" H 5400 4800 50  0001 C CNN
	1    5300 4550
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U10
U 1 1 5BE537CC
P 7000 4550
F 0 "U10" H 7000 4925 50  0000 C CNN
F 1 "MCP-4151" H 7000 4834 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7000 4900 50  0001 C CNN
F 3 "" H 7000 4900 50  0001 C CNN
	1    7000 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0128
U 1 1 5BE52070
P 7500 3500
F 0 "#PWR0128" H 7500 3350 50  0001 C CNN
F 1 "+5V" V 7515 3628 50  0000 L CNN
F 2 "" H 7500 3500 50  0001 C CNN
F 3 "" H 7500 3500 50  0001 C CNN
	1    7500 3500
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0127
U 1 1 5BE5206A
P 9100 3500
F 0 "#PWR0127" H 9100 3350 50  0001 C CNN
F 1 "+5V" V 9115 3628 50  0000 L CNN
F 2 "" H 9100 3500 50  0001 C CNN
F 3 "" H 9100 3500 50  0001 C CNN
	1    9100 3500
	0    1    1    0   
$EndComp
NoConn ~ 7500 3600
NoConn ~ 9100 3600
Text GLabel 8100 3600 0    50   Input ~ 0
D13
Text GLabel 8100 3700 0    50   Input ~ 0
D11
Text GLabel 6500 3700 0    50   Input ~ 0
D11
Text GLabel 6500 3600 0    50   Input ~ 0
D13
Wire Wire Line
	5150 3500 5200 3500
Wire Wire Line
	5150 3200 5150 3500
Wire Wire Line
	9500 3200 5150 3200
Wire Wire Line
	9500 3500 9500 3200
Wire Wire Line
	9650 3500 9500 3500
Wire Wire Line
	4850 3600 5200 3600
Wire Wire Line
	4850 3950 4850 3600
Wire Wire Line
	7500 3950 4850 3950
Wire Wire Line
	7500 3800 7500 3950
Wire Wire Line
	7750 3700 7500 3700
Wire Wire Line
	7750 3950 7750 3700
Wire Wire Line
	9200 3950 7750 3950
Wire Wire Line
	9200 3800 9200 3950
Wire Wire Line
	9100 3800 9200 3800
Wire Wire Line
	9350 3600 9650 3600
Wire Wire Line
	9350 3700 9350 3600
Wire Wire Line
	9100 3700 9350 3700
$Comp
L power:GND #PWR0126
U 1 1 5BE5204D
P 5200 3800
F 0 "#PWR0126" H 5200 3550 50  0001 C CNN
F 1 "GND" V 5205 3672 50  0000 R CNN
F 2 "" H 5200 3800 50  0001 C CNN
F 3 "" H 5200 3800 50  0001 C CNN
	1    5200 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0125
U 1 1 5BE52047
P 6500 3800
F 0 "#PWR0125" H 6500 3550 50  0001 C CNN
F 1 "GND" V 6505 3672 50  0000 R CNN
F 2 "" H 6500 3800 50  0001 C CNN
F 3 "" H 6500 3800 50  0001 C CNN
	1    6500 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 5BE52041
P 8100 3800
F 0 "#PWR0124" H 8100 3550 50  0001 C CNN
F 1 "GND" V 8105 3672 50  0000 R CNN
F 2 "" H 8100 3800 50  0001 C CNN
F 3 "" H 8100 3800 50  0001 C CNN
	1    8100 3800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5BE5203B
P 9650 3800
F 0 "#PWR0123" H 9650 3550 50  0001 C CNN
F 1 "GND" V 9655 3672 50  0000 R CNN
F 2 "" H 9650 3800 50  0001 C CNN
F 3 "" H 9650 3800 50  0001 C CNN
	1    9650 3800
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J4
U 1 1 5BE52034
P 9750 3650
F 0 "J4" H 10028 3655 50  0000 L CNN
F 1 "oMIDItonePort" V 10050 3650 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 9950 3700 50  0001 C CNN
F 3 "" H 9950 3700 50  0001 C CNN
	1    9750 3650
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U9
U 1 1 5BE5202D
P 8600 3650
F 0 "U9" H 8600 4025 50  0000 C CNN
F 1 "MCP-4151" H 8600 3934 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8600 4000 50  0001 C CNN
F 3 "" H 8600 4000 50  0001 C CNN
	1    8600 3650
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K4
U 1 1 5BE52026
P 5300 3650
F 0 "K4" H 5578 3701 50  0000 L CNN
F 1 "KF0602D" H 5578 3610 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5400 3900 50  0001 C CNN
F 3 "" H 5400 3900 50  0001 C CNN
	1    5300 3650
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U8
U 1 1 5BE5201F
P 7000 3650
F 0 "U8" H 7000 4025 50  0000 C CNN
F 1 "MCP-4151" H 7000 3934 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7000 4000 50  0001 C CNN
F 3 "" H 7000 4000 50  0001 C CNN
	1    7000 3650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0122
U 1 1 5BE52019
P 7500 2650
F 0 "#PWR0122" H 7500 2500 50  0001 C CNN
F 1 "+5V" V 7515 2778 50  0000 L CNN
F 2 "" H 7500 2650 50  0001 C CNN
F 3 "" H 7500 2650 50  0001 C CNN
	1    7500 2650
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0121
U 1 1 5BE52013
P 9100 2650
F 0 "#PWR0121" H 9100 2500 50  0001 C CNN
F 1 "+5V" V 9115 2778 50  0000 L CNN
F 2 "" H 9100 2650 50  0001 C CNN
F 3 "" H 9100 2650 50  0001 C CNN
	1    9100 2650
	0    1    1    0   
$EndComp
NoConn ~ 7500 2750
NoConn ~ 9100 2750
Text GLabel 8100 2750 0    50   Input ~ 0
D13
Text GLabel 8100 2850 0    50   Input ~ 0
D11
Text GLabel 6500 2850 0    50   Input ~ 0
D11
Text GLabel 6500 2750 0    50   Input ~ 0
D13
Wire Wire Line
	5150 2650 5200 2650
Wire Wire Line
	5150 2350 5150 2650
Wire Wire Line
	9500 2350 5150 2350
Wire Wire Line
	9500 2650 9500 2350
Wire Wire Line
	9650 2650 9500 2650
Wire Wire Line
	4850 2750 5200 2750
Wire Wire Line
	4850 3100 4850 2750
Wire Wire Line
	7500 3100 4850 3100
Wire Wire Line
	7500 2950 7500 3100
Wire Wire Line
	7750 2850 7500 2850
Wire Wire Line
	7750 3100 7750 2850
Wire Wire Line
	9200 3100 7750 3100
Wire Wire Line
	9200 2950 9200 3100
Wire Wire Line
	9100 2950 9200 2950
Wire Wire Line
	9350 2750 9650 2750
Wire Wire Line
	9350 2850 9350 2750
Wire Wire Line
	9100 2850 9350 2850
$Comp
L power:GND #PWR0120
U 1 1 5BE51FF6
P 5200 2950
F 0 "#PWR0120" H 5200 2700 50  0001 C CNN
F 1 "GND" V 5205 2822 50  0000 R CNN
F 2 "" H 5200 2950 50  0001 C CNN
F 3 "" H 5200 2950 50  0001 C CNN
	1    5200 2950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5BE51FF0
P 6500 2950
F 0 "#PWR0119" H 6500 2700 50  0001 C CNN
F 1 "GND" V 6505 2822 50  0000 R CNN
F 2 "" H 6500 2950 50  0001 C CNN
F 3 "" H 6500 2950 50  0001 C CNN
	1    6500 2950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0118
U 1 1 5BE51FEA
P 8100 2950
F 0 "#PWR0118" H 8100 2700 50  0001 C CNN
F 1 "GND" V 8105 2822 50  0000 R CNN
F 2 "" H 8100 2950 50  0001 C CNN
F 3 "" H 8100 2950 50  0001 C CNN
	1    8100 2950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 5BE51FE4
P 9650 2950
F 0 "#PWR0117" H 9650 2700 50  0001 C CNN
F 1 "GND" V 9655 2822 50  0000 R CNN
F 2 "" H 9650 2950 50  0001 C CNN
F 3 "" H 9650 2950 50  0001 C CNN
	1    9650 2950
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J3
U 1 1 5BE51FDD
P 9750 2800
F 0 "J3" H 10028 2805 50  0000 L CNN
F 1 "oMIDItonePort" V 10050 2800 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 9950 2850 50  0001 C CNN
F 3 "" H 9950 2850 50  0001 C CNN
	1    9750 2800
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U7
U 1 1 5BE51FD6
P 8600 2800
F 0 "U7" H 8600 3175 50  0000 C CNN
F 1 "MCP-4151" H 8600 3084 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8600 3150 50  0001 C CNN
F 3 "" H 8600 3150 50  0001 C CNN
	1    8600 2800
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K3
U 1 1 5BE51FCF
P 5300 2800
F 0 "K3" H 5578 2851 50  0000 L CNN
F 1 "KF0602D" H 5578 2760 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5400 3050 50  0001 C CNN
F 3 "" H 5400 3050 50  0001 C CNN
	1    5300 2800
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U6
U 1 1 5BE51FC8
P 7000 2800
F 0 "U6" H 7000 3175 50  0000 C CNN
F 1 "MCP-4151" H 7000 3084 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7000 3150 50  0001 C CNN
F 3 "" H 7000 3150 50  0001 C CNN
	1    7000 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0116
U 1 1 5BE5160A
P 7550 1800
F 0 "#PWR0116" H 7550 1650 50  0001 C CNN
F 1 "+5V" V 7565 1928 50  0000 L CNN
F 2 "" H 7550 1800 50  0001 C CNN
F 3 "" H 7550 1800 50  0001 C CNN
	1    7550 1800
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 5BE51604
P 9150 1800
F 0 "#PWR0115" H 9150 1650 50  0001 C CNN
F 1 "+5V" V 9165 1928 50  0000 L CNN
F 2 "" H 9150 1800 50  0001 C CNN
F 3 "" H 9150 1800 50  0001 C CNN
	1    9150 1800
	0    1    1    0   
$EndComp
NoConn ~ 7550 1900
NoConn ~ 9150 1900
Text GLabel 8150 1900 0    50   Input ~ 0
D13
Text GLabel 8150 2000 0    50   Input ~ 0
D11
Text GLabel 6550 2000 0    50   Input ~ 0
D11
Text GLabel 6550 1900 0    50   Input ~ 0
D13
Wire Wire Line
	5200 1800 5250 1800
Wire Wire Line
	5200 1500 5200 1800
Wire Wire Line
	9550 1500 5200 1500
Wire Wire Line
	9550 1800 9550 1500
Wire Wire Line
	9700 1800 9550 1800
Wire Wire Line
	4900 1900 5250 1900
Wire Wire Line
	4900 2250 4900 1900
Wire Wire Line
	7550 2250 4900 2250
Wire Wire Line
	7550 2100 7550 2250
Wire Wire Line
	7800 2000 7550 2000
Wire Wire Line
	7800 2250 7800 2000
Wire Wire Line
	9250 2250 7800 2250
Wire Wire Line
	9250 2100 9250 2250
Wire Wire Line
	9150 2100 9250 2100
Wire Wire Line
	9400 1900 9700 1900
Wire Wire Line
	9400 2000 9400 1900
Wire Wire Line
	9150 2000 9400 2000
$Comp
L power:GND #PWR0114
U 1 1 5BE515E7
P 5250 2100
F 0 "#PWR0114" H 5250 1850 50  0001 C CNN
F 1 "GND" V 5255 1972 50  0000 R CNN
F 2 "" H 5250 2100 50  0001 C CNN
F 3 "" H 5250 2100 50  0001 C CNN
	1    5250 2100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 5BE515E1
P 6550 2100
F 0 "#PWR0113" H 6550 1850 50  0001 C CNN
F 1 "GND" V 6555 1972 50  0000 R CNN
F 2 "" H 6550 2100 50  0001 C CNN
F 3 "" H 6550 2100 50  0001 C CNN
	1    6550 2100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 5BE515DB
P 8150 2100
F 0 "#PWR0112" H 8150 1850 50  0001 C CNN
F 1 "GND" V 8155 1972 50  0000 R CNN
F 2 "" H 8150 2100 50  0001 C CNN
F 3 "" H 8150 2100 50  0001 C CNN
	1    8150 2100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 5BE515D5
P 9700 2100
F 0 "#PWR0111" H 9700 1850 50  0001 C CNN
F 1 "GND" V 9705 1972 50  0000 R CNN
F 2 "" H 9700 2100 50  0001 C CNN
F 3 "" H 9700 2100 50  0001 C CNN
	1    9700 2100
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J2
U 1 1 5BE515CE
P 9800 1950
F 0 "J2" H 10078 1955 50  0000 L CNN
F 1 "oMIDItonePort" V 10100 1950 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 10000 2000 50  0001 C CNN
F 3 "" H 10000 2000 50  0001 C CNN
	1    9800 1950
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U5
U 1 1 5BE515C7
P 8650 1950
F 0 "U5" H 8650 2325 50  0000 C CNN
F 1 "MCP-4151" H 8650 2234 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8650 2300 50  0001 C CNN
F 3 "" H 8650 2300 50  0001 C CNN
	1    8650 1950
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K2
U 1 1 5BE515C0
P 5350 1950
F 0 "K2" H 5628 2001 50  0000 L CNN
F 1 "KF0602D" H 5628 1910 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5450 2200 50  0001 C CNN
F 3 "" H 5450 2200 50  0001 C CNN
	1    5350 1950
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U4
U 1 1 5BE515B9
P 7050 1950
F 0 "U4" H 7050 2325 50  0000 C CNN
F 1 "MCP-4151" H 7050 2234 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7050 2300 50  0001 C CNN
F 3 "" H 7050 2300 50  0001 C CNN
	1    7050 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0107
U 1 1 5BE505FF
P 7550 950
F 0 "#PWR0107" H 7550 800 50  0001 C CNN
F 1 "+5V" V 7565 1078 50  0000 L CNN
F 2 "" H 7550 950 50  0001 C CNN
F 3 "" H 7550 950 50  0001 C CNN
	1    7550 950 
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 5BE505C9
P 9150 950
F 0 "#PWR0106" H 9150 800 50  0001 C CNN
F 1 "+5V" V 9165 1078 50  0000 L CNN
F 2 "" H 9150 950 50  0001 C CNN
F 3 "" H 9150 950 50  0001 C CNN
	1    9150 950 
	0    1    1    0   
$EndComp
NoConn ~ 7550 1050
NoConn ~ 9150 1050
Text GLabel 8150 1050 0    50   Input ~ 0
D13
Text GLabel 8150 1150 0    50   Input ~ 0
D11
Text GLabel 6550 1150 0    50   Input ~ 0
D11
Text GLabel 6550 1050 0    50   Input ~ 0
D13
Wire Wire Line
	5200 950  5250 950 
Wire Wire Line
	5200 650  5200 950 
Wire Wire Line
	9550 650  5200 650 
Wire Wire Line
	9550 950  9550 650 
Wire Wire Line
	9700 950  9550 950 
Wire Wire Line
	4900 1050 5250 1050
Wire Wire Line
	4900 1400 4900 1050
Wire Wire Line
	7550 1400 4900 1400
Wire Wire Line
	7550 1250 7550 1400
Wire Wire Line
	7800 1150 7550 1150
Wire Wire Line
	7800 1400 7800 1150
Wire Wire Line
	9250 1400 7800 1400
Wire Wire Line
	9250 1250 9250 1400
Wire Wire Line
	9150 1250 9250 1250
Wire Wire Line
	9400 1050 9700 1050
Wire Wire Line
	9400 1150 9400 1050
Wire Wire Line
	9150 1150 9400 1150
$Comp
L power:GND #PWR0104
U 1 1 5BE4ED18
P 5250 1250
F 0 "#PWR0104" H 5250 1000 50  0001 C CNN
F 1 "GND" V 5255 1122 50  0000 R CNN
F 2 "" H 5250 1250 50  0001 C CNN
F 3 "" H 5250 1250 50  0001 C CNN
	1    5250 1250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5BE4ECBD
P 6550 1250
F 0 "#PWR0103" H 6550 1000 50  0001 C CNN
F 1 "GND" V 6555 1122 50  0000 R CNN
F 2 "" H 6550 1250 50  0001 C CNN
F 3 "" H 6550 1250 50  0001 C CNN
	1    6550 1250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5BE4EC92
P 8150 1250
F 0 "#PWR0102" H 8150 1000 50  0001 C CNN
F 1 "GND" V 8155 1122 50  0000 R CNN
F 2 "" H 8150 1250 50  0001 C CNN
F 3 "" H 8150 1250 50  0001 C CNN
	1    8150 1250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5BE4EBD5
P 9700 1250
F 0 "#PWR0101" H 9700 1000 50  0001 C CNN
F 1 "GND" V 9705 1122 50  0000 R CNN
F 2 "" H 9700 1250 50  0001 C CNN
F 3 "" H 9700 1250 50  0001 C CNN
	1    9700 1250
	0    1    1    0   
$EndComp
$Comp
L kiyoshigawa_lib:oMIDItonePort J1
U 1 1 5BE4EA88
P 9800 1100
F 0 "J1" H 10078 1105 50  0000 L CNN
F 1 "oMIDItonePort" V 10100 1100 50  0001 C CNN
F 2 "kiyoshigawa_lib:kioMIDItone COnnector" H 10000 1150 50  0001 C CNN
F 3 "" H 10000 1150 50  0001 C CNN
	1    9800 1100
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U3
U 1 1 5BE4DBAC
P 8650 1100
F 0 "U3" H 8650 1475 50  0000 C CNN
F 1 "MCP-4151" H 8650 1384 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 8650 1450 50  0001 C CNN
F 3 "" H 8650 1450 50  0001 C CNN
	1    8650 1100
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:KF0602D K1
U 1 1 5BE4D712
P 5350 1100
F 0 "K1" H 5628 1151 50  0000 L CNN
F 1 "KF0602D" H 5628 1060 50  0000 L CNN
F 2 "kiyoshigawa_lib:KF0602D" H 5450 1350 50  0001 C CNN
F 3 "" H 5450 1350 50  0001 C CNN
	1    5350 1100
	1    0    0    -1  
$EndComp
$Comp
L kiyoshigawa_lib:MCP-4151 U2
U 1 1 5BE4C4C7
P 7050 1100
F 0 "U2" H 7050 1475 50  0000 C CNN
F 1 "MCP-4151" H 7050 1384 50  0000 C CNN
F 2 "Housings_DIP:DIP-8_W7.62mm_LongPads" H 7050 1450 50  0001 C CNN
F 3 "" H 7050 1450 50  0001 C CNN
	1    7050 1100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
