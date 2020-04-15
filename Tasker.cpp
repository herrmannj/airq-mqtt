#include "Tasker.h"

// defaults to 16
Tasker::Tasker(uint8_t size)
{
	_task = new task_t[size];
	_size = size;
};

void Tasker::add(uint8_t identifier, unsigned long interval, callback_t callback)
{
	if (_size > identifier) {
		_task[identifier].inUse = true;
		_task[identifier].interval = interval;
		_task[identifier].callback = callback;
	};
};

void Tasker::run(void)
{
	char buf[100];
	unsigned long currentMillis = millis();

	for (uint8_t i=0; i<_size; i++) {
		if (_task[i].inUse) {
			if ((currentMillis - _task[i].last) > _task[i].interval) {
				//sprintf(buf, "found: %u with: %lu", i, _task[i].interval);
				//Serial.println(buf);
				unsigned long result = _task[i].callback(i);
				_task[i].last = currentMillis;
				//disable
				if (result == 0) {
					_task[i].inUse = false;
				} else {
					_task[i].interval = result;
				};
			};
		};
	};
  
};
