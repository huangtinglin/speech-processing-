#ifndef _BA_pitch_Ceps_H_
#define _BA_pitch_Ceps_H_
#include "enframe.h"
#include "pitch_vad.h"
#include "hamming.h"
#include "IFFT.h"

mat period;

mat fix(mat a)
{
	mat result(a.n_rows, a.n_cols);
	int i, j;
	for (i = 0; i < a.n_rows; i++)
		for (j = 0; j < a.n_cols; j++)
		{
			if (a(i, j) >= 0)
				result(i, j) = floor(a(i, j));
			else
			{
				double b = floor(-1 * a(i, j));
				result(i, j) = -1 * b;
			}
		}
	return result;
}

void pitch_Ceps(mat x, mat wnd, int inc, double T1, double fs, int miniL)
{
	int k, i, j;
	int wlen;
	if (miniL == -1)
		miniL = 10;
	if (wnd.n_elem == 1)
		wlen = wnd(0);
	else
		wlen = wnd.n_elem;

	mat y = enframe(x, wnd, inc).t();
	pitch_vad(x, wnd, inc, T1, miniL);
	int fn = SF.n_elem;
	double lmin = floor(fs / 500);
	double lmax = floor(fs / 60);
	period.set_size(1, fn);
	for (k = 1; k <= fn; k++)
	{
		if (SF(k - 1) == 1)
		{
			mat y1 = y.cols(k - 1, k - 1) % hamming(wlen);
			mat y2(2, y1.n_elem);
			y2.fill(0);

			for (j = 0; j < y1.n_elem; j++)
				y2(0, j) = y1(j);


			mat xx1 = FFT(y2);
			mat xx(xx1.n_cols, 1);
			
			for (j = 0; j < xx1.n_cols; j++) {
				double sum = xx1(0, j)*xx1(0, j) + xx1(1, j)*xx1(1, j);
				xx(j) = sqrt(sum);
			}
			mat a = 2 * log(xx + 2.2204e-16);

			mat a2(2, a.n_elem);
			a2.fill(0);

			for (j = 0; j < a.n_elem; j++)
				a2(0, j) = a(j);
			

			mat b = IFFT(a2);
			
			double maxR = 0, maxLc;
			for (j = lmin-1; j < lmax; j++)
			{
				if (b(0, j) > maxR)
				{
					maxR = b(0, j);
					maxLc = j - lmin + 1;;
				}
			}
			period(k - 1) = maxLc + lmin - 1;
		}
	}
}


#endif
#pragma once
