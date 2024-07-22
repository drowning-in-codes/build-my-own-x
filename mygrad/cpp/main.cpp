#include "engine.hpp"
#include "nn.hpp"
int main() {
    MLP<float> m(1, std::vector<int>{ 10, 20 });
    Value<float> v(2);
    Value<float> w(5);
    auto s = v / w;
    s = v + w;
    std::cout << s << std::endl;
    auto out = m.forward(v.toList());
    for(auto&& i : out) {
        std::cout << i << std::endl;
    }
    auto loss = out[ 0 ].sigmoid();
    loss.backward();
    for(auto&& i : m.getParameters()) {
        std::cout << i.grad << std::endl;
    }
    return 0;
}
