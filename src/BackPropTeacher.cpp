#include "BackPropTeacher.hh"


template<typename FlowT>
BackPropTeacher<FlowT>::BackPropTeacher(Network<FlowT>& net, const ActivationFunction der_act) : net(net), der_activation(der_act)
{

}


template<typename FlowT>
void BackPropTeacher<FlowT>::train(const FlowVector& input, const FlowVector& output, const FlowT correction) 
{
	net.fire(input);
	std::vector<FlowVector> layers_error(net.getLayersCount());

	layers_error[net.getLayersCount() - 1] = this->computeOutputLayerError(output);
	
	for (long l_i = net.getLayersCount() - 2; l_i >=0; l_i--) {
		layers_error[l_i] = this->computeLayerError(l_i, layers_error[l_i+1]);
	}

	for (long l_i = net.getLayersCount() - 1; l_i >= 0; l_i--) {
		this->fixLayerSynapses(l_i, layers_error[l_i], correction);
	}
	
	
}

template<typename FlowT>
typename BackPropTeacher<FlowT>::FlowVector BackPropTeacher<FlowT>::computeLayerError(const size_t index, const FlowVector& prev_layer_error) {
	auto& layer = net[index];
	auto& prev_layer = net[index+1];
	FlowVector this_layer_error(layer.getNeuronsCount(), static_cast<FlowT>(0));

	for (size_t c_i = 0; c_i < layer.getNeuronsCount(); c_i++) {
		FlowT prev_synaptic_error = static_cast<FlowT>(0);
		for (size_t p_i = 0; p_i < prev_layer.getNeuronsCount(); p_i++)	{
			prev_synaptic_error += prev_layer[p_i].getSynapseWeight(c_i) * prev_layer_error[p_i];
		}
		prev_synaptic_error *= der_activation(layer[c_i].getLastValue());
		this_layer_error[c_i] = prev_synaptic_error;
	}

	return this_layer_error;
}

template<typename FlowT>
typename BackPropTeacher<FlowT>::FlowVector BackPropTeacher<FlowT>::computeOutputLayerError(const FlowVector& c_output) 
{
	auto& layer = net[net.getLayersCount()-1];
	FlowVector this_layer_error(layer.getNeuronsCount(), static_cast<FlowT>(0));
	for (size_t c_i = 0; c_i < layer.getNeuronsCount(); c_i++) {
		this_layer_error[c_i] = (c_output[c_i] - layer[c_i].getLastValue()) * der_activation(layer[c_i].getLastValue());
	}
	return this_layer_error;
}

template<typename FlowT>
void BackPropTeacher<FlowT>::fixLayerSynapses(const size_t index, const FlowVector& layer_error, const FlowT correction) 
{
	auto& layer = net[index];
	for (size_t n_i = 0; n_i < layer.getNeuronsCount(); n_i++) {
		auto synapse_weights = layer[n_i].getSynapseWeight();
		auto synapse_inputs = layer[n_i].getLastSynapseInput();
		for (size_t s_i = 0; s_i < layer.getInputCount(); s_i++) {
			synapse_weights[s_i] += correction * synapse_inputs[s_i] * layer_error[n_i];
		}
		layer[n_i].setSynapseWeight(synapse_weights);
	}
}
