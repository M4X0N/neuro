//#pragma once

/*
This part of library implements basic level of creating ANN of practically any traditional type

Class Network includes methods for building:
    Adding elements (addNeuron, addSynaps)
    Setting order of activation (enableNeuron, enableSynaps)
    Insertion of layer (insertLayer) under layer or on top

And methods, that return references to vectors of neurons, synapses and layers.
*/

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<utility>

#include"functions.h"

#define MSG(msg) {std::cout << msg << "\t";} 
#define ENDMSG() {std::cout << std::endl;}

//#ifdef DEBUG
#define LOG(msg) {std::cout << msg << std::endl;}
//#else
//#define LOG(msg) {}
//#endif

namespace neuro 
{
	using iomap = std::map< std::string, double* >;

    class Cell 
    {   public: 
            void virtual refresh() = 0; 
    };

    class Neuron : public Cell
	{
	protected:      		
		std::string _function;
		double _alpha;
        double _bias;
		double _axon;
		double _dendron;

        friend class Network;
        friend class Cell;
	public:
		
        // Only for external use
		const std::string& 	getFunction()	const	{ return _function; }
		const double& 		getAlpha() 		const	{ return _alpha; }
        const double& 		getBias() 		const	{ return _bias; }
		const double& 		getAxon() 		const	{ return _axon; }
		const double& 		getDendron()	const	{ return _dendron; }

        explicit Neuron(std::string func, double alpha, double bias);
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

        // Only for external use
		const double& 		getWeight()		const	{ return _weight; }
        // For any use
		const double* 		getAxon() 		const	{ return _axonptr; }
		const double* 		getDendron() 	const	{ return _dendronptr; }

        explicit Synaps(double* axon, double* dendron, double weight);

		void refresh() override;
	};

	using Neurons = std::vector<Neuron>;
	using Synapses = std::vector<Synaps>;
    
    using CellsTable = std::vector< std::vector< Cell* > >;

	class Network
	{
	protected:
        iomap _inputs;
        iomap _outputs;

		Neurons _neurons;
		Synapses _synapses;	
        std::vector< std::vector< Cell* > > _cellsTable;

        // Taking elements (nonconstant pointers to them)
        // Retrun NULL if element doesnt exist
        double* _takeAxon(int neuron);
        double* _takeAxon(std::string input);
        double* _takeDendron(int neuron);
        double* _takeDendron(std::string output);
        
        int _addSynaps(auto in, auto out, double weight); 

        //adding cells to activation order
        void _enableCell(Cell* cell, unsigned int layer);

	public:	
		const Neurons&      getNeurons() 	    { return _neurons; }
		const Synapses&     getSynapses() 	    { return _synapses; }
        const CellsTable&   getCellsTable()     { return _cellsTable; }
    
    //EZ building methods
    // addCell returns true if adding is successful
        void addCell(std::string, double, double, unsigned int);
        void addCell(std::string, std::string, double);

    //"Surgical" building methods
    //STRUCTURAL MODIFICATIONS
        int addNeuron( std::string func, double alpha, double bias ); // returns index
        // "Mask" overloaded method for private _addSynaps
        // Returns index of newly created synaps in _synapses or -1,
        int addSynaps(std::string in, std::string out, double weight)     { return _addSynaps(in,out,weight); }
        int addSynaps(unsigned int in, std::string out, double weight)    { return _addSynaps(in,out,weight); }
        int addSynaps(std::string in, unsigned int out, double weight)    { return _addSynaps(in,out,weight); }
        int addSynaps(unsigned int in, unsigned int out, double weight)   { return _addSynaps(in,out,weight); }

        // Adding cells to activation order. Without it cell will never be activated.
        void enableNeuron(unsigned int neuron, unsigned int layer);
        void enableSynaps(unsigned int synaps, unsigned int layer);
        // Remove cell from activation order
        void disableCell(unsigned int layer, unsigned int cell);
        // Auxillary function for insertion layer under layer n. If there are no layer n - adding layers up to n on top;
        void insertLayer(unsigned int layer);

    // INTERNAL MODIFICATIONS
        void adjustAlpha(unsigned int neuron, double alpha);
        void adjustBias(unsigned int neuron, double bias);
        void adjustWeight(unsigned int synaps, double weight);

		explicit Network(iomap inputs, iomap outputs);
		void tick();
	};	
}
