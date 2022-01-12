#include "Network.hh"
#include <iostream>

template<typename FlowT>
Network<FlowT>::Network(const size_t input_count, const std::initializer_list<size_t>& layers, const ActivationFunction activation) : input_count(input_count), activation(activation) {
	this->output_count = 0;
	for(const auto& layer_size : layers)	{
		makeLayer(layer_size);
	}
}

template<typename FlowT>
void Network<FlowT>::addLayer(Layer<FlowT, FlowT>&& layer) 
{
	if(net.empty()) {
		if(input_count == layer.getInputCount()) {
			net.emplace_back(layer);
			this->output_count = layer.getNeuronsCount();
		}
	}
}

template<typename FlowT>
void Network<FlowT>::makeLayer(const size_t neurons) 
{
	if(net.empty()) {
		net.emplace_back(input_count, neurons, activation);
	}
	else {
		net.emplace_back(net.back().getNeuronsCount(), neurons, activation);
	}
	this->output_count = neurons;
}

template<typename FlowT>
void Network<FlowT>::fillRandom(const std::function<float()> rnd) 
{
	for (size_t i = 0; i < net.size(); i++)	{
		fillRandom(i, rnd);
	}
}

template<typename FlowT>
void Network<FlowT>::fillRandom(const size_t index, const std::function<float()> rnd) 
{
	for (size_t i = 0; i < net.at(index).getNeuronsCount(); i++) {
		for (size_t s_i = 0; s_i < net.at(index)[i].getSynapseCount(); s_i++) net.at(index)[i].setSynapseWeight(s_i, rnd());
	}
}

template<typename FlowT>
void Network<FlowT>::printStructure() const
{
	for (size_t i = 0; i < net.size(); i++)
	{
		std::cout << "Layer #" << i << ": synapses = " << net[i].getInputCount() << " neurons = " << net[i].getNeuronsCount() << std::endl;
	}
}

template<typename FlowT>
void Network<FlowT>::printWeights() const
{
	for (size_t i = 0; i < net.size(); i++)
	{
		std::cout << "Layer #" << i << std::endl;
		net[i].printNeuronWeights();
	}
}

template<typename FlowT>
size_t Network<FlowT>::getLayersCount() const
{
	return net.size();
}

template<typename FlowT>
Layer<FlowT,FlowT>& Network<FlowT>::operator[](const size_t index)
{
	return net.at(index);
}

template<typename FlowT>
typename Network<FlowT>::FlowVector Network<FlowT>::fire(const FlowVector& input) 
{
	auto result = input;
	for(auto& layer : net) {
		result = layer.fire(result);
	}
	return result;
}
