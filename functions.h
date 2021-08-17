#pragma once

#include<map>
#include<functional>
#include<string>
#include<cmath>
#include<numbers>

namespace neuro {
	
	using Activation = std::function< double(double, double) >;
	
	std::map< std::string, Activation > activations = 
	{
	{ "linear", 	[](double x, double a) { return x*a; } }, 
	{ "tanh",		[](double x, double a) { return activations["sigmoid"](x,a)*2-1 ;} }, 
	{ "sigmoid", 	[](double x, double a) { return 1/(1+exp(-x*a)); } },
	{ "gauss",		[](double x, double a) { return ( 1/(exp(a)*sqrt(std::numbers::pi*2)) )*exp( -0.5*std::pow( x/exp(a), 2 ) ); 	} 	}, //Sigma is exp of parameter - in order to prevent div by zero.
	
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


	//TODO производные функций
//	std::map< std::string, Activation > derivatives = 
//	{ }
}
