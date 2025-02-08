#include <limits>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Point {
  int x, y;
};

struct Line {
  Point start, end;
};

struct VectorObject {
  virtual std::vector<Line>::iterator begin() = 0;
  virtual std::vector<Line>::iterator end() = 0;
};

struct VectorTriangle : VectorObject {
 private:
  std::vector<Line> lines;

 public:
  VectorTriangle(int x, int y, int width, int height) {
    lines.emplace_back(Line{Point{x, y}, Point{x + width, y}});
    lines.emplace_back(Line{Point{x, y}, Point{x, y + height}});
    lines.emplace_back(Line{Point{x + width, y}, Point{x, y + height}});
    lines.emplace_back(Line{Point{x, y + height}, Point{x + width, y}});
  }

  std::vector<Line>::iterator begin() override { return lines.begin(); }

  std::vector<Line>::iterator end() override { return lines.end(); }
};

struct LineToPointAdapter {
  using Points = std::vector<Point>;

  explicit LineToPointAdapter(const Line& line) {
    int left = std::min(line.start.x, line.end.x);
    int right = std::max(line.start.x, line.end.x);
    int top = std::min(line.start.y, line.end.y);
    int bottom = std::max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    if (dx == 0) {
      for (int y = top; y <= bottom; ++y) {
        points.emplace_back(Point{left, y});
      }
    } else if (dy == 0) {
      for (int x = left; x <= right; ++x) {
        points.emplace_back(Point{x, top});
      }
    }
  }

  virtual Points::iterator begin() { return points.begin(); }

  virtual Points::iterator end() { return points.end(); }

 private:
  Points points;
};

struct HashablePoint {
  int x, y;

  friend std::size_t hash_value(const HashablePoint& obj) {
    std::size_t seed = 0x1E44943F;
    seed ^= obj.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= obj.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

struct HashableLine {
  HashablePoint start, end;

  friend std::size_t hash_value(const HashableLine& line) {
    // hash the start and end points
    std::size_t seed = 0x1E44943F;
    seed ^= hash_value(line.start) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash_value(line.end) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

namespace std {

template <>
struct hash<HashablePoint> {
  std::size_t operator()(const HashablePoint& line) const {
    return hash_value(line);
  }
};

template <>
struct hash<HashableLine> {
  std::size_t operator()(const HashableLine& line) const {
    return hash_value(line);
  }
};

}  // namespace std

struct CacheLineToPointAdapter {
  using HashPoints = std::vector<HashablePoint>;
  std::map<std::size_t, HashPoints> cache;

  explicit CacheLineToPointAdapter(const HashableLine& line) {
    line_hash_ = std::hash<HashableLine>{}(line);
    if (cache.find(line_hash_) != cache.end()) {
      return;
    }
    HashPoints points;
    cache[line_hash_] = points;
  }

  virtual HashPoints::iterator begin() { return cache[line_hash_].begin(); }

  virtual HashPoints::iterator end() { return cache[line_hash_].begin(); }

 private:
  std::size_t line_hash_{};
};

template <typename TFrom, typename TTo>
class Converter {
 public:
  [[nodiscard]]
  virtual TTo Convert(const TFrom& obj) = 0;
  [[nodiscard]]
  virtual TFrom ConvertBack(const TTo& obj) = 0;
};

class IntToStringConverter : public Converter<int, std::string> {
 public:
  [[nodiscard]]
  std::string Convert(const int& obj) override {
    return std::to_string(obj);
  }

  [[nodiscard]]
  int ConvertBack(const std::string& obj) override {
    int result{};
    try {
      result = std::stoi(obj);
    } catch (const std::exception& e) {
      return std::numeric_limits<int>::min();
    }
    return result;
  }
};

int main() {
  std::vector<std::shared_ptr<VectorObject>> vectorobjs{
      std::make_shared<VectorTriangle>(10, 10, 100, 100),
      std::make_shared<VectorTriangle>(10, 10, 100, 100)};

  for (const auto& obj : vectorobjs) {
    for (const auto& line : *obj) {
      LineToPointAdapter ltp{line};
      // drawPoints
    }
  }
  IntToStringConverter converter;
  auto p = converter.Convert(10);

  return 0;
}