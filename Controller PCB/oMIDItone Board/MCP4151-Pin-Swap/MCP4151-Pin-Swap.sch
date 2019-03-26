EESchema Schematic File Version 4
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
L kiyoshigawa_lib:MCP-4151 U1
U 1 1 5C383CC7
P 8600 5500
F 0 "U1" H 8600 5875 50  0000 C CNN
F 1 "MCP-4151" H 8600 5784 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 8600 5850 50  0001 C CNN
F 3 "" H 8600 5850 50  0001 C CNN
	1    8600 5500
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-Connectors:CONN_04 J1
U 1 1 5C383DC0
P 7900 5650
F 0 "J1" H 7856 6260 45  0000 C CNN
F 1 "CONN_04" H 7856 6176 45  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 7900 6150 20  0001 C CNN
F 3 "" H 7900 5650 50  0001 C CNN
F 4 "CONN-09696" H 7856 6081 60  0000 C CNN "Field4"
	1    7900 5650
	1    0    0    -1  
$EndComp
$Comp
L SparkFun-Connectors:CONN_04 J2
U 1 1 5C383E0D
P 9300 5350
F 0 "J2" H 9072 5405 45  0000 R CNN
F 1 "CONN_04" H 9072 5489 45  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9300 5850 20  0001 C CNN
F 3 "" H 9300 5350 50  0001 C CNN
F 4 "CONN-09696" H 9072 5584 60  0000 R CNN "Field4"
	1    9300 5350
	-1   0    0    1   
$EndComp
Wire Wire Line
	9100 5350 9200 5350
Wire Wire Line
	9100 5450 9200 5450
Wire Wire Line
	9100 5550 9200 5550
Wire Wire Line
	9100 5650 9200 5650
Wire Wire Line
	8100 5650 8000 5650
Wire Wire Line
	8000 5550 8100 5550
Wire Wire Line
	8100 5450 8000 5450
Wire Wire Line
	8000 5350 8100 5350
$EndSCHEMATC
