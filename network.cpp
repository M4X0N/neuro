#include "network.h"

/* LOGGING SYSTEM */
#include "logging.h" 
/*
-DLOGLEVEL=level 1
DEBUG       0
INFO        1
WARNING     2
ERROR       3
CRITICAL    4
*/


	//////////////////
	/* class Neuron */
	//////////////////

// Constructor
neuro::Neuron::Neuron(unsigned int id, Activation* func, float alpha, float bias)
{
LOG(DEBUG) << "Neuron constructor called. id " << id << " a " << alpha << " b " << bias;

	_id = id;
	_axon = 0;
	_function = func;
	_alpha = alpha;
	_bias = bias;
	_dendron = 0;

LOG(INFO) << "Neuron " << _id << " constructed";
}

// Tick
void neuro::Neuron::refresh()
{
LOG(DEBUG) << "Neuron " << _id << " refresh called. Status:\n" << "D " << _dendron << " A " << _axon << " F* " << _function << " a " << _alpha << " b " <<  _bias;

	_axon = (*_function)(_dendron+_bias,_alpha);
	_feedback = _dendron; // Saving incoming signal for hebbian learning.
	_dendron = 0;

LOG(INFO) << "Neuron " << _id << " new Ax " << _axon;
}


	//////////////////
	/* class Synaps */
	//////////////////

// Constructor
neuro::Synaps::Synaps(unsigned int id, float* axon, float* dendron, float weight, float hebb)
{
LOG(DEBUG) << "Synaps constructor called. id " << id << " A* " << axon << " D* " << dendron << " w " << weight << " h " << hebb;

	_id = id;
	_axonptr = axon;
	_dendronptr = dendron;
	_weight = weight;
	_hebb = hebb;

LOG(INFO) << "Synaps " << _id << " constructed";
}

// Tick
void neuro::Synaps::refresh()
{
LOG(DEBUG) << "Synaps " << _id << " refresh called. Status: \n" << "A* " << _axonptr << " A " << *_axonptr << " D* " << _dendronptr << " D " << *_dendronptr << " w " << _weight;

	*_dendronptr += *_axonptr * _weight;
LOG(INFO) << "Synaps " << _id << ": new D " << *_dendronptr;
}

// Hebb
void neuro::Synaps::hebb_learn( float global_hebb )
{
LOG(DEBUG) << "Synaps " << _id << "hebb called. w " << _weight << " ghebb " << global_hebb << " lhebb " << _hebb;
	
	// Adding to weight: (coeff-s ghebb and lhebb, gauss to limit "runaway" of weight)*Axon*Delta between sfeedback and real signal.
	_weight =+ ( _hebb * global_hebb * activations["gauss"]( _weight, 0 ) * *_axonptr * ( (*_axonptr * _weight) - *_feedbackptr); 

LOG(INFO) << "Synaps " << _id << " weight hebbed to " << _weight;
}


	///////////////////
	/* class Network */
	///////////////////

// Constructor
neuro::Network::Network(iomap* inputs, iomap* outputs)
{
	_inputs = inputs;
	_outputs = outputs;


LOG(INFO) << "Network constructed";
}

// Tick
void neuro::Network::tick()
{
LOG(DEBUG) << "Network tick called."
	for(auto const& layer : _cellsTable) {
		for(auto const& cell : layer) {
			cell->refresh();
		}
	}
LOG(DEBUG) << "Network tick finished.";
}

// Hebb
void neuro::Network::hebb( float global_hebb )
{
LOG(DEBUG) << "Network Hebbian Learning called with gHebb " << global_hebb;
	for(auto const& layer : _cellsTable) {
		for(auto const& cell : layer) {
			cell->hebb(global_hebb);
		}
	}
LOG(DEBUG) << "Network Hebb finished";
} 

	/* Native level network building */

/* THIS METHOD IS USELESS NOW
// Check by int/string presence of cell in index. Index Only - if false, it will search in table if not found in index.
bool neuro::Network::contains(unsigned int id, bool indexOnly)
{
LOG(DEBUG) << "Network contains? is called for ID: " << id;

	auto id = _cellsIndex.find(id);
	if( it != _cellsIndex.end() ) {
LOG(INFO) << "Cell ID " << id << "  found in index";
	return true;
	} else {
LOG(INFO) << "Cell ID " << id << " not found in index";
		if( indexOnly ) {
			return false;
		} else {
LOG(INFO) << "Cell ID " << id << " searching in _cellsTable";

			for( auto&& lit l : _cellsTable ) {
				auto cit = l.begin();
				while ( cit != l.end() ) {
					if( *cit.id = id ) {
LOG(INFO) << "Cell ID " << id << " found in table. Adding to index";
						_cellsIndex[id] = &*lit;
						return true;
					} else ++cit;
				}
			}
LOG(INFO) << "Cell ID " << id << " not found in table";
		return false;
		}
	}
}
*/

// Layer Finder
unsigned int neuro::Network::getLayer(unsigned int id)
{	
LOG(DEBUG) << "Network getLayer is called for ID: " << id;
	auto it = _cellsIndex.find(id);
	if( it == _cellsIndex.end() ) { 
LOG(WARNING) << "Cell " << id << " not found in _cellsIndex. Throwing exeption";
		throw std::exception("no cell in index");
	}
	int layer = std::distance(_cellsTable.begin(), std::find(_cellsTable.begin(), _cellsTable.end(), *it ));
LOG(DEBUG) << "Cell with ID " << id << " is found on layer " << layer;
	return layer;
}

// Adding Layer
unsigned int neuro::Network::addLayer(int layer)
{
	unsigned int layern = std::distance( _cellsTable.begin(), _cellsTable.end() );
	if( layer <= 0 ) {
		_cellsTable.emplace_front();
	} else {
		if ( layer <= layern ) {
			auto it = _cellsTable.begin();
			std::advance( it, layern );
		} else {
			auto it = _cellsTable.end();
		}
	}
	_cellsTable.emplace_after(it);
	return ++layern;
}

// Adding Neuron
void neuro::Network::addNeuron(unsigned int id, std::function<float(float,float)>* , float alpha, float bias, unsigned int layer )
{
	// TODO
}

// Adding synaps
	// int neuron - ID of neuron
	// string - name of input or output
void neuro::Network::addSynaps(auto axon, auto dendron,unsigned int layer, float weight, float hebb = 0)
{
	
	{
		if( std::distance( _cellsTable.begin(), _cellsTable.end() ) < layer ) {
			addLayer( layer );
			layer = std::distance( _cellsTable.begin(), _cellsTable.end();
		}
	}
	
	_cellsTable[layer].
	Synaps( /*TODO ID*/, _takeAxon(axon), _takeDendron(dendron), weight, hebb )
}

// Auxillary functions for adding synaps
float* neuro::Network::_takeAxon(int neuron) {
	// TODO
}
float* neuro::Network::_takeAxon(std::string input)
{
	// TODO
}
DendronPair neuro::Network::_takeDendron(int neuron);
{
	// TODO и с туплом разберись. Тупл с двумя float*
}
DendronPair neuro::Network::_takeDendron(std::string output);
{
	// TODO
}
