import torch
import torch.nn as nn

batch_size = 2
time_steps = 3
embedding_dim = 100

input = torch.randn(batch_size, time_steps, embedding_dim)

# 使用nn.BatchNorm1d
# https://pytorch.org/docs/stable/generated/torch.nn.BatchNorm1d.html

m = nn.BatchNorm1d(embedding_dim)
out = m(input.transpose(-1, -2)).transpose(-1, -2)

# 手写 batch norm
x_mean = input.mean(dim=(0, 1), keepdim=True)
x_std = input.std(dim=(0, 1), unbiased=False, keepdim=True)
output = (input - x_mean) / (x_std + 1e-5)


# 使用nn.LayerNorm
# https://pytorch.org/docs/stable/generated/torch.nn.LayerNorm.html#torch.nn.LayerNorm
m_ln = nn.LayerNorm(normalized_shape=embedding_dim, elementwise_affine=False)
out_ln = m_ln(input)

x_mean_ln = input.mean(dim=-1, keepdim=True)
x_std_ln = input.std(dim=-1, unbiased=False, keepdim=True)
output_ln = (input - x_mean_ln) / (x_std_ln + 1e-5)


# 使用nn.InstanceNorm
m_In = nn.InstanceNorm1d(embedding_dim)
out_In = m_In(input.transpose(-1, -2)).transpose(-1, -2)

x_mean_In = input.mean(dim=1, keepdim=True)
x_std_In = input.std(dim=1, unbiased=False, keepdim=True)
output_In = (input - x_mean_In) / (x_std_In + 1e-5)


# 使用nn.GroupNorm
num_groups = 2
m_GN = nn.GroupNorm(num_groups=num_groups, num_channels=embedding_dim)
out_Gn = m_GN(input.transpose(-1, -2)).transpose(-1, -2)

group_inputx = torch.split(
    input, split_size_or_sections=embedding_dim // num_groups, dim=-1
)
results = []
for g_inputx in group_inputx:
    gn_mean = g_inputx.mean(dim=(1, 2), keepdim=True)
    gn_std = g_inputx.std(dim=(1, 2), keepdim=True)
    gn_result = (g_inputx - gn_mean) / (gn_std + 1e-5)
    results.append(gn_result)

results = torch.cat(results, dim=-1)


# weight normalization
# https://pytorch.org/docs/stable/generated/torch.nn.utils.weight_norm.html
module = nn.Linear(embedding_dim, 10)
m_wn = nn.utils.weight_norm(module, dim=0)
wn_linear_output = m_wn(input)


weight_direction = module.weight / (module.weight.norm(dim=1, keepdim=True))
weight_magnitude = torch.tensor(
    [module.weight[i:].norm() for i in torch.arange(module.weight.size(0))],
    dtype=torch.float32,
).unsqueeze(-1)
output_wn = (
    input @ weight_direction.transpose(-1, -2) * weight_magnitude.transpose(-1, -2)
)
