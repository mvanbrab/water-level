EESchema Schematic File Version 4
LIBS:receiver-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Water level receiver"
Date "2020-07-08"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:C C4
U 1 1 5E5611A9
P 3850 2400
F 0 "C4" H 3965 2446 50  0000 L CNN
F 1 "100n" H 3965 2355 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 3888 2250 50  0001 C CNN
F 3 "~" H 3850 2400 50  0001 C CNN
	1    3850 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 5E561C4A
P 4550 3000
F 0 "C5" H 4665 3046 50  0000 L CNN
F 1 "100n" H 4665 2955 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 4588 2850 50  0001 C CNN
F 3 "~" H 4550 3000 50  0001 C CNN
	1    4550 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5E562632
P 2550 3000
F 0 "C1" H 2665 3046 50  0000 L CNN
F 1 "100n" H 2665 2955 50  0000 L CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 2588 2850 50  0001 C CNN
F 3 "~" H 2550 3000 50  0001 C CNN
	1    2550 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5E562C2E
P 2600 3350
F 0 "C2" V 2500 3000 50  0000 C CNN
F 1 "100n" V 2600 2950 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 2638 3200 50  0001 C CNN
F 3 "~" H 2600 3350 50  0001 C CNN
	1    2600 3350
	0    1    1    0   
$EndComp
$Comp
L Device:C C3
U 1 1 5E5633AB
P 2600 3650
F 0 "C3" V 2550 3300 50  0000 C CNN
F 1 "100n" V 2650 3250 50  0000 C CNN
F 2 "Capacitor_THT:C_Rect_L7.0mm_W2.0mm_P5.00mm" H 2638 3500 50  0001 C CNN
F 3 "~" H 2600 3650 50  0001 C CNN
	1    2600 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	3550 2550 3550 2250
Wire Wire Line
	3550 2250 3850 2250
$Comp
L power:GND #PWR05
U 1 1 5E5A5340
P 3850 2550
F 0 "#PWR05" H 3850 2300 50  0001 C CNN
F 1 "GND" H 3855 2377 50  0000 C CNN
F 2 "" H 3850 2550 50  0001 C CNN
F 3 "" H 3850 2550 50  0001 C CNN
	1    3850 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 2850 2750 2850
Wire Wire Line
	2550 3150 2750 3150
Wire Wire Line
	4350 2850 4550 2850
Wire Wire Line
	4350 3150 4550 3150
$Comp
L power:GND #PWR02
U 1 1 5E5B4B87
P 2450 3650
F 0 "#PWR02" H 2450 3400 50  0001 C CNN
F 1 "GND" H 2455 3477 50  0000 C CNN
F 2 "" H 2450 3650 50  0001 C CNN
F 3 "" H 2450 3650 50  0001 C CNN
	1    2450 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5E5B5306
P 2450 3350
F 0 "#PWR01" H 2450 3100 50  0001 C CNN
F 1 "GND" H 2455 3177 50  0000 C CNN
F 2 "" H 2450 3350 50  0001 C CNN
F 3 "" H 2450 3350 50  0001 C CNN
	1    2450 3350
	1    0    0    -1  
$EndComp
$Comp
L Interface_UART:MAX3232 U1
U 1 1 5E559341
P 3550 3750
F 0 "U1" H 4100 4950 50  0000 C CNN
F 1 "MAX3232" H 4000 4850 50  0000 C CNN
F 2 "Package_DIP:DIP-16_W7.62mm_LongPads" H 3600 2700 50  0001 L CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX3222-MAX3241.pdf" H 3550 3850 50  0001 C CNN
	1    3550 3750
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5E6151F3
P 3550 4950
F 0 "#PWR04" H 3550 4700 50  0001 C CNN
F 1 "GND" H 3555 4777 50  0000 C CNN
F 2 "" H 3550 4950 50  0001 C CNN
F 3 "" H 3550 4950 50  0001 C CNN
	1    3550 4950
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5E56F38B
P 6250 6850
F 0 "#FLG01" H 6250 6925 50  0001 C CNN
F 1 "PWR_FLAG" H 6250 7023 50  0000 C CNN
F 2 "" H 6250 6850 50  0001 C CNN
F 3 "~" H 6250 6850 50  0001 C CNN
	1    6250 6850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5E56FE79
P 6250 6950
F 0 "#PWR012" H 6250 6700 50  0001 C CNN
F 1 "GND" H 6255 6777 50  0000 C CNN
F 2 "" H 6250 6950 50  0001 C CNN
F 3 "" H 6250 6950 50  0001 C CNN
	1    6250 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 6950 6250 6850
