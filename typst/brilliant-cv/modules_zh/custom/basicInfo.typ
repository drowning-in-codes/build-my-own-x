// Imports
#import "@preview/brilliant-cv:2.0.4": cvSection, cvEntry, hBar
#let metadata = toml("../../metadata.toml")
#let cvSection = cvSection.with(metadata: metadata)
#let cvEntry = cvEntry.with(metadata: metadata)



#cvSection("基本信息")
#let cvBasicInfoBlock = {grid(
  columns: (1fr, 1fr),
  align(center)[
    姓名: #metadata.lang.non_latin.name \
    电话: #link(metadata.personal.info.phone) \
    邮箱: #box({image("../../src/logos/gmail.svg",width: 12pt)  }) 
          #link("mailto:"+metadata.personal.info.email)[#metadata.personal.info.email]
  ],
  align(center)[
    Github:  #link("https://github.com/"+metadata.personal.info.github)[#metadata.personal.info.github] \
    个人博客: #link("https://sekyoro.top")[Blog]
  ]
)}

#cvBasicInfoBlock
