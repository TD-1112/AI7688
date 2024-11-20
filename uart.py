import serial
import time

# Cấu hình UART
ser = serial.Serial(
    port='/dev/ttyUSB0',       # Thay COM3 bằng cổng UART của bạn (Linux có thể là /dev/ttyUSB0)
    baudrate=9600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

# Gửi dữ liệu
while True:
    while True:
        message = "hello"
        ser.write(message.encode('utf-8'))  # Gửi chuỗi
        print(f"Sent: {message}")

        # Nhận lại dữ liệu từ AI7688
        response = ser.readline().decode('utf-8').strip()
        if response:
            print(f"Received: {response}")

        time.sleep(1)  # Chờ 1 giây trước khi gửi lại
