#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>  
#include <thread>   
#include <chrono>   

#define BUFFER_SIZE 256

void configureUART(int uart1_filestream, speed_t baudrate);
void sendStringUART(int uart1_filestream, const char *message);
void receiveStringUART(int uart1_filestream, char *buffer, int buffer_size);

int main() {
    int uart1_filestream = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart1_filestream == -1) {
        std::cerr << "can't open uart port\n";
        return -1;
    }

    configureUART(uart1_filestream, B9600);

    char buffer[BUFFER_SIZE];

    while (true) {
        receiveStringUART(uart1_filestream, buffer, BUFFER_SIZE);
        if (buffer[0] != '\0')
        { 
            std::cout << "received: " << buffer << std::endl;

            sendStringUART(uart1_filestream, buffer);

            memset(buffer, 0, BUFFER_SIZE);
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    close(uart1_filestream);

    return 0;
}

void configureUART(int uart1_filestream, speed_t baudrate) {
    struct termios options;
    if (tcgetattr(uart1_filestream, &options) != 0) {
        std::cerr << "error when config uart\n";
        return;
    }

    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);

    options.c_cflag = CS8 | CLOCAL | CREAD; // 8 bits, no parity, 1 stop bit
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart1_filestream, TCIFLUSH);
    if (tcsetattr(uart1_filestream, TCSANOW, &options) != 0) {
        std::cerr << "error when config UART\n";
    }
}

void sendStringUART(int uart1_filestream, const char *message) {
    if (uart1_filestream != -1) {
        int count = write(uart1_filestream, message, strlen(message));
        if (count < 0) {
            std::cerr << "error when try to send data using uart\n";
        } else {
            std::cout << "send: " << message << std::endl;
        }
    }
}

void receiveStringUART(int uart1_filestream, char *buffer, int buffer_size) {
    int rx_length = read(uart1_filestream, buffer, buffer_size - 1);
    if (rx_length < 0)
    {
        if (errno != EAGAIN)
        { 
            std::cerr << "error when try to receive data using uart \n";
        }
        buffer[0] = '\0';
    }
    else if (rx_length > 0) 
    {
        buffer[rx_length] = '\0'; 
    }
    else 
    {
        buffer[0] = '\0'; 
    }
}
