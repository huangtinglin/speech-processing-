#pragma once
#ifndef _BA_pitch_vad_H_
#define _BA_pitch_vad_H_

#include <cmath>
#include <armadillo>
#include "enframe.h"
#include "fft.h"
#include "findSegment.h"
using namespace arma;

extern int vosl;
extern mat Ef;
extern mat SF;
extern Segment voiceseg;

void pitch_vad(mat x, mat wnd, int inc, double T1, int miniL);

#endif
#pragma once
