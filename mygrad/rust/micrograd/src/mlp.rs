use crate::{layer::Layer, value::Value};

#[derive(Clone)]
pub struct MLP {
    layers: Vec<Layer>,
}

impl MLP {
    pub fn new(input_count: usize, output_counts: Vec<usize>) -> MLP {
        let output_counts_len = output_counts.len();
        let layer_sizes: Vec<usize> = [input_count]
            .iter()
            .chain(output_counts.iter())
            .cloned()
            .collect();
        MLP {
            layers: (0..output_counts_len)
                .map(|i| Layer::new(layer_sizes[i], layer_sizes[i + 1]))
                .collect(),
        }
    }
}
