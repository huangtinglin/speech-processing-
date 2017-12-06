#pragma once

#ifndef META
#define META
#include<iostream>
#include<armadillo>
#define pi 3.1415926
using namespace std;
using namespace arma;

mat meta(int n) {
	mat x(2, n + 1);
	for (int i = 1; i <= n; i++) {
		x(1, i) = i;
	}
	return x;
}

double mean(mat a) {
	double s = 0;
	for (int i = 1; i <= a.n_rows; i++) {
		for (int j = 1; j < a.n_cols; j++) {
			s += a(i, j);
		}
	}
	return s / a.n_elem;
}

#endif
#pragma once