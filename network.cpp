#include "network.h"

	////////////////////
	/* LOGGING SYSTEM */
	////////////////////

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

const float* neuro::Neuron::_getAxonptr()
{
	return &_axon; 
}

const float* neuro::Neuron::_getDendronptr()
{
	return &_dendron;
}

const float* neuro::Neuron::_getFeedbackptr()
{
	return &_feedback;
}

	//////////////////
	/* class Synaps */
	//////////////////

// Constructor
neuro::Synaps::Synaps(unsigned int id, float* axon, float* dendron, float* feedback, float weight, float hebb)
{
LOG(DEBUG) << "Synaps constructor called. id " << id << " A* " << axon << " D* " << dendron << " w " << weight << " h " << hebb;

	_id = id;
	_axonptr = axon;
	_dendronptr = dendron;
	_weight = weight;
	_feedbackptr = feedback;
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

// ( TODO ) Это заглушка, возвращающая указатели на A и D соединяемых нейронов. В будущем - имплементировать собственные А и Д с ФБ внутри синапса для реализации "синаптического умножения".
const float* neuro::Synaps::_getAxonptr()
{
	return _axonptr; 
}

const float* neuro::Synaps::_getDendronptr()
{
	return _dendronptr;
}

const float* neuro::Synaps::_getFeedbackptr()
{
	return _feedbackptr;
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

	/* NATIVE SYNTHESIS */


CellsLayer& neuro::Network::_onLayer( int layer )
{	
	auto it = _cellsTable.begin();
	for( int i = 0; i != layer; ++i ) {
		if( it == _cellsTable.end() ) {

LOG(DEBUG) << "No layer " << layer << " in network. Calling addLayer(" << i << ")";
			addLayer(i);
			break;
		}
	}

	return *it;
}

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

unsigned int neuro::Network::addLayer(int layer)
{
LOG(DEBUG) << "addLayer called, l " << layer;

	if( layer <= 0 ) {
		_cellsTable.emplace_front();
	} else {

		auto it = _cellsTable.begin();
		int i;
		for(i = 0; i != layer || it != _cellsTable.end(); ++i) ++it;

LOG(DEBUG) << "addLayer insertion to layer " << i;
		_cellsTable.emplace_after(it);

		for(; it != _cellsTable.end(); i++ ) ++it;

	}
LOG(INFO) << "Layer " << layer << " inserted. Last layer: " << i; // TODO Эмпирически проверить корректность возвратов
	return i;
}

void neuro::Network::addNeuron(unsigned int id, unsigned int layer, std::function<float(float,float)>* func , float alpha = 1, float bias = 0)
{
LOG(DEBUG) << "addNeuron called to layer " << layer << " id " << id << " a " << alpha << " b " << bias;

	Neuron neuron = Neuron(id, func, alpha, bias);	// TODO Вообще не уверен, что это будет работать
	_onLayer( layer ).insert_after( *it.begin(), neuron );

LOG(INFO) << "Neuron id " << id << " inserted";
}

// Adding synaps TODO TODO TODO
void neuro::Network::addSynaps(unsigned int id, unsigned int layer, auto axon, auto dendron, float weight, float hebb = 0)
{
LOG(DEBUG) << "addSynaps called to layer " << layer << " id " << id << " A " << axon << " D " << dendron << " w " << weight << " h " << hebb;

		// TODO

	Synaps synaps = Synaps( id, axonptr, dendronptr, feedbackptr, weight, hebb );
	_onLayer( layer ).insert_after( *it.begin(), synaps ); // TODO TODO И тут ессесно тоже хз, будет ли работать
}
 
// Auxillary functions for adding synaps




