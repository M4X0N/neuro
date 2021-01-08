


//EASY BUILDER
// Neuron
void neuro::Network::addCell(std::string func, double alpha, double bias, unsigned int layer)
{
    enableNeuron( addNeuron( func, alpha, bias ), layer );
}
// Synaps
void neuro::Network::addCell(std::string axon, std::string dendron, double weight)
{
    enableSynaps( addSynaps(axon, dendron, weight), findLayer(_takeAxon(axon))+1 )
}

//FINDER
int findLayer(Cell* cell)
{
    for(int i = 0; i < _cellsTable.size(); ++i) {
        for(int j = 0; j < _cellsTable[i].size(); ++j) {
            if(cell == _cellsTable[i][j]) return i;
        }
    }
    return -1;    
}

// ALMOST DONE LINE ----------------------------

//ENABLER
void neuro::Network::_enableCell(Cell* cell, unsigned int layer) {
    if( _cellsTable.size() <= layer ) { _cellsTable.resize(layer+1); }
    _cellsTable[layer].push_back( cell );
}
void neuro::Network::enableNeuron(unsigned int neuron, unsigned int layer) {
    if( _neurons.size() > neuron ) {
        _enableCell( &_neurons[neuron], layer );
    } else {
        LOG("enableNeuron, neuron number");LOG(neuron);LOG("not found");
    }
}
void neuro::Network::enableSynaps(unsigned int synaps, unsigned int layer) {
    if( _synapses.size() > synaps ) {
        _enableCell( &_synapses[synaps], layer );
    } else {
        LOG("enableSynaps, synaps number");LOG(synaps);LOG("not found");
    }
}

//DISABLER
void neuro::Network::disableCell(unsigned int layer, unsigned int cell) {
    _cellsTable[layer].erase( _cellsTable[layer].begin()+cell );
}

//LAYER MANIPULATION 
void neuro::Network::insertLayer(unsigned int layer) {
    if( _cellsTable.size() > layer ) {
        _cellsTable.emplace( _cellsTable.begin()+layer );
    } else {
        _cellsTable.resize(layer+1);
    }
}

//ADDERS
int neuro::Network::addNeuron(std::string func, double alpha, double bias)
{
    _neurons.emplace_back( func, alpha, bias );
    return _neurons.size()-1 ;
}

// takers for creating synapses. Return NULL if not found
double* neuro::Network::_takeAxon(int neuron) {
    if( neuron < _neurons.size() ) {
        return &_neurons[neuron]._axon;
    } else {
        LOG("_takeAxon, neuron number"); LOG(neuron); LOG("Not found");
        return NULL;
    }
}
double* neuro::Network::_takeDendron(int neuron) {
    if( neuron < _neurons.size() ) {
        return &_neurons[neuron]._dendron;
    } else {
        LOG("_takeDendron, neuron number"); LOG(neuron); LOG("Not found");
        return NULL;
    }
}
double* neuro::Network::_takeAxon(std::string input) {
    if( _inputs.find(input) != _inputs.end() ) {
        return _inputs[input];
    } else {
        LOG("_takeAxon, input"); LOG(input); LOG("Not found");
        return NULL;
    }
}
double* neuro::Network::_takeDendron(std::string output) {
    if( _outputs.find(output) != _outputs.end() ) {
        return _outputs[output];
    } else {
        LOG("_takeDendron, output"); LOG(output); LOG("Not found");
        return NULL;
    }
}

// synaps creator (called by public addSynaps mask functions, they're in .h)
// returns index of synaps or -1 if not successful
int neuro::Network::_addSynaps(auto in, auto out, double weight)
{
    double* axonptr = _takeAxon(in);
    double* dendronptr = _takeDendron(out);
    //Check for null(notfound) returns
    if( axonptr != 0 && dendronptr != 0 ) {
        _synapses.emplace_back( axonptr, dendronptr, weight );
        return _synapses.size()-1;
    } else { 
        LOG("_addSynaps"); LOG(in); LOG(out); LOG("Synaps creation not successful, skipping");
        return -1;
    };
}

//REFRESHES
void neuro::Synaps::refresh() 
{
	*_dendronptr += *_axonptr * _weight;
}

void neuro::Neuron::refresh() 
{
	if( activations.find(_function) == activations.end() ) {
		MSG("neuron: FUNCTION not found in Functions.h");
        ENDMSG();
		exit(1);
	} else {	
		_axon = activations[_function](_dendron+_bias,_alpha);	
		_dendron = 0;
	}
}

void neuro::Network::tick() 
{
    for(auto&& layer : _cellsTable) {
        for(auto&& cell : layer) {
            cell->refresh();
        }
    }
}

//CONSTRUCTORS
neuro::Network::Network(iomap inputs, iomap outputs)
{
    _inputs = inputs;
    _outputs = outputs;
}

neuro::Neuron::Neuron(std::string func, double alpha, double bias)
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
}
