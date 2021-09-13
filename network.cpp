#include "network.h"
#include "logging.h" 
/*
-DLOGLEVEL=level 1
DEBUG       0
INFO        1
WARNING     2
ERROR       3
CRITICAL    4
*/



	// Class Neuron

// Constructor
neuro::Neuron::Neuron(unsigned int id, Activation* func, float alpha, float bias)
{
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
LOG(INFO) << "Neuron " << _id << " refresh called. Status:\n" << "D " << _dendron << " A " << _axon << " F* " << _function << " a " << _alpha << " b " <<  _bias;

	_axon = (*_function)(_dendron+_bias,_alpha);
	_feedback = _dendron; // Saving incoming signal for hebbian learning.
	_dendron = 0;

LOG(DEBUG) << "Neuron " << _id << " new Ax " << _axon;
}



	// Class Synaps

// Constructor
neuro::Synaps::Synaps(unsigned int id, float* axon, float* dendron, float weight, float hebb)
{
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
LOG(INFO) << "Synaps " << _id << " refresh called. Status: \n" << "A* " << _axonptr << " A " << *_axonptr << " D* " << _dendronptr << " D " << *_dendronptr << " w " << _weight;

	*_dendronptr += *_axonptr * _weight;
LOG(DEBUG) << "Synaps " << _id << ": new D " << *_dendronptr;
}

// Hebb
void neuro::Synaps::hebb_learn( float global_hebb )
{
LOG(INFO) << "Synaps " << _id << "hebb called. w " << _weight << " ghebb " << global_hebb << " lhebb " << _hebb;
	
	_weight =+ ( _hebb * global_hebb * activations["gauss"]( _weight, 0 ) * *_axonptr * ( (*_axonptr * _weight) - *_feedbackptr) ; // Adding to weight: (coeff-s ghebb and lhebb, gauss from 0 to limit weight "runaway")*Axon*Delta between sfeedback and real signal.

LOG(DEBUG) << "Synaps " << _id << " weight hebbed to " << _weight;
}

	// Class Network

//Constructor
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
LOG(DEBUG) << "Network tick finished."
}

//Hebb
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
// Check by int/string presence of cell\io
bool neuro::Network::contains(unsigned int id)
{
	// TODO тут можно подумать о том, чтобы искать только по индексу, либо закапываться в таблицу... Либо сделать отдельную функцию переиндексации	
}

