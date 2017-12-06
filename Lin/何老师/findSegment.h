#pragma once
#ifndef _BA_findSegment_H_
#define _BA_findSegment_H_

#include<armadillo>
using namespace arma;

class Segment {
public:
	mat begin;
	mat end;
	mat duration;
	Segment(int n) {
		begin.set_size(1, n);
		end.set_size(1, n);
		duration.set_size(1, n);
	}
};

/*
matlab的Segment数组，属性为begin、end、duration
我的是Segment变量，属性为begin矩阵、end矩阵、duration矩阵
matlab要取soundSegment(1).begin
这里是soundSegment.begin(1)
因为mat一定要声明好大小，所以这里有个循环我做了两次，没写好，不过程序是对的，下标从0开始
*/
Segment findSegment(mat express)
{
	int i, k;
	mat voicedIndex(1, express.n_cols);
	if (express(0) == 0) {
		k = 0;
		for (i = 0; i < express.n_cols; i++) {
			if (express(i) != 0)
				voicedIndex(k++) = i + 1;
		}
	}
	else
		voicedIndex = express;

	k = 1;
	for (i = 0; i <= voicedIndex.n_elem - 2; i++) {
		if (voicedIndex(i + 1) - voicedIndex(i) > 1) k++;
	}

	Segment soundSegment(k);
	k = 0;
	soundSegment.begin(0) = voicedIndex(0);
	for (i = 0; i <= voicedIndex.n_elem - 2; i++) {
		if (voicedIndex(i + 1) - voicedIndex(i) > 1) {
			soundSegment.end(k) = voicedIndex(i);
			soundSegment.begin(k + 1) = voicedIndex(i + 1);
			k++;
		}
	}

	soundSegment.end(k) = voicedIndex(voicedIndex.n_elem - 1);
	for (i = 0; i <= k; i++) {
		soundSegment.duration(i) = soundSegment.end(i) - soundSegment.begin(i);
	}
	return soundSegment;
}
#endif
#pragma once