#ifndef  HANNING
#define HANNING

#include<iostream>
#include<armadillo>
#include"meta.h"
#define pi 3.1415926
using namespace std;
using namespace arma;

mat hanning(int wlen) {
	if (wlen % 2 == 0) {
		int half = wlen / 2;
		mat p = meta(half);
		mat w(3, half + 1);
		for (int i = 1; i <= half; i++) {
			double t = 0.5*(1 - cos(2 * pi*p(1, i) / (wlen + 1)));
			w(1, i) = t;
			w(2, half - i + 1) = t;
		}
		int p_res = 0;
		mat res(1, wlen);
		for (int j = 1; j <= 2; j++)
			for (int i = 1; i <= half; i++) {
				if (p_res < wlen) {
					res(p_res++) = w(j, i);
				}
				if (p_res == wlen) {
					return res;
				}
			}
	}
	else {
		int half = (wlen + 1) / 2;
		mat p = meta(half);
		mat w(3, half + 1);
		for (int i = 1; i <= half; i++) {
			double t = 0.5*(1 - cos(2 * pi*p(1, i) / (wlen + 1)));
			w(1, i) = t;
			w(2, half - i) = t;
		}
		int p_res = 0;
		mat res(1, wlen);
		for (int j = 1; j <= 2; j++)
			for (int i = 1; i <= half; i++)
				if (p_res < wlen)
					res(0, p_res++) = w(j, i);
				else
					return res;
	}
}



#endif // ! HANNING

#pragma once
