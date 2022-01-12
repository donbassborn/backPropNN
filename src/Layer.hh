#ifndef INC_LAYER
#define INC_LAYER

#include <vector>
#include <iostream>
#include "Neuron.hh"


template <typename SynapseT>
class BackPropTeacher;

template <typename SynapseT = float, typename ValueT = float>
class Layer {
	typedef std::function<ValueT(const SynapseT)> ActivationFunction;
	typedef std::vector<SynapseT> SynapseVector;
	typedef std::vector<ValueT> ValueVector;

	friend class BackPropTeacher<SynapseT>;

	private:
		const size_t neurons_count;
		const size_t input_count;
		std::vector<Neuron<SynapseT, ValueT>> neurons;

		
	public:
		ValueVector fire(const SynapseVector& input);
		ValueVector fire(const std::initializer_list<SynapseT>& input);

		size_t getNeuronsCount() const;
		size_t getInputCount() const;

		void printNeuronWeights() const;
		void fillRandom(const std::function<SynapseT()> rnd);

		Neuron<SynapseT, ValueT>& operator[](const size_t index);

		Layer(const size_t input_count, const size_t neurons_count, const ActivationFunction activation);
		Layer(const Layer& previous, const size_t neurons_count);
		~Layer() = default;

		void throwError(int line) const;
};

#include "Layer.cpp"

#endif // INC_LAYER