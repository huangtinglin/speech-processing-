#include "stdafx.h"
#include "linsomoothm.h"

mat linsomoothm(mat x, int n)
{
	if (n == -1)
		n = 3;
	mat win = hanning(n).t();
	//mat win;
	//win << 0.75 << endr << 0.75 << endr;

	mat s = sum(win);
	win = win / s(0);
	x.reshape(1, x.n_elem);
	int len = x.n_elem;
	mat y = zeros(len, 1);
	if (n % 2 == 0)
	{
		int l = n / 2;
		mat x1(1, 1 + x.n_elem + l);
		for (int i = 0; i < 1 + x.n_elem + l; i++) {
			if (i == 0)
				x1(i) = x(0);
			else if (i >= x.n_elem + 1)
				x1(i) = x(len - 1);
			else
				x1(i) = x(i - 1);
		}
		x = x1.t();
	}
	else {
		int l = (n - 1) / 2;
		mat x1(1, 1 + x.n_elem + 2);
		for (int i = 0; i < 1 + x.n_elem + 2; i++) {
			if (i == 0)
				x1(i) = x(0);
			else if (i >= x.n_elem + 1)
				x1(i) = x(len - 1);
			else
				x1(i) = x(i - 1);
		}
		x = x1.t();
	}
	for (int i = 1; i <= len; i++) {
		//cout << x.rows(i - 1, i + n - 2) << endl;
		mat a = win % x.rows(i - 1, i + n - 2);
		a = sum(a);
		y(i - 1) = a(0);
	}
	return y;
}