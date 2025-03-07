// Import
#import "@preview/brilliant-cv:2.0.4": cvSection, cvSkill, hBar
#let metadata = toml("../metadata.toml")
#let cvSection = cvSection.with(metadata: metadata)


#cvSection("个人技能")
#show link: underline

#box([
- 本人对新技术充满热情,热衷通过实际项目学习新技能,能够快速掌握和应用新技术.经常通过阅读英文论文和技术文档来追踪业界最新动态，且熟悉多种编程语言和工具. 使用编程解决生活中的问题,比如, #link("https://proanimer.com/weather-app/")[arxiv论文查询],#link("https://proanimer.com/weather-app/")[天气查询]等.
- 了解C++17/20, 熟悉Linux编程,计算机网络,数据结构与算法. 愿意学习新知识.
- 熟练使用Python、前端技术栈构建应用. 比如使用PyQt,diffusers,Flask等构建AI应用,包含#link("https://github.com/drowning-in-codes/Agentic-RAG-and-LLMs")[基础RAG以及AI Agent]以及使用Ollama基于本地模型的#link("https://github.com/drowning-in-codes/mini_sd_trainer/tree/main/drawdream")[绘画,写作软件].  
- 经常在#link("https://sekyoro.top")[*个人博客*]和#link("https://proanimer.com")[网站]发表技术文章和搭建服务,比如搭建OSS图床以及使用docker搭建微服务等.
- 热衷参与开源项目与使用效率工具,比如在使用neovim时为blink.cmp(一个neovim的代码补全插件)提交#link("https://github.com/Saghen/blink.cmp/pull/1030")[PR], 为#link("http://www.wox.one/plugin/417")[Wox(类似Windows上的Alfred)]编写#link("https://github.com/drowning-in-codes/mywox-plugin")[插件].
- #cvSkill(
  type: [技术栈],
  info: [C++ #hBar() Python #hBar() JavaScript  #hBar() Java #hBar() and more],
)
])




