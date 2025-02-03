#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

// force users to user builder instead of elements to create object
// *****************************************
// struct HtmlElement {
//   std::string name;
//   std::string text;
//   std::vector<HtmlElement> elements;
//   HtmlElement() {}
//   HtmlElement(const std::string &name, const std::string &text)
//       : name{name}, text{text} {}
//   std::string str(int indent = 0) const {
//     std::ostringstream oss;
//     oss << "<" << name << ">" << std::endl;
//     oss << text << std::endl;
//     for (auto &child : elements) {
//       oss << child.str(indent + 2);
//     }
//     oss << "</" << name << ">" << std::endl;
//     return oss.str();
//   }
// };
// *****************************************
using std::string;

struct HtmlBuilder;

struct HtmlElement {
  friend struct HtmlBuilder;
  friend struct FluentHtmlBuilder;
  std::string name, text;
  std::vector<HtmlElement> elements;

 public:
  std::string str(int indent = 0) const {
    std::ostringstream oss;
    oss << "<" << name << ">" << std::endl;
    oss << text << std::endl;
    for (const auto& child : elements) {
      oss << child.str(indent + 2);
    }
    oss << "</" << name << ">" << std::endl;
    return oss.str();
  }

  static std::unique_ptr<HtmlBuilder> create(const std::string& root_name);

  friend std::ostream& operator<<(std::ostream& os,
                                  const HtmlElement& element) {
    os << element.str();
    return os;
  }

 protected:
  HtmlElement() = default;

  HtmlElement(string name, std::string text)
      : name{std::move(name)}, text{std::move(text)} {}
};

struct HtmlBuilder {
  HtmlElement root_ele;

  explicit HtmlBuilder(const std::string& root_name)
      : root_ele{root_name, ""} {}

  std::string str(int indent = 0) const { return root_ele.str(indent); }

  void add_child(std::string name, std::string text) {
    root_ele.elements.push_back({std::move(name), std::move(text)});
  }

  HtmlElement build() const { return root_ele; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const HtmlBuilder& element) {
    os << element.str();
    return os;
  }

  explicit operator HtmlElement() const { return root_ele; }
};

struct FluentHtmlBuilder {
  HtmlElement root_ele;

  explicit FluentHtmlBuilder(std::string root_name)
      : root_ele{std::move(root_name), ""} {}

  std::string str(int indent = 0) const {
    std::ostringstream oss;
    oss << "<" << root_ele.name << ">" << std::endl;
    for (const auto& child : root_ele.elements) {
      oss << child.str(indent + 2);
    }
    oss << "</" << root_ele.name << ">" << std::endl;
    return oss.str();
  }

  FluentHtmlBuilder& add_child(const std::string& name,
                               const std::string& text) {
    root_ele.elements.push_back({name, text});
    return *this;
  }

  HtmlElement build() const { return root_ele; }

  HtmlElement operator()() const { return root_ele; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const FluentHtmlBuilder& element) {
    os << element.str();
    return os;
  }
};

std::unique_ptr<HtmlBuilder> HtmlElement::create(const std::string& root_name) {
  return std::make_unique<HtmlBuilder>(root_name);
}

// Groovy style builder
struct Tag {
  std::string name, text;
  std::vector<Tag> children;
  std::vector<std::pair<std::string, std::string>> attributes;

  friend std::ostream& operator<<(std::ostream& os, const Tag& tag) {
    std::string attrs;
    for (const auto& attr : tag.attributes) {
      attrs += attr.first + "=\"" + attr.second + "\"";
    }
    os << "<" << tag.name << " " << attrs << ">" << std::endl;
    if (tag.children.empty()) {
      os << tag.text << std::endl;
    } else {
      for (const auto& child : tag.children) {
        os << child;
      }
    }
    os << "</" << tag.name << ">" << std::endl;
    return os;
  }

 protected:
  Tag(string name, std::string text)
      : name{std::move(name)}, text{std::move(text)} {}

  Tag(string name, const std::vector<Tag>& children)
      : name{std::move(name)}, children{children} {}
};

struct P : Tag {
  explicit P(const std::string& text) : Tag{"p", text} {}

  P(std::initializer_list<Tag> children) : Tag{"p", children} {}
};

struct IMG : Tag {
  explicit IMG(const std::string& url) : Tag{"img", ""} {
    attributes.emplace_back("src", url);
  }
};
// composite builder
class PersonBuilder;

class Person {
  friend class PersonBuilder;

 public:
  static PersonBuilder create();
  std::string street_address, post_code, city;
  std::string company_name, position;
  int annual_income = 0;

 private:
  Person() = default;
};
class PersonAddressBuilder;
class PersonJobBuilder;

class PersonBuilderBase {
 protected:
  Person& person_;

  explicit PersonBuilderBase(Person& person) : person_{person} {}

 public:
  PersonAddressBuilder lives() const;
  PersonJobBuilder works() const;

  Person build() const { return std::move(person_); }

  explicit operator Person() const { return std::move(person_); }
};

class PersonAddressBuilder : public PersonBuilderBase {
  using self = PersonAddressBuilder;

