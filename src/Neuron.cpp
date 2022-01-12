#include "Neuron.hh"

template<typename SynapseT, typename ValueT>
Neuron<SynapseT, ValueT>::Neuron(const size_t synapse_count, const ActivationFunction activation) : activation(activation), synapse_count(synapse_count), synapse_weight(synapse_count, static_cast<SynapseT>(0)), synapse_input(synapse_count, static_cast<SynapseT>(0))
{
	this->bias = 0.0F;
	this->value = 0.0F;
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::setSynapseWeight(const Neuron<SynapseT, ValueT>::SynapseVector& weight) 
{
	if(weight.size() == this->synapse_weight.size())
		this->synapse_weight = weight;
	else throwError(__LINE__);
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::setSynapseWeight(const size_t synindex, const SynapseT weight) 
{
	this->synapse_weight.at(synindex) = weight;
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::setSynapseWeightRandom(const std::function<float()> rnd) 
{
	if(!rnd) throwError(__LINE__);
	else {
		for(auto& synapse : synapse_weight)	{
			synapse = rnd();
		}
	}
}

template<typename SynapseT, typename ValueT>
typename Neuron<SynapseT, ValueT>::SynapseVector Neuron<SynapseT, ValueT>::getSynapseWeight() const
{
	return this->synapse_weight;
}

template<typename SynapseT, typename ValueT>
SynapseT Neuron<SynapseT, ValueT>::getSynapseWeight(const size_t synindex) const
{
	if(synindex < synapse_count)
		return this->synapse_weight.at(synindex);
	else 
		throwError(__LINE__);
		return static_cast<SynapseT>(0);
}

template<typename SynapseT, typename ValueT>
typename Neuron<SynapseT, ValueT>::SynapseVector Neuron<SynapseT, ValueT>::getLastSynapseInput() const
{
	return this->synapse_input;
}

template<typename SynapseT, typename ValueT>
SynapseT Neuron<SynapseT, ValueT>::getLastSynapseInput(const size_t synindex) const
{
	if(synindex < synapse_count)
		return this->synapse_input.at(synindex);
	else
		throwError(__LINE__);
		return static_cast<SynapseT>(0);
}

template<typename SynapseT, typename ValueT>
size_t Neuron<SynapseT, ValueT>::getSynapseCount() const
{
	return this->synapse_count;
}
template<typename SynapseT, typename ValueT>
SynapseT Neuron<SynapseT, ValueT>::getBias() const
{
	return this->bias;
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::setBias(const SynapseT bias) 
{
	this->bias = bias;
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::setActivation(const std::function<ValueT(SynapseT)> activation) 
{
	if(!activation)
		throwError(__LINE__);
	else
		this->activation = activation;
}

template<typename SynapseT, typename ValueT>
ValueT Neuron<SynapseT, ValueT>::fire(const Neuron<SynapseT, ValueT>::SynapseVector& input) 
{
	if(input.size() == synapse_count)
		this->synapse_input = input;
	else
		throwError(__LINE__);

	SynapseT synapse_sum = static_cast<SynapseT>(0);
	for (size_t i = 0; i < synapse_count; i++) {
		#ifdef ____DEBUG
			std::cout << "\t\tSYNAPSE #" << i << ":" << std::endl;
			std::cout << "\t\t\tInput:" << this->synapse_input[i] << ":" << std::endl;
			std::cout << "\t\t\tWeight:" << this->synapse_weight[i] << ":" << std::endl;
			std::cout << "\t\t\tCalc:" << this->synapse_input[i] * this->synapse_weight[i] << ":" << std::endl;
		#endif // ____DEBUG
		
		synapse_sum += this->synapse_input[i] * this->synapse_weight[i];
	}

	this->value = this->activation(synapse_sum + this->bias);
	#ifdef ____DEBUG
		std::cout << "\t\t###Sum:" << synapse_sum  << std::endl;
		std::cout << "\t\t###Bias:" << this->bias  << std::endl;
		std::cout << "\t\t###Value:" << this->value  << std::endl;
	#endif // ____DEBUG
	return this->value;
}

template<typename SynapseT, typename ValueT>
ValueT Neuron<SynapseT, ValueT>::getLastValue() const
{
	return this->value;
}

template<typename SynapseT, typename ValueT>
typename Neuron<SynapseT, ValueT>::ActivationFunction Neuron<SynapseT, ValueT>::getActivation() const
{
	return this->activation;
}

template<typename SynapseT, typename ValueT>
void Neuron<SynapseT, ValueT>::throwError(int line) const
{
	std::cout << "Error occured on line " << line << std::endl;
}