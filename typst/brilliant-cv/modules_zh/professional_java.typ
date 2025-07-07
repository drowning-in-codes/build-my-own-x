// Imports
#import "@preview/brilliant-cv:2.0.4": cvEntry, cvSection
#let metadata = toml("../metadata.toml")
#let cvSection = cvSection.with(metadata: metadata)
#let cvEntry = cvEntry.with(metadata: metadata)


#cvSection("项目与论文经历")
#show link: underline

#cvEntry(
  title: [SpringBoot、Redis、MyBatis、MyBatis-Plus],
  society: [多端仿外卖项目],
  // logo: image("../src/logos/abc_company.png"),
  date: [2023/9 - 2024/1],
  location: [],
  description: list(
    [利用JWT与自定义拦截器验证登录并使用Redis的Cache Aside策略缓存数据,使用CAS乐观锁解决超卖问题,使用可重入锁解决一人一单问题.利用Redis的lua脚本保证操作原子性,此外使用Redis的Stream消息队列优化订单问题,在缓存中使用lua脚本缓存,并在Stream队列中加入对应订单信息用于数据库操作.],
    [利用Redis的Geo数据结果查询附近商户,BitMap用于用户签到.利用Spring Task和WebSocket定时为管理端提醒订单业务并定时清理过期订单.使用SortedSet存储点赞和关注用户博客信息,使用推模式实现好友博客feed流.],
    [编写了项目脚手架,根据数据库表生成controller,service,mapper等层基础模板代码.同时构建docker方便部署.客户端使用Flutter方便多端适配. 客户端增加AI客服,利用SSE、WebSocket等技术实现实时聊天,利用RAG进行知识检索.],
  ),
)

// #cvEntry(
//   title: [SpringBot、Linux],
//   society: [WebServer],
//   // logo: image("../src/logos/pqr_corp.png"),
//   date: [2025/1],
//   location: [],
//   description: list(
//     [基于Epoll的事件驱动web服务器,实现多线程Reactor以及主从Reactor模型.使用互斥锁、条件变量等实现线程池.设计事件驱动核心类,包括服务端Acceptor和连接Connection类等,降低耦合. 使用状态机完成http基本请求和响应解析.],
//     [设计连接定时器,对于长时间未读写的连接进行关闭,提高性能，使用单例模式以及互斥锁实现数据库连接池,减少连接开销.使用单例模式创建日志,同时设计阻塞队列方便日志异步写入.使用智能指针以及Concepts等高级特征提高程序可读性.],
//   ),
// )


#cvEntry(
  title: [SpringBoot、Netty,Redis,MyBatis,Langchain,Flask],
  society: [智聊聊天项目],
  // logo: image("../src/logos/pqr_corp.png"),
  date: [2025/1],
  location: [],
  description: list(
    [采用token实现鉴权,使用Redis存储好友信息与心跳.用户端实现用户加删、拉黑好友,群聊拉删人、解散等功能.管理端实现用户删除、群聊解散以及版本更新等功能. 基于Netty、Websocket支持聊天,同时使用Redisson的RTopic实现集群下消息订阅发布.记录联系人之间聊天记录,实现多媒体文件传输.],
    [使用Langchain调用本地AI模型以及Flask提供服务调用,支持机器人聊天,同时增加语义图片和聊天记录搜索功能.],
  ),
)

#cvEntry(
  title: [目标检测、多智能体协作、自动驾驶],
  society: [多智能体协同感知],
  date: [2023 - 现在],
  location: [],
  description: list(
    [*项目介绍*:在多车协作场景下,基于多车数据集,在噪声、延迟以及有限带宽的场景下提升协作感知性能],
    [使用了点云编码器和Transformer特征融合技术,在多车协作环境下实现了有效避免物体遮挡并扩大视野范围的功能。此方法提升了目标检测精度,并显著减少了通信量],
    [引入范围注意力机制对不同区域物体置信度不同,结合智能体间与局部信息融合技,实现了提升多智能体协作感知中的3D目标检测精度的目标.此外利用VQVAE量化多智能体通信信息以减少通信量。相关工作以第一作者身份发表一区论文T-ITS与TIV,#link("https://github.com/drowning-in-codes/OpenCOOD")[相关代码地址] ],
  ),
)
