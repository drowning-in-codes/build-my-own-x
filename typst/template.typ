#let contact(text:"",link:none) = {
  (text:text,link:link)
}

#let subSection(title:"",titleEnd:none,subTitle:none,subTitleEnd:none,content:[]) = {
  (title:title,titleEnd:titleEnd,subTitle:subTitle,subTitleEnd:subTitleEnd,content:content)
}

#let section(title:"",content:subSection()) = {
  (title:title,content:content)
}

#let project(
  theme:rgb("#4273B0"),
  name:"",
  email:none,
  title:none,
  contact:((text:[],link:"")),
  skills:(
    languages:()
  ),
  main:(
   ( title:"",content:[])
  ),
  sidebar:(),
  body
) = {
  let backgroundTitle(content) = {
    align(center,box(fill:theme,text(white,size:1.25em,weight:"bold",upper(content)),width:1fr,inset:0.3em))
  }

  let secondaryTitle(content) = {
    text(weight:"bold",size:1.125em,upper(content))
  }

  let italicColorTitle(content) = {
    text(weight:"bold",style:"italic",size:1.125em,theme,content)
  }
  let contactColumn = align(center)[#contact.map(c => {
    if c.link == none [
      #c.text\
    ] else [
      #underline(link(c.link,text(theme,c.text)))\
    ]
  }).join()]

  grid(
    columns:(1fr,2fr),
    column-gutter:2em,
    contactColumn,
    titleColumn
  )
  set par(justify: true)
  let formattedLanguageSkills = [
    #text(skills.languages.join(" • "))
  ]
  let createLeftRight(left:[],right:none)={
    if(right == none) {
      align(start,text(left))
    }else{
      grid(
        columns:(1fr,1fr),
        align(start,text(left)),
        align(end,right) 
      )
    }
  }

  let parseSubSections(subSections) = {
    subSections.map(s=>{
      [
        #createLeftRight(
          left:secondaryTitle(s.title),
          right:if s.titleEnd != none [
            #italicColorTitle(s.titleEnd)
          ]
        )
        #if s.subTitle != none or s.subTitleEnd != none [
          #text(top-edge: 0.2em,
          createLeftRight(left: italicColorTitle(s.subTile),right:s.subTitleEnd)
          )
        ]
        #s.content
      ]
    }).join()
  }
  let parseSection(seciton) = {
    section.map(m=>{
      [
        #backgroundTitle(m.title),
        #parseSubSections(m.content)
      ]
    }).join()
  }
  let mainSection = parseSection(main)
  let sidebarSection = parseSection(sidebar)
  grid(
    columns: (1fr,2fr),
  column-gutter: 1em,
  sidebarSection,
  mainSection
  )
// Main body.
  set par(justify:true)
  show: columns.with(3,gutter:1.3em)
}