NoConn ~ 2750 4450
NoConn ~ 4350 4450
NoConn ~ 4350 4050
NoConn ~ 14350 800 
NoConn ~ 15800 750 
$Comp
L Mechanical:MountingHole H2
U 1 1 5E5B6257
P 4900 6900
F 0 "H2" H 5000 6946 50  0000 L CNN
F 1 "MountingHole" H 5000 6855 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4900 6900 50  0001 C CNN
F 3 "~" H 4900 6900 50  0001 C CNN
	1    4900 6900
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5E5B6F64
P 4900 7200
F 0 "H3" H 5000 7246 50  0000 L CNN
F 1 "MountingHole" H 5000 7155 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4900 7200 50  0001 C CNN
F 3 "~" H 4900 7200 50  0001 C CNN
	1    4900 7200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5E5B7762
P 4900 7500
F 0 "H4" H 5000 7546 50  0000 L CNN
F 1 "MountingHole" H 5000 7455 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4900 7500 50  0001 C CNN
F 3 "~" H 4900 7500 50  0001 C CNN
	1    4900 7500
	1    0    0    -1  
$EndComp
NoConn ~ 2750 4050
Wire Wire Line
	2650 4150 2650 3850
Wire Wire Line
	2650 3850 2750 3850
$Comp
L Mechanical:MountingHole H1
U 1 1 5E5B5B7F
P 4900 6600
F 0 "H1" H 5000 6646 50  0000 L CNN
F 1 "MountingHole" H 5000 6555 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 4900 6600 50  0001 C CNN
F 3 "~" H 4900 6600 50  0001 C CNN
	1    4900 6600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 5EF7A827
P 8700 3400
F 0 "#PWR018" H 8700 3150 50  0001 C CNN
F 1 "GND" H 8705 3227 50  0000 C CNN
F 2 "" H 8700 3400 50  0001 C CNN
F 3 "" H 8700 3400 50  0001 C CNN
	1    8700 3400
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR013
U 1 1 5EF7C2FC
P 6550 2800
F 0 "#PWR013" H 6550 2650 50  0001 C CNN
F 1 "+3.3V" H 6565 2973 50  0000 C CNN
F 2 "" H 6550 2800 50  0001 C CNN
F 3 "" H 6550 2800 50  0001 C CNN
	1    6550 2800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR03
U 1 1 5EF81D51
P 3550 2250
F 0 "#PWR03" H 3550 2100 50  0001 C CNN
F 1 "+3.3V" H 3565 2423 50  0000 C CNN
F 2 "" H 3550 2250 50  0001 C CNN
F 3 "" H 3550 2250 50  0001 C CNN
	1    3550 2250
	1    0    0    -1  
$EndComp
Connection ~ 3550 2250
$Comp
L power:+5V #PWR015
U 1 1 5EF87CD0
P 6550 4600
F 0 "#PWR015" H 6550 4450 50  0001 C CNN
F 1 "+5V" H 6565 4773 50  0000 C CNN
F 2 "" H 6550 4600 50  0001 C CNN
F 3 "" H 6550 4600 50  0001 C CNN
	1    6550 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5EF882A8
P 9600 5950
F 0 "J6" H 9680 5896 50  0000 L CNN
F 1 "Conn_01x04" H 9680 5851 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 9600 5950 50  0001 C CNN
F 3 "~" H 9600 5950 50  0001 C CNN
	1    9600 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 5950 9400 5950
Wire Wire Line
	9400 6050 9300 6050
Wire Wire Line
	9400 6150 9300 6150
Wire Wire Line
	9300 6150 9300 6200
$Comp
L power:GND #PWR020
U 1 1 5EF8C6DE
P 9300 6200
F 0 "#PWR020" H 9300 5950 50  0001 C CNN
F 1 "GND" H 9305 6027 50  0000 C CNN
F 2 "" H 9300 6200 50  0001 C CNN
F 3 "" H 9300 6200 50  0001 C CNN
	1    9300 6200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR019
U 1 1 5EF8CDAB
P 9300 5800
F 0 "#PWR019" H 9300 5650 50  0001 C CNN
F 1 "+5V" H 9315 5973 50  0000 C CNN
F 2 "" H 9300 5800 50  0001 C CNN
F 3 "" H 9300 5800 50  0001 C CNN
	1    9300 5800
	1    0    0    -1  
