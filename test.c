#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#define BUFFER_SIZE 1024

typedef struct {
    char buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

// Initialize the circular buffer
void initBuffer(CircularBuffer* cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
}

// Check if the buffer is full
bool isFull(CircularBuffer* cb) {
    return cb->count == BUFFER_SIZE;
}

// Check if the buffer is empty
bool isEmpty(CircularBuffer* cb) {
    return cb->count == 0;
}

// Add an element to the buffer
bool writeBuffer(CircularBuffer* cb, char data) {
    if (isFull(cb)) {
        return false;  // Buffer is full, write operation fails
    }

    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;
    cb->count++;
    return true;
}

// Read an element from the buffer (non-blocking)
bool readBuffer(CircularBuffer* cb, char* data) {
    if (isEmpty(cb)) {
        return false;  // Buffer is empty, read operation fails
    }

    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    cb->count--;
    return true;
}

// Read line from the circular buffer
bool myreadline(CircularBuffer* cb, char* line, int maxLength) {
    int bytesRead = 0;
    char ch;
    while (bytesRead < maxLength - 1) {
        if (!readBuffer(cb, &ch)) {
            return false;  // No complete line available yet
        }
        line[bytesRead++] = ch;
        if (ch == '\n') {
            break;
        }
    }
    line[bytesRead] = '\0';
    return true;
}

void setNonBlocking(int fd) {
    
    if (ioctl(fd, FIONBIO, 0) == -1)
    {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
}

int main() {
    CircularBuffer cb;
    initBuffer(&cb);

    // Set stdin to non-blocking mode
    setNonBlocking(STDIN_FILENO);
    char line[BUFFER_SIZE];
    char ch;
    int bytesRead;

    printf("Enter text (Ctrl+D to stop):\n");

    // Read input from stdin and write to the buffer
    while (1) {
        // Non-blocking read from stdin
        while ((bytesRead = read(STDIN_FILENO, &ch, 1)) > 0) {
            if (writeBuffer(&cb, ch)) {
                printf("Written: %c\n", ch);
            } else {
                printf("Buffer is full, cannot write: %c\n", ch);
            }

            // Attempt to read a line from the buffer
            if (myreadline(&cb, line, BUFFER_SIZE)) {
                printf("Read line: %s", line);
            }
        }

        if (bytesRead == -1 && errno != EAGAIN) {
            perror("read");
            break;
        }

        // Simulate other processing
        usleep(100000);  // Sleep for 100 ms
    }

    printf("End of input.\n");

    return 0;
}
