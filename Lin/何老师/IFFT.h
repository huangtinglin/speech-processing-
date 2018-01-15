#pragma once
#ifndef _IFFT_
#define _IFFT_

#include<armadillo>
#include"Complex.h"
using namespace arma;
#define PI 3.1415926

int Pow(char BaseNumber, int IndexNumber);

int Log(char BaseNumber, int AntiNumber);

void Invert_order(int N, Complex *x);

void Ifft1(int N, Complex *x);

void Dit2Fft(int N, Complex *x);

void Ifft2(int N, Complex *x);//2¡¢x(n)=IFFT[x(k)]=1/N*{FFT[x*(k)]}*

mat IFFT(mat m);

#endif