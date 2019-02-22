crystal 8Mhz,timetick=1ms
STM32F407ZET6 setting pin
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|I/O | Diration | Function |I/O  |Diration | Function |I/O  |Diration  |Function |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE4 |  input   |  KEY0    |PA11 |output   | USB_DM   |PC8  | output   | SDIO_D0 |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE3 |  input   |  KEY1    |PA12 |output   | USB_DP   |PC9  | output   | SDIO_D1 |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE2 |  input   |  KEY2    |PA15 |output   | USP_PWR  |PC10 | output   | SDIO_D2 |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PA0 |  output  |  KEY3    |PB10 |output   | UART3_TX |PC11 | output   | SDIO_D3 |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PF8 |  output  |  BEEP    |PB11 |output   | UART3_RX |PC12 | output   | SDIO_SCK|
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PF9 |  output  |  Led0    |PA9  |output   | UART1_TX |     |          |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PF10|  output  |  Led1    |PA10 |output   | UART1_RX |     |          |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
Lcd Pin,driver NT35510,
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PD14|  output  |  FSMC_D0 |PE11 |output   | FSMC_D8  |PB15 | output   |LCD_BL   |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PD15|  output  |  FSMC_D1 |PE12 |output   | FSMC_D9  |PD4  | output   |FSMC_NOE |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PD0 |  output  |  FSMC_D2 |PE13 |output   | FSMC_D10 |PD5  | output   |FSMC_NWE |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PD1 |  output  |  FSMC_D3 |PE14 |output   | FSMC_D11 |PF12 | output   |FSMC_A6  |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE7 |  output  |  FSMC_D4 |PE15 |output   | FSMC_D12 |PG12 | output   |FSMC_NE4 |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE8 |  output  |  FSMC_D5 |PD8  |output   | FSMC_D13 |     | output   |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE9 |  output  |  FSMC_D6 |PD9  |output   | FSMC_D14 |     | output   |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PE10|  output  |  FSMC_D7 |PD10 |output   | FSMC_D15 |     | output   |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
touch key Pin, driver GT9147
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PB0 |  output  |  T_SCK   |PE11 |output   | T_MOSI   |PB2  | input    |T_MISO   |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
|PB1 |  output  |  T_PEN   |PC13 |output   | T_CS     |     |          |         |
+----+----------+----------+-----+---------+----------+-----+----------+---------+
2019年 02月 22日 星期五 11:23:34 CST
1. update makefile (program debug release ....)

2019年 01月 31日 星期四 11:03:01 CST
1.touch key in lcd ok
2.driver is GT9147 need update touch.c

2019年 01月 29日 星期二 11:52:44 CST
1. lcd and fsmc working ok

2019年 01月 23日 星期三 13:06:22 CST
1. led blinking ok
2. key debouce  ok

2018年 12月 14日 星期五 14:02:52 CST
1. fatfs usb host msc ok
2. freeRTOS ok

2018年 12月 14日 星期五 11:50:09 CST
1. fatfs usb host msc ok
2. freeRTos not ok

2018年 11月 12日 星期一 14:53:49 CST
1. sd driver ok
2. fatfs operate ok

2018年 11月 06日 星期二 14:22:58 CST
1. explorer board basic test ok
2. led blink ok
3. key trigger signal and queue ok
4. uart printf ok
5. freertos test ok