$EndComp
Text Notes 9850 6100 0    50   ~ 0
Connect to I2C compatible\nHD44780 LCD display.\nWarning: I2C at 3V3!
Text GLabel 4750 4250 2    50   Input ~ 0
IO16
Text GLabel 4750 3850 2    50   Input ~ 0
IO17
$Comp
L power:GND #PWR014
U 1 1 5EF9CDCE
P 6550 4100
F 0 "#PWR014" H 6550 3850 50  0001 C CNN
F 1 "GND" H 6555 3927 50  0000 C CNN
F 2 "" H 6550 4100 50  0001 C CNN
F 3 "" H 6550 4100 50  0001 C CNN
	1    6550 4100
	1    0    0    -1  
$EndComp
Text GLabel 8800 5850 0    50   Input ~ 0
IO22
Wire Wire Line
	8800 5850 9400 5850
Wire Wire Line
	9300 6050 9300 5800
Text Label 9050 5850 0    50   ~ 0
SCL
Text Label 9050 5950 0    50   ~ 0
SDA
Text GLabel 8800 5950 0    50   Input ~ 0
IO21
Wire Wire Line
	4350 3850 4750 3850
Wire Wire Line
	4350 4250 4750 4250
Text Label 4450 3850 0    50   ~ 0
TXD2
Text Label 4450 4250 0    50   ~ 0
RXD2
Connection ~ 8700 3400
Wire Wire Line
	8700 2800 8700 3400
Wire Wire Line
	7050 4100 6550 4100
Text GLabel 8200 2900 2    50   Input ~ 0
IO23
Text GLabel 8200 3000 2    50   Input ~ 0
IO22
Text GLabel 8200 3100 2    50   Input ~ 0
IO1
Text GLabel 8200 3200 2    50   Input ~ 0
IO3
Text GLabel 8200 3300 2    50   Input ~ 0
IO21
Text GLabel 8200 3500 2    50   Input ~ 0
IO19
Text GLabel 8200 3600 2    50   Input ~ 0
IO18
Text GLabel 8200 3700 2    50   Input ~ 0
IO5
Text GLabel 8200 3800 2    50   Input ~ 0
IO17
Text GLabel 8200 3900 2    50   Input ~ 0
IO16
Text GLabel 8200 4000 2    50   Input ~ 0
IO4
Text GLabel 8200 4100 2    50   Input ~ 0
IO0
Text GLabel 8200 4200 2    50   Input ~ 0
IO2
Text GLabel 8200 4300 2    50   Input ~ 0
IO15
Text GLabel 8200 4400 2    50   Input ~ 0
IO8
Text GLabel 8200 4500 2    50   Input ~ 0
IO7
Text GLabel 8200 4600 2    50   Input ~ 0
IO6
Text GLabel 7050 4500 0    50   Input ~ 0
IO11
Text GLabel 7050 4400 0    50   Input ~ 0
IO10
Text GLabel 7050 4300 0    50   Input ~ 0
IO9
Text GLabel 7050 4200 0    50   Input ~ 0
IO13
Text GLabel 7050 4000 0    50   Input ~ 0
IO12
Text GLabel 7050 3900 0    50   Input ~ 0
IO14
Text GLabel 7050 3800 0    50   Input ~ 0
IO27
Text GLabel 7050 3700 0    50   Input ~ 0
IO26
Text GLabel 7050 3600 0    50   Input ~ 0
IO25
Text GLabel 7050 3500 0    50   Input ~ 0
IO33
Text GLabel 7050 3400 0    50   Input ~ 0
IO32
Text GLabel 7050 3300 0    50   Input ~ 0
IO35
Text GLabel 7050 3200 0    50   Input ~ 0
IO34
Text GLabel 7050 3100 0    50   Input ~ 0
IO39
Text GLabel 7050 3000 0    50   Input ~ 0
IO36
Text GLabel 7050 2900 0    50   Input ~ 0
EN
Wire Wire Line
	7050 4600 6550 4600
Wire Wire Line
	7050 2800 6550 2800
Wire Wire Line
	8200 2800 8700 2800
Wire Wire Line
	8200 3400 8700 3400
$Comp
L Connector_Generic:Conn_01x19 J3
U 1 1 5EFB5B61
P 6200 3700
F 0 "J3" H 6150 4800 50  0000 L CNN
F 1 "Conn_01x19" H 6280 3651 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x19_P2.54mm_Vertical" H 6200 3700 50  0001 C CNN
F 3 "~" H 6200 3700 50  0001 C CNN
	1    6200 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x19 J5
