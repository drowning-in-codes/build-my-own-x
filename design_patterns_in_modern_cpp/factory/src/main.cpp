#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

enum class Material { kBrick, kAeratedConcrete, kDrywall };

class Point2D {
  float x_, y_;

 public:
  Point2D(float x, float y) : x_{x}, y_{y} {}
};
enum class WallType { basic, main, partition };

class Wall {
  Point2D start, end_;
  int elevation_, height_;

 public:
  Wall(const Point2D start, const Point2D end, int elevation, int height)
      : start{start}, end_{end}, elevation_{elevation}, height_{height} {}
};

class SolidWall : public Wall {
  int width_;
  Material material_;

 protected:
  SolidWall(const Point2D start, const Point2D end, int elevation, int height,
            int width, Material material)
      : Wall{start, end, elevation, height},
        width_{width},
        material_{material} {}

 public:
  // static SolidWall create_main(Point2D start, Point2D end, int elevation,
  //                              int height) {

  //   return SolidWall{start,  end, elevation,
  //                    height, 375, Material::aerated_concrete};
  // }

  static std::unique_ptr<SolidWall> create_partition(Point2D start, Point2D end,
                                                     int elevation,
                                                     int height) {
    auto solidwall_ptr = std::unique_ptr<SolidWall>(
        new SolidWall{start, end, elevation, height, 100, Material::kDrywall});
    return solidwall_ptr;
  }

  static std::shared_ptr<SolidWall> create_main(Point2D start, Point2D end,
                                                int elevation, int height) {
    if (elevation < 0) {
      return {};
    }
    auto p = std::shared_ptr<SolidWall>(
        new SolidWall{start, end, elevation, height, 375, Material::kBrick});
    return p;
  }
};

class WallFactory {
  static std::vector<std::weak_ptr<Wall>> m_walls;

 public:
  static std::shared_ptr<SolidWall> create_main(Point2D start, Point2D end,
                                                int elevation, int height) {
    auto wall = SolidWall::create_main(start, end, elevation, height);
    auto wall_ptr = std::shared_ptr<SolidWall>(wall);
    m_walls.push_back(wall_ptr);
    return wall_ptr;
  }
};

class NestedWall {

 private:
  class BasicWallFactory {
    friend class NestedWall;
    BasicWallFactory() = default;

   public:
    std::shared_ptr<Wall> create(const Point2D start, const Point2D end,
                                 int elevation, int height) {
      Wall* wall = new Wall(start, end, elevation, height);
      return std::shared_ptr<Wall>(wall);
    }
  };

 public:
  static BasicWallFactory factory;
};

NestedWall::BasicWallFactory NestedWall::factory;

struct HotDrink {
  virtual void prepare(int volume) = 0;
  virtual ~HotDrink() = default;
};

struct Tea : HotDrink {
  void prepare(int volume) override {
    std::cout << "Take tea bag, boil water, pour " << volume << "ml, add lemon"
              << std::endl;
  }
};

struct Coffee : HotDrink {
  void prepare(int volume) override {
    std::cout << "Coffee: Grind beans, boil water, pour " << volume
              << "ml, add cream, enjoy!" << std::endl;
  }
};

std::unique_ptr<HotDrink> make_drink(std::string type) {
  std::unique_ptr<HotDrink> drink;
  if (type == "tea") {
    drink = std::make_unique<Tea>();
    drink->prepare(200);
  } else {
    drink = std::make_unique<Coffee>();
    drink->prepare(50);
  }
  return drink;
}

class HotDrinkFactory {

 public:
  virtual ~HotDrinkFactory() = default;
  virtual std::unique_ptr<HotDrink> make() const = 0;
};

class TeaFactory : public HotDrinkFactory {
 public:
  std::unique_ptr<HotDrink> make() const override {
    return std::make_unique<Tea>();
  }
};

class CoffeeFactory : public HotDrinkFactory {
  std::unique_ptr<HotDrink> make() const override {
    return std::make_unique<Coffee>();
  }
};

class DrinkFactory {
  std::map<std::string, std::unique_ptr<HotDrinkFactory>> hot_factories;

 public:
  DrinkFactory() {
    hot_factories["coffee"] = std::make_unique<CoffeeFactory>();
    hot_factories["tea"] = std::make_unique<TeaFactory>();
  }

  std::unique_ptr<HotDrink> make_drink(const std::string& name) {
    auto drink = hot_factories[name]->make();
    drink->prepare(200);
    return drink;
  }
};

class DrinkWithVolumeFactory {
  std::map<std::string, std::function<std::unique_ptr<HotDrink>(void)>>
      factories;

 public:
  DrinkWithVolumeFactory() {
    factories["tea"] = []() {
      auto tea = std::make_unique<Tea>();
      tea->prepare(200);
      return tea;
    };
    factories["coffee"] = []() {
      auto coffee = std::make_unique<Coffee>();
      coffee->prepare(50);
      return coffee;
    };
  }

  inline std::unique_ptr<HotDrink> make_drink(const std::string& name);
};

std::unique_ptr<HotDrink> DrinkWithVolumeFactory::make_drink(
    const std::string& name) {
  return factories[name]();
}

int main() {
  const auto main_wall = SolidWall::create_main({0, 10}, {0, 100}, 0, 300);
  if (!main_wall) {
    std::cout << "Main wall not created" << std::endl;
  }
  auto pwall = NestedWall::factory.create({0, 10}, {0, 100}, 0, 300);
  return 0;
}
