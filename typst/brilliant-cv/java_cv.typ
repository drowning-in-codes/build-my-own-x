// Imports
#import "@preview/brilliant-cv:2.0.4": cv
#let metadata = toml("./metadata.toml")
#let importModules(modules, lang: metadata.language) = {
  for module in modules {
    include {
      "modules_" + lang + "/" + module + ".typ"
    }
  }
}


#show: cv.with(
  metadata,
  profilePhoto: image("./src/profile.jfif"),
)
#importModules((
  "education",
  "professional_java",
  // "projects",
  // "certificates",
  // "publications",
  "skills",
))