// Layer Finder
unsigned int neuro::Network::getLayer(unsigned int id)
{	
LOG(DEBUG) << "Network getLayer is called for ID: " << id;
	auto it = _cellsIndex.find(id);
	if( it == _cellsIndex.end() ) { 
LOG(WARNING) << "Cell " << id << " not found in _cellsIndex. Throwing exeption";
		throw std::exeption("no cell in index");
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
float* neuro::Network::_takeDendron(int neuron);
{
	// TODO
}
float* neuro::Network::_takeDendron(std::string output);
{
	// TODO
}




















/* Дальше - старый код. Из него буду брать куски */



//! Друже, не проеби главную идею: в этой части никаких лишних методов.
//! Всякие утилиты для поиска клетки на слое и прочие свистелки - в отдельный хедер
//! Все, что нам доступно для голой сети - добавить в нее нейрон на интересующий нас слой,
//! добавить синапс на слой и тикнуть сеть.
//! Дилемма: стоит ли отделить манипуляции со слоями от добавления элементов
//!
//! За разделение: прозрачность. Никакого имплицитного трансформирования структуры
//! За совмещение: не нужно будет отдельной утилитарной функцией выяснять, что где находится
// todo Вечером после душа принято решение разделять
//? Вот еще: второй генотипический (дабловый) параметр у синапса свободен. Можно применить для А - коэффициента обучения, Б - коэффициента реаптейка

/* // TODO РАСПРЕДЕЛИ ВСЕ В ПОРЯДКЕ
TABLE OF CONTENTS:
- NETWORK::SYNTHESIS
- NETWORK::REFRESHMENT
-
- CONSTRUCTORS
*/

/*	SYNTHESIS	*/

// TODO TODO TODO Это все навдо будет переделывать

unsigned int neuro::Network::addNeuron(Activation* func, float alpha, float bias, unsigned int layer)
{
LOG("addNeuron called, funcptr alpha bias layer:"); LOG(func); LOG(alpha); LOG(bias); LOG(layer); ENDLOG;

	_neurons.emplace_back( func, alpha, bias ); 
	if( _cellsTable.size() <= layer ) addLayer(layer); 
	_cellsTable[layer].emplace_back( &_neurons.back() );
LOG("New neuron"); LOG(_neurons.size()-1); LOG("emplaced at layer"); LOG(layer); LOG("cell") LOG( _cellsTable[layer].size()-1); ENDLOG2;
	return _neurons.size()-1 ;
}

bool neuro::Network::addSynaps( std::string axon, std::string dendron, float weight, unsigned int layer )
{
LOG("addSynaps called, axon dendron weight layer:"); LOG(axon); LOG(dendron); LOG(weight); LOG(layer); ENDLOG;

	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
LOG("_addSynaps successful, emplaced at layer"); LOG(layer); LOG("cell"); LOG( _cellsTable[layer].size()-1 ); ENDLOG2;

		return true;
	} else {
LOG("_addSynaps unsuccessful, SYNAPS NOT ADDED"); ENDLOG2;

		return false;
	}
}
bool neuro::Network::addSynaps( std::string axon, unsigned int dendron, float weight, unsigned int layer )
{
LOG("addSynaps called, axon dendron weight layer:"); LOG(axon); LOG(dendron); LOG(weight); LOG(layer); ENDLOG;

	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
LOG("_addSynaps successful, emplaced at layer"); LOG(layer); LOG("cell"); LOG( _cellsTable[layer].size()-1 ); ENDLOG2;

		return true;
	} else {
LOG("_addSynaps unsuccessful, SYNAPS NOT ADDED"); ENDLOG2;

		return false;
	}
}
bool neuro::Network::addSynaps( unsigned int axon, std::string dendron, float weight, unsigned int layer )
{
LOG("addSynaps called, axon dendron weight layer:"); LOG(axon); LOG(dendron); LOG(weight); LOG(layer); ENDLOG;

	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
LOG("_addSynaps successful, emplaced at layer"); LOG(layer); LOG("cell"); LOG( _cellsTable[layer].size()-1 ); ENDLOG2;

		return true;
	} else {
LOG("_addSynaps unsuccessful, SYNAPS NOT ADDED"); ENDLOG2;

		return false;
	}
}
bool neuro::Network::addSynaps( unsigned int axon, unsigned int dendron, float weight, unsigned int layer )
{
LOG("addSynaps called, axon dendron weight layer:"); LOG(axon); LOG(dendron); LOG(weight); LOG(layer); ENDLOG;

	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
LOG("_addSynaps successful, emplaced at layer"); LOG(layer); LOG("cell"); LOG( _cellsTable[layer].size()-1 ); ENDLOG2;

		return true;
	} else { 
LOG("_addSynaps unsuccessful, SYNAPS NOT ADDED"); ENDLOG2;
		return false;
	}
}

unsigned int neuro::Network::addLayer( unsigned int layer )
{
	if( layer < _cellsTable.size() ) { // Insertion case
		std::vector< Cell* > emptylayer;
		_cellsTable.insert( _cellsTable.begin() + layer, emptylayer );
	} else { // extension case
		_cellsTable.resize( layer+1 );
	}
	return _cellsTable.size();
}

/*	NATIVE SYTHESIS 	*/
//TODO REWIRTING
bool neuro::Network::_addSynaps(auto in, auto out, float weight)
{
	float* axonptr = _takeAxon(in);
	float* dendronptr = _takeDendron(out);
	//Check for null(notfound) returns
	if( axonptr != nullptr && dendronptr != nullptr ) {
		_synapses.emplace_back( axonptr, dendronptr, weight );
		return true;
	} else {
		return false;
	};
}
//Overloaded takers. Return NULL if element not found.
float* neuro::Network::_takeAxon(int neuron) {
	if( neuron < _neurons.size() ) {
		auto i = _neurons.begin();
		std::advance(i, neuron);
		return &i->_axon;
	} else {
		return nullptr;
	}
}
float* neuro::Network::_takeDendron(int neuron) {
	if( neuron < _neurons.size() ) {
		auto i = _neurons.begin();
		std::advance(i, neuron);
		return &i->_dendron;
	} else {
		return nullptr;
	}
}
float* neuro::Network::_takeAxon(std::string input) {
	if( _inputs.find(input) != _inputs.end() ) {
		return _inputs[input];
	} else {
		return nullptr;
	}
}
float* neuro::Network::_takeDendron(std::string output) {
	if( _outputs.find(output) != _outputs.end() ) {
		return _outputs[output];
	} else {
		return nullptr;
	}
}
