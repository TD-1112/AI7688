import serial
import time


ser = serial.Serial(
    port='/dev/ttyUSB0',       
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)


while True:
    while True:
        message = "hello"
        ser.write(message.encode('utf-8'))  
        print(f"Sent: {message}")

        response = ser.readline().decode('utf-8').strip()
        if response:
            print(f"Received: {response}")

        time.sleep(1) 
