#pragma once
#ifndef _BA_FrameTimeC_H_
#define _BA_FrameTimeC_H_

#include <armadillo>
using namespace arma;

mat FrameTimeC(int frameNum, int framelen, int inc, int fs);

#endif
#pragma once