import torch
import torch.nn.functional as F


words = open("names.txt", "r").read().splitlines()
b = {}
for w in words:
    chs = ["<S>"] + list(w) + ["<E>"]
    for ch1, ch2 in zip(chs, chs[1:]):
        bigram = (ch1, ch2)
        b[bigram] = b.get(bigram, 0) + 1


chars = sorted(list(set("".join(words))))
stoi = {ch: i for i, ch in enumerate(chars)}
stoi["."] = 0
itos = {i: ch for ch, i in stoi.items()}
N = torch.zeros((27, 27), dtype=torch.int32)
for w in words:
    chs = ["."] + list(w) + ["."]
    for ch1, ch2 in zip(chs, chs[1:]):
        ix1 = stoi[ch1]
        ix2 = stoi[ch2]
        N[ix1, ix2] += 1

P = N.float()
P = P / P.sum(dim=1, keepdim=True)


xs, ys = [], []
for w in words:
    chs = ["."] + list(w) + ["."]
    for ch1, ch2 in zip(chs, chs[1:]):
        ix1 = stoi[ch1]
        ix2 = stoi[ch2]
        xs.append(ix1)
        ys.append(ix2)

xs, ys = torch.tensor(xs), torch.tensor(ys)
num = xs.nelement()
print("number of exmaples", num)

g = torch.Generator().manual_seed(2024)
W = torch.randn((27, 27), generator=g, requires_grad=True)
# W = torch.zeros((27, 27), requires_grad=True)
for k in range(1):
    xenc = F.one_hot(xs, 27).float()
    logits = xenc @ W
    # counts = (logits+1) / (logits+1).sum(dim=1, keepdim=True)
    counts = logits.exp()
    probs = counts / counts.sum(dim=1, keepdim=True)
    loss = -probs[torch.arange(num), ys].log().mean() + 0.01 * (W**2).mean()
    print("loss", loss.item())
    W.grad = None
    loss.backward()
    W.dara -= 0.1 * W.grad  # 0.1 is the learning rate

g = torch.Generator().manual_seed(2024)
for i in range(5):
    out = []
    ix = 0
    while True:
        xenc = F.one_hot(torch.tensor([ix]), num_classes=27).float()
        logits = xenc @ W
        counts = logits.exp()
        probs = counts / counts.sum()
        ix = torch.multinomial(
            probs[0], 1, generator=g
        ).item()  # consider use top-k or top-p sampling
        out.append(itos[ix])
        if ix == 0:
            break
    print("".join(out))
