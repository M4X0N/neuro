#pragma once

/*
This part of library implements basic level of creating ANN of practically any traditional type

Class Network includes methods for building:
addNeuron, addSynaps, addLayer

And methods, that return references to vectors of neurons, synapses and layers.
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <forward_list>
#include <iterator>
#include <algorithm>
#include <memory>
#include <exception>

#include "functions.h"


//////////////////////
/*	LOGGING MACRO	*/
//#ifdef _DEBUG
#define LOG(msg) { \
	std::cerr << msg << " "; \
}
#define ENDLOG { std::cerr <<  std::endl;}
#define ENDLOG2 { std::cerr << "\n\n";}
//#else
//#define LOG(msg) {}
//#define ENDLOG {}
//#endif

namespace neuro 
{
	using iomap = std::map< std::string, float* >;

	class Cell 
	{ 
	friend class Network;
	public: 
		const unsigned int 		id;

		bool operator==(const Cell& rhs) { return rhs.id == this->id ; } ;

		void virtual refresh() = 0; 
	};
	
	class Neuron : public Cell
	{
	protected:      		
		Activation* _function;
		float _alpha;
		float _bias;
		float _axon;
		float _dendron;
		
		friend class Network;
		friend class Cell;
	public:

		const Activation&		getFunction()	const	{ return _function; }
		const float& 			getAlpha() 		const	{ return _alpha; }
		const float& 			getBias() 		const	{ return _bias; }
		const float& 			getAxon() 		const	{ return _axon; }
		const float& 			getDendron()	const	{ return _dendron; }

		
		explicit Neuron(unsigned int id, Activation* func, float alpha, float bias);

		void refresh() override;
	};
	
	class Synaps : public Cell
	{
	protected:
		float*	_axonptr;		// Axon of neuron-transmitter
		float*	_dendronptr;	// Dendron of neuron-reciever
		float	_weight;		// Synaptic weight
		float*	_feedbackptr;	// Feedback from neuron-reciever, for hebbian learning 
		float	_hebb; 			// Hebbian learning coefficient. To be used in future
			
	friend class Network;
	friend class Cell;
	public:
		
		const float&	getWeight()		const	{ return _weight; }
		const float*	getAxon() 		const	{ return _axonptr; }
		const float*	getDendron()	const	{ return _dendronptr; }
	

		explicit Synaps(int id, float* axon, float* dendron, float weight, float hebb);
			
		void refresh() override;
		void hebb_learn( float global_hebb );
	};

	using CellsTable = std::forward_list< std::forward_list<Cell> >;
	using CellsIndex = std::map<int, std::pair< std::forward_list< Cell >*, Cell*>; 
	// int - id of cell. Pointer to layer in list of layers and pointer to cell itself.

	// Two pointers - for dendron and for feedback.
	using DendronPair = std::tuple< float*, float* >;

	class Network
	{
	protected:
	//iomaps can be shared between multiple networks or given individually
		iomap* _inputs;
		iomap* _outputs;
		
		CellsTable _cellsTable; // Here all cells are stored
		CellsIndex _cellsIndex; // Here addresses of layers and cells are stored - for fast search.
 
		//Auxillary methods for synaptic connections
		float* _takeAxon(int neuron);
		float* _takeAxon(std::string input);
		DendronPair _takeDendron(int neuron);
		DendronPair _takeDendron(std::string output);
		
	public:	
		const CellsTable&   getCellsTable()     { return _cellsTable; }
		const LayerIndex&   getCellsIndex()     { return _cellsIndex; }

	//REFRESHMENT

		void tick();

	//NATIVE SYNTHESIS TODO
	/* Basic methods for building the network. Not supposed to be used directly, but in external "synthetizers" e.g. NEAT genetic synthesis */

		/* USELESS NOW
		//Returns true if network contains cell with ID. If indexOnly false - search also in table and add to index if found.
		bool contains(unsigned int id, bool indexOnly = true);
		*/

		//Finds layer, where cell with id is placed. Will throw error "no cell in index" if cell not found.
		unsigned int getLayer(unsigned int id);

		// Add empty layer. If layer exists - new will be inserted under. If not - net will be expanded. Returns new number of layers.
		unsigned int addLayer(unsigned int layer); 
		//Add neuron to Layer. Will call addLayer if layer not exists. TODO check arguments
		void addNeuron(unsigned int id, std::function<float(float,float)>* , float alpha, float bias, unsigned int layer );

		//Connect two neurons, put synaps to layer, set weight and local hebbian learning rate (default 0)
		void addSynaps(unsigned int id, auto axon, auto dendron, unsigned int layer, float weight, float hebb = 0); // TODO implement in .cpp

	//CONSTRUCTOR TODO
		explicit Network(iomap* inputs, iomap* outputs);
	};	
}
