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
// #include <iterator> // MAY BE USELESS, MAY BE NOT
#include <algorithm>
#include <memory>
#include <exception>

#include "functions.h"


namespace neuro 
{
	using iomap = std::map< std::string, float* >;

	class Cell 
	{ 
	friend class Network;

	private:
		// Access for Network::<native building methods>
		virtual const float* 	_getAxonptr() 		= 0;
		virtual const float* 	_getDendronptr() 	= 0;
		virtual const float* 	_getFeedbackptr() 	= 0;
	public: 
		const unsigned int 		id; // To be used as u want

		// Will be useful in genetic synthesis
		bool operator==(const Cell& rhs) { return rhs.id == this->id ; } ;

		virtual void refresh() = 0; 
	};
	
	class Neuron : public Cell
	{
	friend class Network;

	private:
		const float* 	_getAxonptr(); 		
    	const float* 	_getDendronptr(); 	
    	const float* 	_getFeedbackptr(); 	
	
	protected:      		
		Activation* _function;
		float 		_alpha;
		float 		_bias;
		float 		_axon;
		float 		_dendron;
		float		_feedback;
		
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
	friend class Network;
	friend class Cell;

	private:
		const float* 	_getAxonptr(); 		
    	const float* 	_getDendronptr(); 	
    	const float* 	_getFeedbackptr(); 	
	
	protected:
		float*	_axonptr;		// Axon of neuron-transmitter
		float*	_dendronptr;	// Dendron of neuron-reciever
		float	_weight;		// Synaptic weight
		float*	_feedbackptr;	// Feedback from neuron-reciever, for hebbian learning 
		float	_hebb; 			// Hebbian learning coefficient. To be used in future
			
	public:
		const float&	getWeight()		const	{ return _weight; }
		const float*	getAxon() 		const	{ return _axonptr; }
		const float*	getDendron()	const	{ return _dendronptr; }
	

		explicit Synaps(int id, float* axon, float* dendron, float* feedback, float weight, float hebb);
			
		void refresh() override;
		void hebb_learn( float global_hebb );
	};

	using CellsLayer = std::forward_list< Cell >;
	using CellsTable = std::forward_list< CellsLayer >;
	using CellsIndex = std::map<int, std::pair< std::forward_list< Cell >*, Cell*>; // TODO compiler is angry
	// int - id of cell. Pointer to layer in list of layers and pointer to cell itself.

	// Two pointers - for dendron and for feedback.
	using DendronPair = std::tuple< float*, float* >;

	class Network
	{
	private:
	//iomaps can be shared between multiple networks or given individually
		iomap* _inputs;
		iomap* _outputs;
		
		CellsTable _cellsTable; // Here all cells are stored
		CellsIndex _cellsIndex; // Here addresses of layers and cells are stored - for fast search.

	// КРАЙНЕ СОМНИТЕЛЬНОЕ РЕШЕНИЕ, может все-таки тупль?
		struct _connections
		{
			float* axonptr;
			float* dendronptr;
			float* feedbackptr;
		}

		_connections _getConnections( auto axon, auto dendron);
	// // //

		// Returns a reference to layer. If no such layer - adds one.
		CellsLayer& _onLayer( int layer );
		
	public:	
		const CellsTable&   getCellsTable()     { return _cellsTable; }
		const LayerIndex&   getCellsIndex()     { return _cellsIndex; }

		void tick();		// Full Refreshment
		void hebb( float );	// Hebbian learning

	//NATIVE SYNTHESIS
	/* Basic methods for building the network. Not supposed to be used directly, but in external "synthetizers" e.g. NEAT genetic synthesis */

		unsigned int getLayer(unsigned int id);			//Finds layer, where cell with id is placed. Will throw error "no cell in index" if cell not found.
		unsigned int addLayer(unsigned int layer);		// Add empty layer. If layer exists - new will be inserted under. If not - net will be expanded. Returns index of the last layer.

		void addNeuron(unsigned int id, unsigned int layer, std::function<float(float,float)>* , float alpha = 1, float bias = 0)	//Add neuron to Layer. Will call addLayer if layer not exists.
		void addSynaps(unsigned int id, auto axon, auto dendron, unsigned int layer, float weight, float hebb = 0);					// TODO Add synaps to Layer. 

	//CONSTRUCTOR TODO
		explicit Network(iomap* inputs, iomap* outputs);
	};	
}
