#include "stdafx.h"
#include "FrameTimeC.h"

mat FrameTimeC(int frameNum, int framelen, int inc, int fs) {
	mat frameTime(1, frameNum);
	for (int i = 1; i <= frameNum; i++) {
		double k = i;
		frameTime(i - 1) = ((k - 1)*inc + framelen / 2) / fs;
	}
	return frameTime;
}