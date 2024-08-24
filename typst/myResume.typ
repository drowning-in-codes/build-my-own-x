#import "template.typ": *

#set page(
  margin:(
    left: 10mm,
    right: 10mm,
    top: 15mm,
    bottom: 15mm
  )
 )

#set text(font: "Mulish")

#show: project.with(
  theme: rgb("#0F83C0"),
  name: "proanimer",
  title: "Software Engineer",
  contact:(
    contact(
      text: "personal blog",
      link: "https://sekyoro.top"
    ),
    contact(
      text: "gmail",
      link:"mailto:bukalala174@gmail.com"
    )
  ),
  main:(
    section(
      title:"Work Experience",
      content:(
        subSection(
          title: "Freelancer",
          subTitle: "Software Engineer",
          content:list(
            ["Developed a personal blog using Gatsby and React, with a focus on performance and accessibility."],
            ["Implemented a custom CMS using React and Firebase, allowing for easy content management."],
            ["Optimized the blog for SEO, resulting in a 50% increase in organic traffic."],
            ["Designed and implemented a custom theme, resulting in a 30% increase in user engagement."]
          )
        )
      )
    )
  ),
  sidebar:(
    section(
      title: "Skills",
      content:(
        subSection(
          title: "Languages",
          content: list(
            ["JavaScript", "TypeScript", "Python", "Java", "C++"]
          )
        ),
        )
      ),
      section(
        title:"Education",
        content:(
          subSection(
            title: "University of British Columbia",
            titleEnd: "Vancouver, BC",
            subTitle: "Bachelor of Computer Science",
            subTitleEnd: "(2018 - 2022)"
          )
        )
      )
    )
  )


)
