#pragma once
#ifndef _BA_linsomoothm_H_
#define _BA_linsomoothm_H_

#include<armadillo>
#include "hanning.h"
using namespace arma;

/*
汉宁窗有问题 不过除了汉宁窗的都没问题
*/
mat linsomoothm(mat x, int n);


#endif
#pragma once