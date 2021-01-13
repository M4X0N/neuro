#pragma once

/*
This part of library implements basic level of creating ANN of practically any traditional type

Class Network includes methods for building:
addNeuron, addSynaps, addLayer

And methods, that return references to vectors of neurons, synapses and layers.
*/

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<list>

#include"functions.h"


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
	using iomap = std::map< std::string, double* >;

	class Cell 
	{ 
	friend class Network;

	public: 
//	virtual const double getSignal() = 0;
	void virtual refresh() = 0; 
	};
	
	class Neuron : public Cell
	{
	protected:      		
		Activation* _function;
		double _alpha;
		double _bias;
		double _axon;
		double _dendron;
		
		friend class Network;
		friend class Cell;
	public:
//	Useless now
//		const Activation&	getFunction()	const	{ return _function; }
//		const double& 		getAlpha() 		const	{ return _alpha; }
//		const double& 		getBias() 		const	{ return _bias; }
//		const double& 		getAxon() 		const	{ return _axon; }
//		const double& 		getDendron()	const	{ return _dendron; }
		
		explicit Neuron(Activation* func, double alpha, double bias);

		void refresh() override;
	};
	
	class Synaps : public Cell
	{
	protected:
		double* _axonptr;
		double* _dendronptr;
		double _weight;
		
	friend class Network;
	friend class Cell;
	public:
		
		// Not very useful
		/*
		const double&	getWeight()	const	{ return _weight; }
		const double*	getAxon() 	const	{ return _axonptr; }
		const double*	getDendron()	const	{ return _dendronptr; }
		*/
	
		explicit Synaps(double* axon, double* dendron, double weight);
			
		void refresh() override;
	};
	
	using Neurons = std::list<Neuron>;
	using Synapses = std::list<Synaps>;
	using CellsTable = std::vector< std::vector< Cell* > >;
	
	class Network
	{
	protected:
		iomap _inputs;
		iomap _outputs;
		
		Neurons _neurons;
		Synapses _synapses;	
		CellsTable _cellsTable;
		
		double* _takeAxon(int neuron);
		double* _takeAxon(std::string input);
		double* _takeDendron(int neuron);
		double* _takeDendron(std::string output);

		bool _addSynaps( auto in, auto out, double weight );
		
	public:	
		const Neurons&      getNeurons() 	    { return _neurons; }
		const Synapses&     getSynapses() 	    { return _synapses; }
		const CellsTable&   getCellsTable()     { return _cellsTable; }
	//REFRESHMENT
		void tick();
	//SYNTHESIS
		//Add neuron to Layer and return it's index from _neurons. Will call addLayer if layer not exists.
		unsigned int addNeuron( std::function<double(double,double)>* , double alpha, double bias, unsigned int layer );
		
		// Add synaps from axon to dendron, place it to layer. Returns if successful or not (A or D not found). If layer not exists - addLayer called.
		bool addSynaps( std::string     axon,  std::string  dendron, double weight, unsigned int layer);
		bool addSynaps( std::string     axon,  unsigned int dendron, double weight, unsigned int layer);
		bool addSynaps( unsigned int    axon,  std::string  dendron, double weight, unsigned int layer);
		bool addSynaps( unsigned int    axon,  unsigned int dendron, double weight, unsigned int layer);
		// Add pseudo-synaps with weight 1 that sends state of axon to external variable. True if succeed
		bool addProbe( unsigned int axon, double* output, unsigned int layer );
		
		// Add empty layerr. If layer exists - new will be unserter under. If not - net will be expanded. Returns new number of layers.
		unsigned int addLayer(unsigned int layer); 
		
	//CONSTRUCTOR
		explicit Network(iomap inputs, iomap outputs);
	};	
}
