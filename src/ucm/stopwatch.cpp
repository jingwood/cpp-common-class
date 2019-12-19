///////////////////////////////////////////////////////////////////////////////
//  Common classes for cross-platform C++ application development.
//
//  MIT License
//  Copyright © 2016-2019 Jingwood, unvell.com, all rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include "stopwatch.h"

#if defined(_WIN32)
//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable:4996)

int gettimeofday(timeval* tp, struct timezone* tzp) {
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);
	
	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;
	
	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;
	
	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	
	return 0;
}

#endif // defined(_WIN32)

namespace ucm {

void Stopwatch::start() {
	gettimeofday(&this->startTime, NULL);
	this->running = true;
}

void Stopwatch::stop() {
	this->running = false;
	gettimeofday(&this->endTime, NULL);
	this->refreshElapsedSeconds(this->endTime);
}

void Stopwatch::refreshElapsedSeconds(const timeval& tv) {
	this->elapsedSeconds = (tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0 - this->startTime.tv_sec * 1000.0 - this->startTime.tv_usec / 1000.0) / 1000.0;
}

double Stopwatch::getElapsedSeconds() {
	if (this->running) {
		timeval tmpTime;
		gettimeofday(&tmpTime, NULL);
		this->refreshElapsedSeconds(tmpTime);
	}
	return this->elapsedSeconds;
}

Stopwatch Stopwatch::startNew() {
	Stopwatch sw;
	sw.start();
	return sw;
}

}