U 1 1 5EFB7EAC
P 9050 3700
F 0 "J5" H 9050 4800 50  0000 C CNN
F 1 "Conn_01x19" H 8968 4726 50  0001 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x19_P2.54mm_Vertical" H 9050 3700 50  0001 C CNN
F 3 "~" H 9050 3700 50  0001 C CNN
	1    9050 3700
	-1   0    0    -1  
$EndComp
Text GLabel 6000 2900 0    50   Input ~ 0
EN
Text GLabel 6000 3000 0    50   Input ~ 0
IO36
Text GLabel 6000 3100 0    50   Input ~ 0
IO39
Text GLabel 6000 3200 0    50   Input ~ 0
IO34
Text GLabel 6000 3300 0    50   Input ~ 0
IO35
Text GLabel 6000 3400 0    50   Input ~ 0
IO32
Text GLabel 6000 3500 0    50   Input ~ 0
IO33
Text GLabel 6000 3600 0    50   Input ~ 0
IO25
Text GLabel 6000 3700 0    50   Input ~ 0
IO26
Text GLabel 6000 3800 0    50   Input ~ 0
IO27
Text GLabel 6000 3900 0    50   Input ~ 0
IO14
Text GLabel 6000 4000 0    50   Input ~ 0
IO12
Text GLabel 6000 4200 0    50   Input ~ 0
IO13
Text GLabel 6000 4300 0    50   Input ~ 0
IO9
Text GLabel 6000 4400 0    50   Input ~ 0
IO10
Text GLabel 6000 4500 0    50   Input ~ 0
IO11
Text GLabel 9250 2900 2    50   Input ~ 0
IO23
Text GLabel 9250 3000 2    50   Input ~ 0
IO22
Text GLabel 9250 3100 2    50   Input ~ 0
IO1
Text GLabel 9250 3200 2    50   Input ~ 0
IO3
Text GLabel 9250 3300 2    50   Input ~ 0
IO21
Text GLabel 9250 3500 2    50   Input ~ 0
IO19
Text GLabel 9250 3600 2    50   Input ~ 0
IO18
Text GLabel 9250 3700 2    50   Input ~ 0
IO5
Text GLabel 9250 3800 2    50   Input ~ 0
IO17
Text GLabel 9250 3900 2    50   Input ~ 0
IO16
Text GLabel 9250 4000 2    50   Input ~ 0
IO4
Text GLabel 9250 4100 2    50   Input ~ 0
IO0
Text GLabel 9250 4200 2    50   Input ~ 0
IO2
Text GLabel 9250 4300 2    50   Input ~ 0
IO15
Text GLabel 9250 4400 2    50   Input ~ 0
IO8
Text GLabel 9250 4500 2    50   Input ~ 0
IO7
Text GLabel 9250 4600 2    50   Input ~ 0
IO6
Wire Wire Line
	6000 2800 5500 2800
Wire Wire Line
	6000 4100 5500 4100
Wire Wire Line
	6000 4600 5500 4600
Wire Wire Line
	9250 2800 9750 2800
Wire Wire Line
	9250 3400 9750 3400
Wire Wire Line
	9750 2800 9750 3400
$Comp
L power:GND #PWR021
U 1 1 5EFCF095
P 9750 3400
F 0 "#PWR021" H 9750 3150 50  0001 C CNN
F 1 "GND" H 9755 3227 50  0000 C CNN
F 2 "" H 9750 3400 50  0001 C CNN
F 3 "" H 9750 3400 50  0001 C CNN
	1    9750 3400
	1    0    0    -1  
$EndComp
Connection ~ 9750 3400
$Comp
L power:+3.3V #PWR09
U 1 1 5EFCF508
P 5500 2800
F 0 "#PWR09" H 5500 2650 50  0001 C CNN
F 1 "+3.3V" H 5515 2973 50  0000 C CNN
F 2 "" H 5500 2800 50  0001 C CNN
F 3 "" H 5500 2800 50  0001 C CNN
	1    5500 2800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EFCF9AB
P 5500 4100
F 0 "#PWR010" H 5500 3850 50  0001 C CNN
F 1 "GND" H 5505 3927 50  0000 C CNN
F 2 "" H 5500 4100 50  0001 C CNN
F 3 "" H 5500 4100 50  0001 C CNN
	1    5500 4100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR011
