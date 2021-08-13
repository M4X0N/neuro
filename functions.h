#pragma once

#include<map>
#include<functional>
#include<string>
#include<cmath>

namespace neuro {
	
	using Activation = std::function< double(double, double) >;
	
	std::map< std::string, Activation > activations = 
	{
	{ "linear", 	[](double x, double a) { return x*a; } }, 
	{ "tanh",	[](double x, double a) { return activations["sigmoid"](x,a)*2-1 ;} }, 
	{ "sigmoid", 	[](double x, double a) { return 1/(1+exp(-x*a)); } },
// Logistic sigmoid
// Arctg
// Rootsigm
// Rational sigmoid

// ReLU
// Leaky ReLU
// Спизди с википедии из статьи "Функции активации"

	{ "treshold",	[](double x, double a) { if(x>a) return 1; else return 0; } },	
//	{ "name",	[](double x, double a) { return 0;} },  // TEMPLATE
	};
}
