export module MyMod;

export class C;
export void print(const C &c);
module :private;

class C {
private:
  int value;

public:
  void print() const;
};

void print(const C &c) { c.print(); }
