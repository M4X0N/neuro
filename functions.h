#pragma once

#include<map>
#include<functional>
#include<string>
#include<cmath>
#include<numbers>

namespace neuro {
	
	using Activation = std::function< float(float, float) >;
	
	std::map< std::string, Activation > activations = 
	{
	{ "linear", 	[](float x, float a) { return x*a; } }, 
	{ "sigmoid", 	[](float x, float a) { return 1/(1+exp(-x*a)); } },
	{ "tanh",		[](float x, float a) { return activations["sigmoid"](x,a)*2-1 ;} }, 
// TODO - gauss is not working
	{ "gauss",		[](float x, float a) { return ( 1/(exp(a)*sqrt(std::numbers::pi*2)) )*exp( -0.5*std::pow( x/exp(a), 2 ) ); 	} 	}, //Sigma is exp of parameter - to prevent div by zero.
	
// Logistic sigmoid
// Arctg
// Rootsigm
// Rational sigmoid

// ReLU
// Leaky ReLU
// Спизди с википедии из статьи "Функции активации"

	{ "treshold",	[](float x, float a) { if(x>a) return 1; else return 0; } },	
//	{ "name",	[](float x, float a) { return 0;} },  // TEMPLATE
	};


	//TODO производные функций
//	std::map< std::string, Activation > derivatives = 
//	{ }
}
