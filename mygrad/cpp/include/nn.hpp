#include "engine.hpp"
#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

template <typename T>
class Module {
public:
    virtual std::vector<Value<T>> getParameters() {
        return std::vector<Value<T>>();
    }
    void zero_grad() {
        for(Value<T>& p : this->getParameters()) {
            p.grad = 0;
        }
    }
};

template <typename T>
class Neuron : public Module<T> {
public:
    std::vector<Value<T>> weights;
    Value<T> bias;
    float grad;
    bool nonlin;

    Neuron(int nin, bool nonlin = true) : nonlin(nonlin), grad(0) {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::normal_distribution<double> distribution(0.0, 1.0);
        std::vector<Value<T>> v;
        for(int i = 0; i < nin; i++) {
            v.emplace_back(static_cast<T>(distribution(generator)));
        }
        this->weights = v;
        this->bias = Value<T>(static_cast<T>(distribution(generator)));
    }

    Value<T> forward(std::vector<Value<T>>& x) {
        std::vector<T> value;
        assert(x.size() == this->weights.size());
        for(int i = 0; i < this->weights.size(); i++) {
            value.push_back(x[ i ].data * this->weights[ i ].data);
        }
        T sum = std::accumulate(value.begin(), value.end(), T(0));
        sum += this->bias.data;
        Value<T> out(sum);
        if(this->nonlin) { out = out.ReLU(); }
        return out;
    };
    std::vector<Value<T>> getParameters() {
        auto parameters = this->weights;
        parameters.push_back(this->bias);
        return parameters;
    }
    void outputInfo() {
        std::cout << this->info() << std::endl;
    }
    std::string info() {
        return std::format("{} Neuron({})", this->nonlin ? "ReLU" : "Linear",
                           this->weights.size());
    }
};
template <typename T>
class Layer : public Module<T> {
public:
    std::vector<Neuron<T>> neurons;
    Layer(int nin, int nout) {
        for(int i = 0; i < nout; i++) {
            this->neurons.emplace_back(Neuron<T>(nin));
        }
    }
    std::vector<Value<T>> forward(std::vector<Value<T>>& x) {
        std::vector<Value<T>> out;
        for(auto&& neuron : neurons) {
            out.push_back(neuron.forward(x));
        }
        return out;
    }
    std::vector<Value<T>> getParameters() {
        std::vector<Value<T>> parameters;
        for(auto& neuron : this->neurons) {
            for(auto p : neuron.getParameters()) {
                parameters.push_back(p);
            }
        }
        return parameters;
    }
    void outputInfo() {
        std::cout << this->info() << std::endl;
    }
    std::string info() {
        std::string neuronsInfo;
        for(int i = 0; i < this->neurons.size(); i++) {
            auto neuron = this->neurons.at(i);
            neuronsInfo += neuron.info();
            if(i == this->neurons.size() - 1) { break; }
            neuronsInfo += ", ";
        }
        return std::format("Layer of [{}]", neuronsInfo);
    }
};

template <typename T>
class MLP : public Module<T> {
public:
    std::vector<Layer<T>> layers;
    MLP(int nin, std::vector<int> nouts) {
        nouts.insert(nouts.begin(), nin);
        for(int i = 0; i < nouts.size() - 1; i++) {
            auto layer = Layer<T>(nouts[ i ], nouts[ i + 1 ]);
            this->layers.push_back(layer);
        }
    }

    std::vector<Value<T>> getParameters() {
        std::vector<Value<T>> parameters;
        for(auto& layer : this->layers) {
            for(auto p : layer.getParameters()) {
                parameters.push_back(p);
            }
        }
        return parameters;
    }
    std::vector<Value<T>> forward(std::vector<Value<T>> x) {
        std::vector<Value<T>> out;
        for(auto&& layer : this->layers) {
            x = layer.forward(x);
        }
        return x;
    }
    void outputInfo() {
        std::cout << this->info() << std::endl;
    }
    std::string info() {
        std::string mlpInfo;
        for(int i = 0; i < this->layers.size(); i++) {
            auto layer = this->layers.at(i);
            mlpInfo += layer.info();
            if(i == this->layers.size() - 1) { break; }
            mlpInfo += ", ";
        }
        return std::format("MLP of [{}]", mlpInfo);
    }
};
