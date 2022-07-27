#include "headfile.h"

float FastSqrtI(float x)
{
	//////////////////////////////////////////////////////////////////////////
	//less accuracy, more faster
	/*
	L2F l2f;
	float xhalf = 0.5f * x;
	l2f.f = x;

	l2f.i = 0x5f3759df - (l2f.i >> 1);
	x = l2f.f * (1.5f - xhalf * l2f.f * l2f.f);
	return x;
	*/
	//////////////////////////////////////////////////////////////////////////
	union { unsigned int i; float f;} l2f;
	l2f.f = x;
	l2f.i = 0x5F1F1412 - (l2f.i >> 1);
	return l2f.f * (1.69000231f - 0.714158168f * x * l2f.f * l2f.f);
}

float FastSqrt(float x)
{
	return x * FastSqrtI(x);
}

