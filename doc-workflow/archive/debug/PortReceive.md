[RTOS] create queue_activation OK
[RTOS] create queue_sensor_data OK
[RTOS] create queue_cloud_cmd OK
[RTOS] create Task_StateMachi OK
[RTOS] create Task_4G_MQTT OK
[RTOS] create Task_I2C_Sensor OK
[RTOS] create Task_GPS OK
[RTOS] create Task_Flash OK
[RTOS] create Task_Alarm OK
[Air780E] init start (HAL UART, USART1 PA9/PA10)
[Power] Air780E power-on (PWRKEY low 1.5s)
[Power] Air780E boot wait 5s
[StateMachine] init -> SLEEP
[SM] state_machine done
[Alarm] init OK (LD2 yellow heartbeat, buzzer PC8)
[SM] alarm done
[Power] init OK (Stop Mode stub, Air780E PWRKEY GPIO ready)
[SM] power done
[StateMachine] === AUTO-START full flow ===
[StateMachine] activation event=1
[StateMachine] SLEEP -> NFC_ACTIVE
[StateMachine] NFC_ACTIVE -> GPS_LOCATE
[Air780E] >> AT
[Air780E] << timeout/no match
[Air780E] >> ATE0
[Air780E] << ATE0

OK
[Air780E] >> AT+CSQ
[Air780E] << 

+CSQ: 26,0

OK
[Air780E] >> AT+CREG?
[Air780E] << 

+CREG: 0,1

OK
[Air780E] >> AT+CGREG?
[Air780E] << 

+CGREG: 0,1

OK
[Air780E] >> AT+CEREG?
[Air780E] << 

+CEREG: 0,1

OK
[Air780E] >> AT+CGDCONT=1,"IP","cmnet"
[Air780E] << 

OK
[Air780E] >> AT+CGATT?
[Air780E] << 

+CGATT: 1
[Air780E] init ready, CSQ=26
[MQTT] init: Air780E TCP MQTT stack
[MQTT] >> AT+CIPSTART="TCP","broker.emqx.io",1883
[MQTT] << 

OK

CONNECT
[MQTT] sending MQTT CONNECT (29 bytes)
[MQTT] >> AT+CIPSEND=29
[MQTT] <<  
>
[MQTT] waiting CONNACK...
[MQTT] CONNACK drain: 4 bytes
[MQTT] >> AT+CIPSEND=16
[MQTT] << 

>
[MQTT] subscribed cargo/cmd
[MQTT] init OK (TCP MQTT, broker.emqx.io:1883)
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] GPS timeout
[StateMachine] GPS_LOCATE -> ENV_SAMPLE
[MQTT] << timeout
[GPS] init OK (HAL UART, ATGM336H on USART2 PD5/PD6)
[GPS] sample lat=31 lon=121
[SHT31] init OK (I2C1 PB8/PB9, addr=0x44)
[I2C Task] SHT31 done, starting PN532...
[PN532] init OK (I2C1 PB8/PB9, addr=0x24)
[I2C Task] PN532 done
[StateMachine] ENV_SAMPLE -> UPLOAD
[MQTT] publish {"temp":24,"hum":51,"lat":0,"lon":0,"gps_valid":0} (69 bytes)
[MQTT] >> AT+CIPSEND=69
[MQTT] << 
>
[StateMachine] UPLOAD -> WAIT_CLOUD
[I2C] SHT31 sample T=24C H=51%
[W25Q128] JEDEC ID: EF 40 18
[W25Q128] cache task idle
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] cloud action: NONE
[StateMachine] WAIT_CLOUD -> RETURN_SLEEP
[Power] Air780E power-off (PWRKEY low 1.5s)
[MQTT] << timeout
[Power] enter sleep stub, waiting next activation
[StateMachine] RETURN_SLEEP -> SLEEP
[StateMachine] waiting activation
[MQTT] >> AT+CIPRXGET=2,256
[MQTT] << timeout
[W25Q128] cache task idle
[StateMachine] waiting activation
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] waiting activation
[MQTT] << timeout
[W25Q128] cache task idle
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] waiting activation
[MQTT] << timeout
[StateMachine] waiting activation
[MQTT] >> AT+CIPRXGET=2,256
[MQTT] << timeout
[W25Q128] cache task idle
[StateMachine] waiting activation
[MQTT] >> AT+CIPRXGET=2,256
[MQTT] << timeout
[StateMachine] waiting activation
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] waiting activation
[MQTT] << timeout
[W25Q128] cache task idle
[MQTT] >> AT+CIPRXGET=2,256
[StateMachine] waiting activation
[MQTT] << timeout