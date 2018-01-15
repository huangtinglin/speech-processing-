#ifndef _BA_FFT_H_
#define _BA_FFT_H_
#include "Complex.h"
#include <cmath>

// 对数计算（以2为底）
int LOG(int n);

// 旋转因子的生成
void GenerateRotationFactor(int size);

// 复数数组复制
void ArrayCopy(Complex dest[], Complex src[], int size);

// 码位倒置
void BitReverse(int inIndex[], int indexSize);

// 快速傅立叶变换
void FFT(Complex IN[], Complex OUT[], int Size);

mat FFT(mat m);

#endif
#pragma once
