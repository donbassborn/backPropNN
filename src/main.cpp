#include <iostream>
#include <functional>
#include "Network.hh"
#include "BackPropTeacher.hh"

#include <lodepng.h>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <regex>
#include <windows.h>
#include <cmath>
#include <chrono>
#include <utility>

#define getTime() std::chrono::high_resolution_clock::now()
#define getDifference(t1, t2) std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()

// service func for reading images in vector
// 	path: image path
// 	size: image size in pixels (height * width)
std::vector<float> decodeOneStepGrayscale(const std::string& path, const size_t size) {
	std::vector<unsigned char> raw;
	std::vector<float> pixels(size, 0);

	raw.reserve(size);

	unsigned width, height;
	unsigned error = lodepng::decode(raw, width, height, path, LCT_GREY);

	for (size_t i = 0; i < size; i++) {
		pixels[i] = static_cast<float>(raw[i]) / 255.0F;
	}

	if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	return pixels;
}

// service func for reading directory content
//	return: first:  path to element
//			second: char at given position in element filename
//	path: directory path
//	regex_match: only content that matches the pattern would be added to output
//	pos: only content that matches the pattern would be added to output
//  count: limitiation of output size, set to -1 for unlimitied (default)
std::vector<std::pair<std::string, char>> readDirContent(const std::string& path, const std::regex& regex_match, const size_t pos = 0, long count = -1) {

	std::vector<std::pair<std::string, char>> content;
	std::string _path = path + (path.back() == '/' ? "*" : "/*");

	WIN32_FIND_DATAA data;
	HANDLE hFind = FindFirstFileA(_path.c_str(), &data);

	if(hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = data.cFileName;
			std::string filepath = path + (path.back() == '/' ? "" : "/") + filename;
			if(count == 0) break;
			if(std::regex_match(filename, regex_match)) {
				content.push_back(std::make_pair(filepath, data.cFileName[pos]));
				count--;
			}
		}
		while (FindNextFileA(hFind, &data));
		FindClose(hFind);
	}
	return content;
}

// func to convert char to its index like
// A -> 0
// B -> 1
// C -> 3 etc.
short charIndex(char chr) {
	return static_cast<short>(chr) - 65;
}


// ACTIVATION FUNC
const std::function<const float (float )> act = [] (const float sum) {
	return 1.0F / (1.0F + std::exp(-sum));
};

// DERIVATIVE OF ACTIVATION FUNC
const std::function<const float(float)> der_act = [] (const float sum) {
	return sum * (1.0F - sum);
};

// Randomizer func, floats, from -0.5 to 0.5
auto rng = std::default_random_engine {};
const std::function<const float()> rnd = [] () {
	return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 0.5F;
};

//image size, also first layer inputs count
const size_t size = 28 * 28;

//last layer output count (26 letters in latin alphabet)
const size_t outputs_count = 26;

//speed of education
const float eta = 0.05F;

// path to images directory
const std::string train_dir("C:/DEV/Neuro/latinLetters/alphabet");

// service regex to not eat shit
const std::regex reg_dirs("[A-Z]");
const std::regex reg_pics("^.*\\.png$");

int main(int argc, char const *argv[]) {
	std::vector<float> pixels(size); 
	std::vector<float> result(outputs_count); 

	//creatin 5-layer network with <size> inputs and given layers dimentions
	Network<float> network(size, {512, 350, 256, 128, 26}, act);
	BackPropTeacher<float> teacher(network, der_act);
	std::vector<std::pair<std::string, char>> images;

	network.printStructure();

	//fillin network weights with randomizer
	network.fillRandom(rnd);

	//Reading images from dataset dir to one big vector

	//used to align training dataset bc its unaligned (only 1k for "F" and more than 57k for "O")
	//set -1 for non limitation
	size_t count_of_each_letter = -1;

	auto dirs = readDirContent(train_dir, reg_dirs);
	for(const auto& dir : dirs)	{
		std::cout << "Reading dir: " << dir.first << std::endl;
		auto dirContent = readDirContent(dir.first, reg_pics, 0, count_of_each_letter);
		images.insert(images.end(), dirContent.begin(), dirContent.end());
	}
	std::cout << "Read list complete. Size: " << images.size() << std::endl;
	if(images.size() == 0) return 1;

	//shuffle vector 
	std::shuffle(std::begin(images), std::end(images), rng);

	//getting 80% of vector for training set and 20% for test set
	size_t train_last_index = static_cast<size_t>(static_cast<float>(images.size()) * 0.8F);


	//TRAINING
	//count of epoches
	size_t epoches = 1;
	for (size_t epoch = 0; epoch < epoches; epoch++) {
		for (size_t i = 0; i < train_last_index; i++)
		{
			pixels = decodeOneStepGrayscale(images[i].first, size);
	
			//wanted output vector: all elements are 0 except element in charge of given letter
			std::vector<float> outputs(outputs_count, 0.0F);
			outputs[charIndex(images[i].second)] = 1.0F;
	
			//calc estimated training time
			if(i == 0 && epoch == 0) {
				auto t1 = getTime();
				teacher.train(pixels, outputs, eta);
				auto t2 = getTime();
				auto est = (getDifference(t1, t2) + 3) * train_last_index;
				est /= 1000 * 60;
				est *= epoches;
				std::cout << "Approximate time for training: " << est << " min" << std::endl;
				continue;
			}
			else {
				teacher.train(pixels, outputs, eta);
			}
	
			if(i%1000 == 0) std::cout << "Epoch: " << epoch << " Training: " << i << std::endl;
		}
	}

	std::cout << "Train complete. Testing..." << images.size() - train_last_index - 1  <<  std::endl;
	

	//Primitive errors of network:
	//	avg_accuracy: average output of neuron in change of given letter 
	//		so, for image "A" to avg_accuracy would append output of neuron #0
	//	avg_miss: average output of all neurons except output of neuron in change of given letter
	//		for image "B" to avg_miss would append average from neuron #0 to neuron #25 EXCEPT neuron #1
	//	abs_accuracy: ratio between cases count then needed neuron's output value is 0.5 or greater and all cases count
	float avg_accuracy = 0.0F;
	float avg_miss = 0.0F;
	size_t abs_accuracy = 0;

	//TESTING
	for(size_t i = train_last_index; i < images.size(); i++) {
		pixels = decodeOneStepGrayscale(images[i].first, size);
		result = network.fire(pixels);

		avg_accuracy += result[charIndex(images[i].second)];
		float not_charged = 0.0F;
		for (size_t k = 0; k < result.size(); k++) {
			if(k != charIndex(images[i].second)) not_charged += result[k];
		}
		avg_miss += not_charged/static_cast<float>(result.size()-1);

		if(result[charIndex(images[i].second)] > 0.5F) abs_accuracy++;
		if(i%1000 == 0) std::cout << "Testing: " << i << std::endl;

	}
	avg_accuracy = avg_accuracy / static_cast<float>(images.size() - train_last_index - 1);
	avg_miss = avg_miss / static_cast<float>(images.size() - train_last_index - 1);

	std::cout << "Average accuracy: " << avg_accuracy << std::endl;
	std::cout << "Avg miss: " << avg_miss << std::endl;
	std::cout << "Abs accuracy: " << static_cast<float>(abs_accuracy) / static_cast<float>(images.size() - train_last_index - 1) << std::endl;


	return 0;
}