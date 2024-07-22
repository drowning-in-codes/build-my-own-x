# here is a mathematical expression that takes 3 inputs and produces one output
import math
from math import log10, sin, cos
from typing import ValuesView
from graphviz.unflattening import backend


def f(a, b, c):
    return -(a**3) + sin(3 * b) - 1.0 / c + b**2.5 - a**0.5


print(f(2, 3, 4))


# write the function df that returns the analytical gradient of f
# i.e. use your skills from calculus to take the derivative, then implement the formula
# if you do not calculus then feel free to ask wolframalpha, e.g.:
# https://www.wolframalpha.com/input?i=d%2Fda%28sin%283*a%29%29%29


def gradf(a, b, c):
    df_da = -3 * a**2 - 0.5 * a ** (-0.5)
    df_db = 3 * cos(3 * b) + 2.5 * b**1.5
    df_dc = c ** (-2.0)
    return [df_da, df_db, df_dc]  # todo, return [df/da, df/db, df/dc]


# expected answer is the list of
ans = [-12.353553390593273, 10.25699027111255, 0.0625]
yours = gradf(2, 3, 4)
for dim in range(3):
    ok = "OK" if abs(yours[dim] - ans[dim]) < 1e-5 else "WRONG!"
    print(f"{ok} for dim {dim}: expected {ans[dim]}, yours returns {yours[dim]}")


# now estimate the gradient numerically without any calculus, using
# the approximation we used in the video.
# you should not call the function df from the last cell

# -----------
numerical_grad = [0, 0, 0]  # TODO
h = 0.000001
value = f(2, 3, 4)
df_da = (f(2 + h, 3, 4) - value) / h
df_db = (f(2, 3 + h, 4) - value) / h
df_dc = (f(2, 3, 4 + h) - value) / h
numerical_grad[0] = df_da
numerical_grad[1] = df_db
numerical_grad[2] = df_dc
# -----------

for dim in range(3):
    ok = "OK" if abs(numerical_grad[dim] - ans[dim]) < 1e-5 else "WRONG!"
    print(
        f"{ok} for dim {dim}: expected {ans[dim]}, yours returns {numerical_grad[dim]}"
    )


# there is an alternative formula that provides a much better numerical
# approximation to the derivative of a function.
# learn about it here: https://en.wikipedia.org/wiki/Symmetric_derivative
# implement it. confirm that for the same step size h this version gives a
# better approximation.

# -----------
numerical_grad2 = [0, 0, 0]  # TODO
h = 0.000001
df_da = (f(2 + h, 3, 4) - f(2 - h, 3, 4)) / (2 * h)
df_db = (f(2, 3 + h, 4) - f(2, 3 - h, 4)) / (2 * h)
df_dc = (f(2, 3, 4 + h) - f(2, 3, 4 - h)) / (2 * h)
numerical_grad2[0] = df_da
numerical_grad2[1] = df_db
numerical_grad2[2] = df_dc
# -----------

for dim in range(3):
    ok = "OK" if abs(numerical_grad2[dim] - ans[dim]) < 1e-5 else "WRONG!"
    print(
        f"{ok} for dim {dim}: expected {ans[dim]}, yours returns {numerical_grad2[dim]}"
    )

# Value class starter code, with many functions taken out
from math import exp, log


class Value:
    def __init__(self, data, _children=(), _op="", label=""):
        self.data = data
        self.grad = 0.0
        self._backward = lambda: None
        self._prev = set(_children)
        self._op = _op
        self.label = label

    def __repr__(self):
        return f"Value(data={self.data})"

    def __add__(self, other):  # exactly as in the video
        other = other if isinstance(other, Value) else Value(other)
        out = Value(self.data + other.data, (self, other), "+")

        def _backward():
            self.grad += 1.0 * out.grad
            other.grad += 1.0 * out.grad

        out._backward = _backward

        return out

    # ------
    # re-implement all the other functions needed for the exercises below
    # your code here
    # TODO
    def __mul__(self, other):
        other = other if isinstance(other, Value) else Value(other)
        out = Value(
            self.data * other.data,
            (self, other),
            "*",
        )

        def _backward():
            self.grad += other.data * out.grad
            other.grad += self.data * out.grad

        out._backward = _backward
        return out

    def exp(self):
        x = self.data
        out = Value(exp(x), (self,), "exp")

        def _backward():
            self.grad += out.grad * out.data

        out._backward = _backward
        return out

    def log(self):
        x = self.data
        out = Value(log(x), (self,), "ln")

        def _backward():
            self.grad += out.grad * x**-1

        out._backward = _backward
        return out

    def __pow__(self, other):
        assert isinstance(other, (float, int))
        out = Value(self.data**other, (self,), f"**{other}")

        def _backward():
            self.grad += other * self.data ** (other - 1) * out.grad

        out._backward = _backward
        return out

    def relu(self):
        out = Value(self.data if self.data > 0 else 0, (self,), "RELU")

        def _backward():
            self.grad += out.grad * (out.data > 0)

        out._backward = _backward
        return out

    def __neg__(self):
        return self * -1

    def __radd__(self, other):
        return self + other

    def __sub__(self, other):
        return self + (-other)

    def __rsub__(self, other):
        return -self + other

    def __rmul__(self, other):
        return self * other

    def __truediv__(self, other):
        return self * other ** (-1)

    def __rtruediv__(self, other):
        return self ** (-1) * other

    # ------

    def backward(self):  # exactly as in video
        topo = []
        visited = set()

        def build_topo(v):
            if v not in visited:
                visited.add(v)
                print(v)
                for child in v._prev:
                    build_topo(child)
                topo.append(v)

        build_topo(self)

        self.grad = 1.0
        for node in reversed(topo):
            node._backward()


# without referencing our code/video __too__ much, make this cell work
# you'll have to implement (in some cases re-implemented) a number of functions
# of the Value object, similar to what we've seen in the video.
# instead of the squared error loss this implements the negative log likelihood
# loss, which is very often used in classification.


# this is the softmax function
# https://en.wikipedia.org/wiki/Softmax_function
def softmax(logits):
    counts = [logit.exp() for logit in logits]
    denominator = sum(counts)
    out = [c / denominator for c in counts]
    return out


# this is the negative log likelihood loss function, pervasive in classification
logits = [Value(0.0), Value(3.0), Value(-2.0), Value(1.0)]
probs = softmax(logits)
loss = -probs[3].log()  # dim 3 acts as the label for this input example
loss.backward()
print(loss.data)

ans = [
    0.041772570515350445,
    0.8390245074625319,
    0.005653302662216329,
    -0.8864503806400986,
]
for dim in range(4):
    ok = "OK" if abs(logits[dim].grad - ans[dim]) < 1e-5 else "WRONG!"
    print(f"{ok} for dim {dim}: expected {ans[dim]}, yours returns {logits[dim].grad}")
