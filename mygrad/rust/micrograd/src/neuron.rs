use crate::value::Value;
use rand::{thread_rng, Rng};

#[derive(Clone)]
pub struct Neuron {
    weights: Vec<Value>,
    bias: Value,
}

impl Neuron {
    pub fn new(input_count: usize) -> Neuron {
        Self {
            weights: (0..input_count)
                .map(|_| Value::from(rand::random::<f64>()))
                .collect(),
            bias: Value::from(rand::random::<f64>()),
        }
    }

    pub fn forward(&self, xs: &Vec<Value>) -> Value {
        let products = std::iter::zip(&self.weights, xs)
            .map(|(a, b)| a * b)
            .collect::<Vec<_>>();
        let sum = self.bias.clone() + products.into_iter().reduce(|a, b| a + b).unwrap();
        sum.tanh()
    }
    pub fn parameters(&self) -> Vec<Value> {
        [self.bias.clone()]
            .into_iter()
            .chain(self.weights.clone())
            .collect()
    }
}
