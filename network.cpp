#include "network.h"
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
unsigned int neuro::Network::addNeuron(Activation* func, double alpha, double bias, unsigned int layer)
{
	_neurons.emplace_back( func, alpha, bias );
	if( _cellsTable.size() <= layer ) 	addLayer(layer);
	_cellsTable[layer].emplace_back( &_neurons.back() );
	return _neurons.size()-1 ;
}

bool neuro::Network::addSynaps( std::string axon, std::string dendron, double weight, unsigned int layer )
{
	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
		return true;
	} else {
		return false;
	}
}
bool neuro::Network::addSynaps( std::string axon, unsigned int dendron, double weight, unsigned int layer )
{
	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
		return true;
	} else {
		return false;
	}
}
bool neuro::Network::addSynaps( unsigned int axon, std::string dendron, double weight, unsigned int layer )
{
	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
		return true;
	} else {
		return false;
	}
}
bool neuro::Network::addSynaps( unsigned int axon, unsigned int dendron, double weight, unsigned int layer )
{
	if(	_addSynaps(axon, dendron, weight) )	{
		if( _cellsTable.size() <= layer )	addLayer(layer);
		_cellsTable[layer].emplace_back( &_synapses.back() );
		return true;
	} else {
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

/*	SYNTHESIS AUXILLARY METHODS		*/
bool neuro::Network::_addSynaps(auto in, auto out, double weight)
{
	double* axonptr = _takeAxon(in);
	double* dendronptr = _takeDendron(out);
	//Check for null(notfound) returns
	if( axonptr != nullptr && dendronptr != nullptr ) {
		_synapses.emplace_back( axonptr, dendronptr, weight );
		return true;
	} else {
		return false;
	};
}
//Overloaded takers. Return NULL if element not found.
double* neuro::Network::_takeAxon(int neuron) {
	if( neuron < _neurons.size() ) {
		auto i = _neurons.begin();
		std::advance(i, neuron);
		return &i->_axon;
	} else {
		return nullptr;
	}
}
double* neuro::Network::_takeDendron(int neuron) {
	if( neuron < _neurons.size() ) {
		auto i = _neurons.begin();
		std::advance(i, neuron);
		return &i->_dendron;
	} else {
		return nullptr;
	}
}
double* neuro::Network::_takeAxon(std::string input) {
	if( _inputs.find(input) != _inputs.end() ) {
		return _inputs[input];
	} else {
		return nullptr;
	}
}
double* neuro::Network::_takeDendron(std::string output) {
	if( _outputs.find(output) != _outputs.end() ) {
		return _outputs[output];
	} else {
		return nullptr;
	}
}

/*	REFRESHES	*/
void neuro::Synaps::refresh()
{
	*_dendronptr += *_axonptr * _weight;
}

void neuro::Neuron::refresh()
{
	_axon = (*_function)(_dendron+_bias,_alpha);
	_dendron = 0;
}

void neuro::Network::tick()
{
	for(auto const& layer : _cellsTable) {
		for(auto const& cell : layer) {
			cell->refresh();
		}
	}
}

/*	CONSTRUCTORS	*/
neuro::Network::Network(iomap inputs, iomap outputs)
{
	_inputs = inputs;
	_outputs = outputs;
}

neuro::Neuron::Neuron(Activation* func, double alpha, double bias)
{
	_axon = 0;
	_function = func;
	_alpha = alpha;
	_bias = bias;
	_dendron = 0;
	
	refresh();
}

neuro::Synaps::Synaps(double* axon, double* dendron, double weight)
{
	_axonptr = axon;
	_dendronptr = dendron;
	_weight = weight;
	refresh();
}
