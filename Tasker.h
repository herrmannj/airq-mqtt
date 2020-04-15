#ifndef __TASKER_H__
#define __TASKER_H__

#include "Arduino.h"

typedef unsigned long (*callback_t)(uint8_t);  

struct task_t
{
	bool inUse = false;
	unsigned long interval;
	unsigned long last;
	callback_t callback;
};

class Tasker
{
	public:
		Tasker(uint8_t size = 16);
		void add(uint8_t identifier, unsigned long interval, callback_t callback);
		void run(void);
		unsigned long prevMillis;
	protected:
		task_t* _task = NULL;
		uint8_t _size = 0;
};

#endif