#pragma once

#include "Common.h"

class Timer
{
public:
	Timer();
	~Timer();

	void Update();

	void Start();
	void Pause();
	void Reset();
	void Stop();

public:
	bool isActivate;
	unsigned maxCount;
	unsigned nowCount;
	float maxTime;
	float nowTime;
	function<void()> onCycle;
	function<void()> onFinish;

};