U 1 1 5EFCFE48
P 5500 4600
F 0 "#PWR011" H 5500 4450 50  0001 C CNN
F 1 "+5V" H 5515 4773 50  0000 C CNN
F 2 "" H 5500 4600 50  0001 C CNN
F 3 "" H 5500 4600 50  0001 C CNN
	1    5500 4600
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 5EFC788C
P 4350 5350
F 0 "J1" H 4430 5346 50  0000 L CNN
F 1 "Conn_01x03" H 4430 5301 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4350 5350 50  0001 C CNN
F 3 "~" H 4350 5350 50  0001 C CNN
	1    4350 5350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5EFCA217
P 4350 6050
F 0 "J2" H 4430 5996 50  0000 L CNN
F 1 "Conn_01x02" H 4430 5951 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4350 6050 50  0001 C CNN
F 3 "~" H 4350 6050 50  0001 C CNN
	1    4350 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6150 4100 6150
Wire Wire Line
	4100 6150 4100 6200
$Comp
L power:GND #PWR08
U 1 1 5EFCBA45
P 4100 6200
F 0 "#PWR08" H 4100 5950 50  0001 C CNN
F 1 "GND" H 4105 6027 50  0000 C CNN
F 2 "" H 4100 6200 50  0001 C CNN
F 3 "" H 4100 6200 50  0001 C CNN
	1    4100 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6050 4100 6050
Wire Wire Line
	4100 6050 4100 6000
$Comp
L power:+5V #PWR07
U 1 1 5EFCCD82
P 4100 6000
F 0 "#PWR07" H 4100 5850 50  0001 C CNN
F 1 "+5V" H 4115 6173 50  0000 C CNN
F 2 "" H 4100 6000 50  0001 C CNN
F 3 "" H 4100 6000 50  0001 C CNN
	1    4100 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 5450 4100 5450
Wire Wire Line
	4100 5450 4100 5500
$Comp
L power:GND #PWR06
U 1 1 5EFCDDD5
P 4100 5500
F 0 "#PWR06" H 4100 5250 50  0001 C CNN
F 1 "GND" H 4105 5327 50  0000 C CNN
F 2 "" H 4100 5500 50  0001 C CNN
F 3 "" H 4100 5500 50  0001 C CNN
	1    4100 5500
	1    0    0    -1  
$EndComp
Text GLabel 4150 5250 0    50   Input ~ 0
TXD_connector
Text GLabel 4150 5350 0    50   Input ~ 0
RXD_connector
Text GLabel 2550 4150 0    50   Input ~ 0
TXD_connector
Text GLabel 2550 4250 0    50   Input ~ 0
RXD_connector
Wire Wire Line
	2650 4150 2550 4150
Wire Wire Line
	2750 4250 2550 4250
Text Notes 4600 6150 0    50   ~ 0
External +5V supply\nDo not connect when USB is connected!!!
Text Notes 4600 5400 0    50   ~ 0
Connection to transmitter
Text Notes 5850 2500 0    50   ~ 0
Expansion\n(Optional)
Text Notes 9000 2500 0    50   ~ 0
Expansion\n(Optional)
Wire Wire Line
	6900 6150 6950 6150
Wire Wire Line
	6900 6200 6900 6150
$Comp
L power:GND #PWR017
U 1 1 5F053D7E
P 6900 6200
F 0 "#PWR017" H 6900 5950 50  0001 C CNN
F 1 "GND" H 6905 6027 50  0000 C CNN
F 2 "" H 6900 6200 50  0001 C CNN
F 3 "" H 6900 6200 50  0001 C CNN
	1    6900 6200
	1    0    0    -1  
$EndComp
Text Notes 7400 6150 0    50   ~ 0
Backlight button\n
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 5F0620AE
P 7150 6050
F 0 "J4" H 7230 5996 50  0000 L CNN
F 1 "Conn_01x02" H 7230 5951 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7150 6050 50  0001 C CNN
F 3 "~" H 7150 6050 50  0001 C CNN
	1    7150 6050
	1    0    0    -1  
$EndComp
$Comp
L MyLib:ESP32DevKitC U2
U 1 1 5EF78635
P 7400 2650
F 0 "U2" H 7625 2765 50  0000 C CNN
F 1 "ESP32DevKitC" H 7625 2674 50  0000 C CNN
F 2 "MyFootprints:DIP-38_W25.4mm" H 7400 2650 50  0001 C CNN
F 3 "" H 7400 2650 50  0001 C CNN
	1    7400 2650
	1    0    0    -1  
$EndComp
Text GLabel 6950 6050 0    50   Input ~ 0
IO12
$EndSCHEMATC
