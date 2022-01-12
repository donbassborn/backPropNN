#ifndef INC_NETWORK
#define INC_NETWORK

#include "Neuron.hh"
#include "Layer.hh"
#include <vector>

template <typename FlowT>
class BackPropTeacher;

template <typename FlowT = float>
class Network {
	typedef std::vector<FlowT> FlowVector;
	typedef std::function<FlowT(const FlowT)> ActivationFunction;

	friend class BackPropTeacher<FlowT>;

	private:
		std::vector<Layer<FlowT, FlowT>> net;
		const ActivationFunction activation;
		const size_t input_count;
		size_t output_count;

		Layer<FlowT,FlowT>& operator[](const size_t index);
		void throwError(int line) const;
	public:
		Network(const size_t input, const std::initializer_list<size_t>& layers, const ActivationFunction activation);
		~Network() = default;
		void addLayer(Layer<FlowT, FlowT>&& layer);
		void makeLayer(const size_t neurons);

		void fillRandom(const std::function<float()> rnd);
		void fillRandom(const size_t index, const std::function<float()> rnd);
		
		void printStructure() const;
		void printWeights() const;

		size_t getLayersCount() const;

		FlowVector fire(const FlowVector& input);
};

#include "Network.cpp"

#endif // INC_NETWORK