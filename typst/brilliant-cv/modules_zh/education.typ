// Imports
#import "@preview/brilliant-cv:2.0.4": cvSection, cvEntry, hBar
#let metadata = toml("../metadata.toml")
#let cvSection = cvSection.with(metadata: metadata)
#let cvEntry = cvEntry.with(metadata: metadata)


#cvSection("教育经历")
#show link: underline
#cvEntry(
  title: [计算机科学与技术硕士],
  society: [西北工业大学],
  date: [2023 - 2026],
  location: [陕西西安],
  logo: image("../src/logos/nwpu.jpg"),
  description: list(
    [第一作者发表论文: #link("https://ieeexplore.ieee.org/document/10713924")["CoRange: Collaborative Range-aware Adaptive Fusion for Multi-Agent Perception"] IEEE TIV #linebreak()
    学生一作已投论文:"QCTF: A Quantized Communication and Transferable Fusion Framework for Multi-agent Collaborative Perception" IEEE T-ITS #link("https://github.com/drowning-in-codes/OpenCOOD")[相关代码] 并发表相关专利 ],
    [AI小项目: #link("https://www.kaggle.com/code/calhouns/stylegan/")[GAN训练以及生成头像] #hBar() #link("https://github.com/drowning-in-codes/classicNets")[classicNets经典网络复现] #hBar() #link("https://github.com/drowning-in-codes/Agentic-RAG-and-LLMs")[使用Langchain、Transformers构建LLM应用] #hBar() #link("https://github.com/drowning-in-codes/mini_sd_trainer")[用于图像生成的LoRA训练器]],
    [技术栈: C++、Python(Gradio,langchain,huggingface-related libs)],
  ),
)

#cvEntry(
  title: [计算机科学与技术学士],
  society: [西北工业大学],
  date: [2019 - 2023],
  location: [陕西西安],
  logo: image("../src/logos/nwpu.jpg"),
  description: list(
    [主修课程:Machine Intelligence and Learning Science	#hBar()人工智能的现代方法 #hBar()Object-oriented System Methodology],
    [奖项: 校级二等奖学金
    #hBar() 学业先进个人
    #hBar() 勤奋博学先进个人
    #hBar() 优秀大学生],
    [参与项目: 基于微信小程序的博物馆志愿者智慧培训管理平台获国家级大学生创新创业项目 #linebreak()
    开发#link("https://github.com/drowning-in-codes/covid_detect")["个人康复管理监测小程序]"申请软著],
    [技术栈: 前端(JavaScript, React)、后端(Flask,Express,Gin)、客户端(Python,Qt,Electron)],
    [其他: GPA:3.768/4.1获得保研资格  #hBar() CET-4 #hBar() CET-6:602]
  ),
)
