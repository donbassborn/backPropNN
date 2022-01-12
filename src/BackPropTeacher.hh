#ifndef INC_BACKPROPTEACHER
#define INC_BACKPROPTEACHER

#include "Network.hh"
#include <functional>

template <typename FlowT = float>
class BackPropTeacher {
	typedef std::vector<FlowT> FlowVector;
	typedef std::function<const FlowT(FlowT)> ActivationFunction;

	private:
		Network<FlowT>& net;
		ActivationFunction der_activation;

		FlowVector computeLayerError(const size_t index, const FlowVector& prev_layer_error);
		FlowVector computeOutputLayerError(const FlowVector& c_output);
		void fixLayerSynapses(const size_t index, const FlowVector& layer_error, const FlowT correction);

		void throwError(int line) const;
	public:
		BackPropTeacher(Network<FlowT>& net, ActivationFunction der_act);
		~BackPropTeacher() = default;
		void train(const FlowVector& input, const FlowVector& output, const FlowT correction);
};

#include "BackPropTeacher.cpp"

#endif // INC_BACKPROPTEACHER