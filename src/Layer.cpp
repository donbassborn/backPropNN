#include "Layer.hh"
#include <windows.h>
template <typename SynapseT, typename ValueT>
Layer<SynapseT, ValueT>::Layer(const size_t input_count, const size_t neurons_count, const ActivationFunction activation) : input_count(input_count), neurons_count(neurons_count)
{
	neurons.reserve(neurons_count);
	for (size_t i = 0; i < neurons_count; i++)
	{
		this->neurons.emplace_back(input_count, activation);
	}
}

template <typename SynapseT, typename ValueT>
Layer<SynapseT, ValueT>::Layer(const Layer& previous, const size_t neurons_count) : input_count(previous.getNeuronsCount()), neurons_count(neurons_count)
{
	Layer(previous.getNeuronsCount(), neurons_count, previous.neurons[0].getActivation());
}

template <typename SynapseT, typename ValueT>
void Layer<SynapseT, ValueT>::throwError(int line) const
{
	std::cout << "Error occured on line " << line << std::endl;
}

template <typename SynapseT, typename ValueT>
size_t Layer<SynapseT, ValueT>::getNeuronsCount() const
{
	return this->neurons_count;
}

template<typename SynapseT, typename ValueT>
size_t Layer<SynapseT, ValueT>::getInputCount() const
{
	return this->input_count;
}

template<typename SynapseT, typename ValueT>
void Layer<SynapseT, ValueT>::printNeuronWeights() const
{	
	size_t k = 0;
	for(const auto& neuron : this->neurons)	{
		std::cout << std::endl << "\tNeuron #" << k++ << std::endl;
		for (size_t i = 0; i < neuron.getSynapseCount(); i++)
		{
			std::cout << "\t\tSynapse #" << i << ": " <<  neuron.getSynapseWeight(i) << std::endl;
		}
	}
}

template<typename SynapseT, typename ValueT>
void Layer<SynapseT, ValueT>::fillRandom(const std::function<SynapseT()> rnd) 
{
	if(!rnd) 
		throwError(__LINE__);
	else {
		for(auto& neuron : this->neurons) 
			neuron.setSynapseWeightRandom(rnd);
	}
}

template <typename SynapseT, typename ValueT>
typename Layer<SynapseT, ValueT>::ValueVector Layer<SynapseT, ValueT>::fire(const Layer<SynapseT, ValueT>::SynapseVector& input) 
{
	if(this->input_count == input.size()) {
		Layer<SynapseT, ValueT>::ValueVector result(this->neurons_count);
		for (size_t i = 0; i < this->neurons_count; i++) 
			result[i] = this->neurons[i].fire(input);
		return result;
	}
	else {
		throwError(__LINE__);
		return Layer<SynapseT, ValueT>::ValueVector();
	}
}

template<typename SynapseT, typename ValueT>
typename Layer<SynapseT, ValueT>::ValueVector Layer<SynapseT, ValueT>::fire(const std::initializer_list<SynapseT>& input) 
{
	return this->fire(SynapseVector(input));
}


template<typename SynapseT, typename ValueT>
Neuron<SynapseT, ValueT>& Layer<SynapseT, ValueT>::operator[](const size_t index) 
{
	return neurons.at(index);
}
