#ifndef INC_NEURON
#define INC_NEURON

#include <vector>
#include <functional>
#include <iostream>

template <typename SynapseT = float, typename ValueT = float>
class Neuron {
	typedef std::function<ValueT(SynapseT)> ActivationFunction;
	typedef std::vector<SynapseT> SynapseVector;
	private:
		SynapseVector 		synapse_weight;
		SynapseVector		synapse_input;
		size_t				synapse_count;
		SynapseT 			bias;
		ValueT  			value;
		ActivationFunction 	activation;

		void throwError(int line) const;
	public:
		void setSynapseWeight(const SynapseVector& weight);
		void setSynapseWeight(const size_t synindex, const SynapseT weight);
		void setSynapseWeightRandom(const std::function<float()> rnd);
		void setBias(const SynapseT bias);
		void setActivation(const std::function<ValueT(SynapseT)> activation);

		SynapseVector getSynapseWeight() const;
		SynapseT getSynapseWeight(const size_t synindex) const;

		SynapseVector getLastSynapseInput() const;
		SynapseT getLastSynapseInput(const size_t synindex) const;

		SynapseT getBias() const;
		size_t getSynapseCount() const;
		ActivationFunction getActivation() const;

		
		ValueT fire(const SynapseVector& input);
		ValueT getLastValue() const;

		Neuron(const size_t synapse_count, const ActivationFunction activation);
		~Neuron() = default;
		
};

#include "Neuron.cpp"

#endif // INC_NEURON