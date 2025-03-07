// Imports
#import "@preview/brilliant-cv:2.0.4": cvSection, cvEntry
#let metadata = toml("../metadata.toml")
#let cvSection = cvSection.with(metadata: metadata)
#let cvEntry = cvEntry.with(metadata: metadata)


#cvSection("项目与论文经历")
#show link: underline
#cvEntry(
  title: [目标检测、多智能体协作、自动驾驶],
  society: [多智能体协同感知],
  date: [2023 - 现在],
  location: [],
  description: list(
    [*项目介绍*:在多车协作场景下,基于多车数据集,在噪声、延迟以及有限带宽的场景下提升协作感知性能],
    [使用了点云编码器和Transformer特征融合技术,在多车协作环境下实现了有效避免物体遮挡并扩大视野范围的功能。此方法提升了目标检测精度,并显著减少了通信量],
    [引入范围注意力机制对不同区域物体置信度不同,结合智能体间与局部信息融合技,实现了提升多智能体协作感知中的3D目标检测精度的目标.此外利用VQVAE量化多智能体通信信息以减少通信量。相关工作以第一作者身份发表于IEEE TIV,#link("https://github.com/drowning-in-codes/OpenCOOD")[相关代码地址] ],
 
  ),
  tags: ("Python","OpenCOOD","Transformer","多模态"),
)

#cvEntry(
  title: [图像融合、GAN],
  society: [图像融合模型UFGAN],
  // logo: image("../src/logos/abc_company.png"),
  date: [2023/9 - 2023/12],
  location: [],
  description: list(
    [*项目介绍*:采用Unet结合生成对抗网络(GAN)的方法,实现红外与可见光图像的融合.通过利用可见光图像的纹理细节和红外图像对遮挡物体的清晰表现，提升了融合图像的整体质量和信息准确性],
    [对经典神经网络,如CNN,RNN,GAN等有了深入认识,并且进一步掌握了这些网络的训练方法.代码和相关报告开源至#link("https://github.com/drowning-in-codes/UFGAN/blob/main/main.py")[Github]],
  ),
  tags: ("Python","GAN","Image Fusion"),
)

#cvEntry(
  title: [Java、模板生成器、Web开发],
  society: [JavaGen],
  // logo: image("../src/logos/pqr_corp.png"),
  date: [2024/5],
  location: [],
  description: list([开发Spring Web项目往往需要一系列模板代码,通过自己构建一个模板生成器(类似脚手架)能更好掌握Web后端开发的业务],[#link("https://github.com/drowning-in-codes/JavaGen")[相关代码]]),
  tags: ("Java","SpringBoot"),

)
