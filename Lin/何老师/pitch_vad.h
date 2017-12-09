#pragma once
#ifndef _BA_pitch_vad_H_
#define _BA_pitch_vad_H_

#include <cmath>
#include <armadillo>
#include "enframe.h"
#include "fft.h"
#include "findSegment.h"
using namespace arma;

int vosl = 0;
mat Ef;
mat SF;
Segment voiceseg(1);

void pitch_vad(mat x, mat wnd, int inc, double T1, int miniL)
{
	int wlen, i, j, k;
	if (miniL == -1)
		miniL = 10;
	mat y = enframe(x, wnd, inc).t();
	int fn = y.n_cols;
	if (wnd.n_elem == 1)
		wlen = wnd(0);
	else
		wlen = wnd.n_elem;

	mat Esum(1, fn);
	mat H(1, fn);
	mat a;
	for (i = 1; i <= fn; i++)
	{
		mat y1 = y.cols(i - 1, i - 1).t();
		mat y2(2, y1.n_elem);
		y2.fill(0);

		for (j = 0; j < y1.n_elem; j++)
			y2(0, j) = y1(j);

		mat SP1 = FFT(y2);
		mat SP(SP1.n_cols, 1);
		for (j = 0; j < SP1.n_cols; j++) {
			double sum = SP1(0, j)*SP1(0, j) + SP1(1, j)*SP1(1, j);
			SP(j) = sqrt(sum);
		}

		SP = SP.rows(0, wlen / 2);

		a = sum(SP%SP);
		Esum(i - 1) = a(0);

		a = sum(SP);

		mat prob = SP / a(0);

		mat prob1 = prob + 2.2204e-16;
		for (j = 0; j < prob1.n_elem; j++)
			prob1(j) = log(prob1(j));
		mat a = -1 * sum(prob%prob1);
		H(i - 1) = a(0);
	}

	for (i = 0; i < H.n_elem; i++)
	{
		if (H(i) < 0.1)
			H(i) = H.max();
	}

	//Ef.set_size(1, H.n_elem);
	Ef.set_size(1, H.n_elem);
	for (i = 0; i < H.n_elem; i++)
	{
		Ef(i) = sqrt(1 + fabs(Esum(i) / H(i)));
	}
	Ef = Ef / Ef.max();

	int len = 0;
	for (i = 0; i < Ef.n_elem; i++)
	{
		if (Ef(i) >= T1)
			len++;
	}

	mat zindex(1, len);
	k = 0;
	for (i = 0; i < Ef.n_elem; i++) {
		if (Ef(i) >= T1)
			zindex(k++) = i;
	}

	Segment zseg = findSegment(zindex);
	int zsl = zseg.duration.n_elem;
	j = 0;
	SF = a.zeros(1, fn);

	for (k = 0; k < zsl; k++)
	{
		if (zseg.duration(k) >= miniL) {
			vosl++;
		}
	}

	voiceseg.setSize(vosl);
	for (k = 0; k < zsl; k++)
	{
		if (zseg.duration(k) >= miniL) {
			voiceseg.begin(j) = zseg.begin(k);
			voiceseg.end(j) = zseg.end(k);
			voiceseg.duration(j) = zseg.duration(k);
			for (i = zseg.begin(k); i <= zseg.end(k); i++)
				SF(i) = 1;
			j++;
		}
	}
}


#endif
#pragma once
