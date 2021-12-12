#pragma once
#define MAXSIZE 8
using namespace std;


class CircularBuffer {
private:
	char buffer[MAXSIZE];
	int start;
	int stop;
	int size;
	int maxSize;

public:
	CircularBuffer();
	bool isEmpty();
	bool isFull();
	void addByte(char info);
	char removeByte();
};

CircularBuffer::CircularBuffer() {
	start = 0;
	stop = 0;
	size = 0;
	maxSize = MAXSIZE;
}

bool CircularBuffer::isFull() {
	bool result;
	result = (size == maxSize) ? true : false;
	return result;
}

bool CircularBuffer::isEmpty() {
	bool result;

	result = (size == 0) ? true : false;
	return result;
}

void CircularBuffer::addByte(char info) {
	buffer[start] = info;
	start = (start == maxSize - 1) ? 0 : start + 1;
	size++;
}

char CircularBuffer::removeByte() {
	char result;
	result = buffer[stop];
	stop = (stop == maxSize - 1) ? 0 : stop + 1;
	size--;
	return result;
}

