#ifndef BATCONTROLLER_FIRMWARE_DOUBLEBUFFER_H
#define BATCONTROLLER_FIRMWARE_DOUBLEBUFFER_H

#include "../Model.hpp"
#include <Display/Color.h>
#include <array>

template<typename T>
class DoubleBuffer {
public:
	DoubleBuffer() : current(&buf0), next(&buf1){}

	T* getWrite() const{
		return next;
	}

	T* getRead() const{
		return current;
	}

	void swap(){
		std::swap(next, current);
	}

private:
	T buf0, buf1;
	//current - reading, next - writing
	T* current, * next;
};


#endif //BATCONTROLLER_FIRMWARE_DOUBLEBUFFER_H
