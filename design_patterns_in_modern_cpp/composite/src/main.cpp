#include <iostream>
#include <list>

class Component {
 protected:
  Component* parent_;

 public:
  virtual ~Component() = default;

  void SetParent(Component* parent) { parent_ = parent; }

  Component* GetParent() const { return parent_; }

  virtual void Add(Component* component) {}

  virtual void Remove(Component* component) {}

  virtual bool IsComposite() const { return false; }

  virtual std::string Operation() const = 0;
};

class Leaf : public Component {
 public:
  std::string Operation() const override { return "Leaf"; }
};

class Composite : public Component {
 protected:
  std::list<Component*> children_;

 public:
  void Add(Component* component) override {
    this->children_.push_back(component);
    component->SetParent(this);
  }

  void Remove(Component* component) override {
    children_.remove(component);
    component->SetParent(nullptr);
  }

  bool IsComposite() const override { return true; }

  std::string Operation() const override {
    std::string result;
    for (const Component* c : children_) {
      if (c == children_.back()) {
        result += c->Operation();
      } else {
        result += c->Operation() + "+";
      }
    }
    return "Branch(" + result + ")";
  }
};

int main() {
  Component* simple = new Leaf;
  std::cout << "Client: I've got a simple component:\n";
  Component* tree = new Composite;
  Component* branch1 = new Composite;
  Component* leaf_1 = new Leaf;
  Component* leaf_2 = new Leaf;
  Component* leaf_3 = new Leaf;
  branch1->Add(leaf_1);
  branch1->Add(leaf_2);
  Component* branch2 = new Composite;
  branch2->Add(leaf_3);
  tree->Add(branch1);
  tree->Add(branch2);
  delete simple;
  delete tree;
  delete branch2;
  delete branch1;
  delete leaf_1;
  delete leaf_2;
  delete leaf_3;
}