 public:
  explicit PersonAddressBuilder(Person& person) : PersonBuilderBase{person} {}

  self& at(std::string street_address) {
    person_.street_address = std::move(street_address);
    return *this;
  }

  self& with_post_code(std::string post_code) {
    person_.post_code = std::move(post_code);
    return *this;
  }

  self& in(std::string city) {
    person_.city = std::move(city);
    return *this;
  }
};

class PersonJobBuilder : public PersonBuilderBase {
  using self = PersonJobBuilder;

 public:
  explicit PersonJobBuilder(Person& person) : PersonBuilderBase{person} {}

  self& at(std::string company_name) {
    person_.company_name = std::move(company_name);
    return *this;
  }

  self& as_a(std::string position) {
    person_.position = std::move(position);
    return *this;
  }

  self& earning(int annual_income) {
    person_.annual_income = annual_income;
    return *this;
  }
};

PersonJobBuilder PersonBuilderBase::works() const {
  return PersonJobBuilder{person_};
}

PersonAddressBuilder PersonBuilderBase::lives() const {
  return PersonAddressBuilder{person_};
}

class PersonBuilder : public PersonBuilderBase {
  Person person_;

 public:
  PersonBuilder() : PersonBuilderBase{person_} {}
};

PersonBuilder Person::create() {
  return PersonBuilder{};
}

// builder parameter
class MailService {
  class Email {
   public:
    std::string from, to, subject, body;
  };

  // MailService() = delete;

 public:
  class EmailBuilder {
    Email& email_;

   public:
    explicit EmailBuilder(Email& email) : email_{email} {}

    EmailBuilder& from(const std::string& from) {
      email_.from = from;
      return *this;
    }

    EmailBuilder& to(const std::string& to) {
      email_.to = to;
      return *this;
    }

    EmailBuilder& subject(const std::string& subject) {
      email_.subject = subject;
      return *this;
    }

    EmailBuilder& body(const std::string& body) {
      email_.body = body;
      return *this;
    }
  };

  void send_email(const std::function<void(EmailBuilder&)>& builder) {
    Email email;
    EmailBuilder b{email};
    builder(b);
    send_email_impl(email);
  }

 private:
  void send_email_impl(const Email& email) {
    /*
     * actually send email
     */
  }
};

/*
 * build inheritance
 */
class People {
  friend class PeopleBuilder;

 private:
  People() = default;

 public:
  std::string name, position, date_of_birth;

  friend std::ostream& operator<<(std::ostream& os, const People& people) {
    os << "name: " << people.name << " position: " << people.position
       << " date_of_birth: " << people.date_of_birth;
    return os;
  }
};

class PeopleBuilder {
 protected:
  People people_;

 public:
  [[nodiscard]] People build() const { return people_; }
};

template <typename TSelf>
class PeopleInfoBuilder : public PeopleBuilder {
  friend TSelf;

  using self = TSelf&;

 private:
  PeopleInfoBuilder() = default;

 public:
  self with_name(const std::string& name) {
    people_.name = name;
    return static_cast<self>(*this);
  }

  self with_position(const std::string& position) {
    people_.position = position;
    return static_cast<self>(*this);
  }
};

template <typename TSelf>
class PeopleJobBuilder : public PeopleInfoBuilder<PeopleJobBuilder<TSelf>> {
  friend TSelf;
  using self = TSelf&;

 private:
  PeopleJobBuilder() = default;

 public:
  self works_as_a(const std::string& position) {
    this->people_.position = position;
    return static_cast<self>(*this);
  }
};

template <typename TSelf>
class PeopleBirthBuilder : public PeopleJobBuilder<PeopleBirthBuilder<TSelf>> {
  using self = TSelf&;
  friend TSelf;

 private:
  PeopleBirthBuilder() = default;

 public:
  self born(const std::string& date_of_birth) {
    this->people_.date_of_birth = date_of_birth;
    return static_cast<self>(*this);
  }
};

class MyBuilder : public PeopleBirthBuilder<MyBuilder> {};

int main() {
  HtmlBuilder builder{"html"};
  builder.add_child("head", "head content");
  std::cout << builder << '\n';
  FluentHtmlBuilder fbuilder{"html"};
  fbuilder.add_child("ul", "Hi").add_child("li", "");
  std::cout << fbuilder;
  auto builder2 = HtmlElement::create("html");
  builder2->add_child("body", "body");
  std::cout << builder2->build();
  // Groovy DSL
  std::cout << P{IMG{"https://sekyoro.top"}};
  Person p = Person::create()
                 .lives()
                 .at("123 London Road")
                 .in("London")
                 .with_post_code("SW12BC")
                 .works()
                 .at("Fabrikam")
                 .as_a("Engineer")
                 .earning(123000)
                 .build();
  // builder parameter
  // MailService::Email email;
  MailService ms;
  ms.send_email([&](auto& eb) {
    eb.from("foo@bar.com").to("bar@baz.com").subject("Hello");
  });
  // builder inheritance
  MyBuilder pbuilder;
  auto pp = pbuilder.with_name("John").build();
  std::cout << pp << '\n';
}
