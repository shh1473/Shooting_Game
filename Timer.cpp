#include "DXUT.h"
#include "Timer.h"
#include "App.h"

Timer::Timer() :
	isActivate(false),
	maxCount(0),
	nowCount(0),
	maxTime(0.0f),
	nowTime(0.0f),
	onCycle([]() {}),
	onFinish([]() {})
{
	app.timers.push_back(this);
}

Timer::~Timer()
{
	app.timers.remove(this);
}

void Timer::Update()
{
	nowTime += app.deltaTime;

	if (nowTime >= maxTime)
	{
		onCycle();

		nowTime = 0.0f;
		++nowCount;

		if (nowCount >= maxCount)
		{
			onFinish();

			nowCount = 0;
			isActivate = false;
		}
	}
}

void Timer::Start()
{
	isActivate = true;
}

void Timer::Pause()
{
	isActivate = false;
}

void Timer::Reset()
{
	nowCount = 0;
	nowTime = 0.0f;
}

void Timer::Stop()
{
	isActivate = false;
	nowCount = 0;
	nowTime = 0.0f;
}