#include "buffer.h"

static int bufferArray[BUFFER_SIZE];

Buffer::Buffer() {
    
}

void Buffer::set(unsigned int where, int value) {
    bufferArray[where] = value;
}

void Buffer::setWithIntegral(unsigned int where, int value) {
    bufferArray[where] += value;
}

void Buffer::reset(int where) {
    bufferArray[where] = 0;
}

int Buffer::get(unsigned int where) {
    return bufferArray[where];
}