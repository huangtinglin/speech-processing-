#ifndef _BA_pitch_Ceps_H_
#define _BA_pitch_Ceps_H_
#include "enframe.h"
#include "pitch_vad.h"
#include "hamming.h"
#include "IFFT.h"

extern mat period;

mat fix(mat a);

void pitch_Ceps(mat x, mat wnd, int inc, double T1, double fs, int miniL);

#endif
#pragma once
