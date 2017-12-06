#pragma once

#ifndef _BA_FrameTimeC_H_
#define _BA_FrameTimeC_H_

#include <armadillo>
using namespace arma;

mat FrameTimeC(int frameNum, int framelen, int inc, int fs) {
	mat frameTime(1, frameNum);
	for (int i = 1; i <= frameNum; i++) {
		double k = i;
		frameTime(i - 1) = ((k - 1)*inc + framelen / 2) / fs;
	}
	return frameTime;
}

#endif
#pragma once