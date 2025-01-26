#include <cassert>
#include <cctype>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum class CharType {
  NO_FIND,
  CHAR,
  STAR,
  LEAD,
  DOT,
  DOLLAR,
};

struct RE {
  std::vector<CharType> type;
  char ch;
  std::vector<std::string> ccl;
  int parentMatch = 0;
  bool isparent = false;
  // bool nccl;
  RE() : type({CharType::NO_FIND}), ch('\0'), ccl({}) {}

  void setMatches(int matches) { this->matches = matches; }
  int getMatches() { return matches; }
  RE(CharType type, char ch, std::vector<std::string> ccl, bool nccl = false)
      : type({CharType::NO_FIND}), ch(ch), ccl(ccl) {}

  void incrementMatches() {
    type.push_back(CharType::NO_FIND);
    parentMatch = matches;
    ccl.push_back("");
    matches = ccl.size();
  }
  CharType &getCurrentType() {
    if (matches <= 0) {
      // throw std::runtime_error(std::string(__func__) +
      // ":decrementing matches below 0");
      return type[0];
    }
    return type[matches - 1];
  }
  CharType getType(int index) {
    if (index < 0 || index >= type.size()) {
      throw std::runtime_error(
          "Index out of range, index: " + std::to_string(index) +
          " size: " + std::to_string(type.size()));
    }
    return type[index];
  }

  void decreaseMatches() {
    if (matches <= 0) {
      throw std::runtime_error(std::string(__func__) +
                               ":decrementing matches below 0");
    }
    matches -= 1;
  }
  std::string &getCurrentCCl() {
    if (ccl.size() == 0) {
      throw std::runtime_error("No matches found");
    }
    if (matches <= 0 && matches > ccl.size()) {
      throw std::runtime_error("Index out of range");
    }
    return ccl[matches - 1];
  }
  std::string &getCCl(int index) {
    if (index < 0 || index >= ccl.size()) {
      throw std::runtime_error(
          "Index out of range, index: " + std::to_string(index) +
          " size: " + std::to_string(ccl.size()));
    }
    return ccl[index];
  }

private:
  int matches = 0;
};

std::string::size_type
findFirstsplitterbetweenPairs(const std::string &pattern, size_t start,
                              size_t count = std::string::npos,
                              char splitter = '|') {
  int depth = 0; // 当前括号的深度
  count = (count == std::string::npos) ? pattern.length() - start : count;
  for (std::string::size_type i = 0; i < count; ++i) {
    std::string::size_type index = start + i;
    if (pattern[index] == '(') {
      depth++; // 进入更深一层括号
    } else if (pattern[index] == ')') {
      depth--; // 退出一层括号
    } else if (pattern[index] == splitter && depth == 1) {
      // 如果找到一个 | 并且当前深度为1（即不在其他括号内）
      return i;
    }
  }
  return std::string::npos; // 如果没有找到符合条件的 |
}

size_t findClosingBracket(const std::string &pattern, size_t start) {
  size_t count = 1;
  for (size_t i = start; i < pattern.length(); i++) {
    if (pattern.at(i) == '(') {
      count++;
    } else if (pattern.at(i) == ')') {
      count--;
    }
    if (count == 0) {
      return i;
    }
  }
  return std::string::npos;
}
size_t findClosingParentheses(const std::string &pattern, size_t start) {
  size_t count = 1;
  for (size_t i = start; i < pattern.length(); i++) {
    if (pattern.at(i) == '[') {
      count++;
    } else if (pattern.at(i) == ']') {
      count--;
    }
    if (count == 0) {
      return i;
    }
  }
  return std::string::npos;
}

bool matchstar(const char &, const std::string &pattern,
               const std::string &text, RE &);
bool matchplus(const char &, const std::string &pattern,
               const std::string &text, RE &);

bool matchmark(const char &, const std::string &pattern,
               const std::string &text, RE &);
bool matchhere(const std::string &pattern, const std::string &text, RE &result,
               bool lead = false, std::string additionalStr = "") {
  if (pattern.empty()) {
    return true;
  }
  if (pattern.at(0) != '$' && text.empty()) {
    return false;
  }
  if (pattern.at(0) == '$' && pattern.length() == 1) {
    return text.empty();
  }

  if (pattern.length() > 1 && pattern.at(1) == '*') {
    return matchstar(pattern.at(0), pattern.substr(2), text, result);
  }
  if (pattern.length() > 1 && pattern.at(1) == '+') {
    return matchplus(pattern.at(0), pattern.substr(2), text, result);
  }
  if (pattern.length() > 1 && pattern.at(1) == '?') {
    return matchmark(pattern.at(0), pattern.substr(2), text, result);
  }

  if (const char &t = pattern.at(0);
      !text.empty() && t != '\\' && t != '[' && t != '(') {
    if (pattern.at(0) == text.at(0) || pattern.at(0) == '.') {
      if (result.getCurrentType() == CharType::CHAR) {
        result.ch = text.at(0);
        result.getCurrentCCl() += text.at(0);
      }
      return matchhere(pattern.substr(1), text.substr(1), result);
    } else {
      if (lead) {
        return false;
      } else {
        return matchhere(pattern, text.substr(1), result);
      }
    }
  }

  if (pattern.at(0) == '\\' && std::isdigit(pattern.at(1))) {
    auto num = pattern.at(1) - '0';
    if (matchhere(result.getCCl(num - 1), text, result)) {
      return matchhere(pattern.substr(2),
                       text.substr(result.getCCl(num - 1).length()), result);
    } else {
      return false;
    }
  }
  if (pattern.substr(0, 2) == "\\d") {
    if (std::isdigit(text.at(0))) {
      if (result.getCurrentType() == CharType::CHAR) {
        result.ch = text.at(0);
        result.getCurrentCCl() += text.at(0);
      }
      if (pattern.substr(1, 2) == "d+") {
        // match until text not equal to \w
        int textEndIndex = 1;
        for (textEndIndex = 1; textEndIndex < text.length(); textEndIndex++) {
          if (std::isalnum(text.at(textEndIndex))) {
            if (result.getCurrentType() == CharType::CHAR) {
              result.ch = text.at(textEndIndex);
              result.getCurrentCCl() += text.at(textEndIndex);
            }
          } else {
            break;
          }
        }
        return matchhere(pattern.substr(3), text.substr(textEndIndex), result);
      } else {
        return matchhere(pattern.substr(2), text.substr(1), result);
      }

      return matchhere(pattern.substr(2), text.substr(1), result);
    } else {
      return matchhere(pattern, text.substr(1), result);
    }
  }
  if (pattern.substr(0, 2) == "\\w") {
    if (std::isalnum(text.at(0))) {
      if (result.getCurrentType() == CharType::CHAR) {
        result.ch = text.at(0);
        result.getCurrentCCl() += text.at(0);
      }
      if (pattern.substr(1, 2) == "w+") {
        // match until text not equal to \w
        int textEndIndex = 1;
        for (textEndIndex = 1; textEndIndex < text.length(); textEndIndex++) {
          if (std::isalnum(text.at(textEndIndex))) {
            if (result.getCurrentType() == CharType::CHAR) {
              result.ch = text.at(textEndIndex);
              result.getCurrentCCl() += text.at(textEndIndex);
            }
          } else {
            break;
          }
        }
        return matchhere(pattern.substr(3), text.substr(textEndIndex), result);
      } else {
        return matchhere(pattern.substr(2), text.substr(1), result);
      }
    } else {
      return matchhere(pattern, text.substr(1), result);
    }
  }

  if (pattern.at(0) == '(') {
    // we need to find the closing bracket
    auto rpos = findClosingBracket(pattern, 1);
    result.isparent = true;
    if (rpos == std::string::npos) {
      return false;
    } else {
      std::string value = pattern.substr(1, rpos - 1);
      result.incrementMatches();
      // TODO: change find_first_of("|") find fist | between bracket pairs
      // auto end = value.find_first_of("|");
      auto end = findFirstsplitterbetweenPairs(pattern, 0);
      if (end == std::string::npos) {
        result.getCurrentType() = CharType::CHAR;
        std::string additionalStr = pattern.substr(rpos + 1, 1);
        if (matchhere(value, text, result, lead, additionalStr)) {
          // match the value ()
          result.getCurrentType() = CharType::NO_FIND;
          std::string searchValue = result.getCurrentCCl();
          for (int i = 2; i <= result.ccl.size(); i++) {
            // replace the value with the value, e.g. \2 -> value of ccl[2]
            auto p = searchValue.find("\\" + std::to_string(i));
            if (p != std::string::npos) {
              searchValue.replace(p, 2, result.getCCl(i - 1));
            }
          }
          auto pos = text.find(searchValue);
          if (pos == std::string::npos) {
            return false;
          }
          if (result.isparent && result.getMatches() != 1 &&
              result.parentMatch != 0) {
            // std::cout << "parentMatch: " << result.parentMatch << std::endl;

            auto &parentMatchContent = result.getCCl(result.parentMatch - 1);

            parentMatchContent += searchValue;
          }

          result.setMatches(result.parentMatch);
          // result.decreaseMatches();
          // if (result.getMatches() > 1) {
          //   result.getCurrentType() = CharType::NO_FIND;
          //   result.getCurrentCCl() += prevCCl;
          //   std::cout << "||" << result.getCurrentCCl() << std::endl;
          // } else {
          //   result.getCurrentType() = CharType::CHAR;
          // }

          return matchhere(pattern.substr(rpos + 1),
                           text.substr(pos + searchValue.length()), result,
                           lead);
        } else {
          return false;
        }
      } else {

        std::string::size_type start{0};
        std::string additionalStr = pattern.substr(rpos + 1, 1);
        while (end != std::string::npos) {
          const std::string word = value.substr(start, end - 1 - start);
          start = end + 1;
          result.getCurrentType() = CharType::CHAR;
          if (matchhere(word, text, result, lead, additionalStr)) {
            result.getCurrentType() = CharType::NO_FIND;
            std::string searchValue = result.getCurrentCCl();
            for (int i = 2; i <= result.ccl.size(); i++) {
              // replace the value with the value, e.g. \2 -> value of ccl[2]
              auto p = searchValue.find("\\" + std::to_string(i));
              if (p != std::string::npos) {
                searchValue.replace(p, 2, result.getCCl(i - 1));
              }
            }
            auto pos = text.find(searchValue);
            if (pos == std::string::npos) {
              return false;
            }
            if (result.isparent && result.getMatches() != 1 &&
                result.parentMatch != 0) {
              // std::cout << "parentMatch: " << result.parentMatch <<
              // std::endl;

              auto &parentMatchContent = result.getCCl(result.parentMatch - 1);

              parentMatchContent += searchValue;
            }

            result.setMatches(result.parentMatch);

            return matchhere(pattern.substr(rpos + 1),
                             text.substr(pos + searchValue.length()), result,
                             lead);
          }
          end = findFirstsplitterbetweenPairs(pattern, start, rpos - start);
        }
        const std::string &word = value.substr(start, rpos - start);

        if (matchhere(word, text, result, lead, additionalStr)) {
          result.getCurrentType() = CharType::NO_FIND;
          std::string searchValue = result.getCurrentCCl();
          for (int i = 2; i <= result.ccl.size(); i++) {
            // replace the value with the value, e.g. \2 -> value of ccl[2]
            auto p = searchValue.find("\\" + std::to_string(i));
            if (p != std::string::npos) {
              searchValue.replace(p, 2, result.getCCl(i - 1));
            }
          }
          auto pos = text.find(searchValue);
          if (pos == std::string::npos) {
            return false;
          }
          if (result.isparent && result.getMatches() != 1) {
            if (result.parentMatch == 0) {
              throw std::runtime_error("Parent match is 0");
            }
            // std::cout << "parentMatch: " << result.parentMatch << std::endl;

            auto &parentMatchContent = result.getCCl(result.parentMatch - 1);

            parentMatchContent += searchValue;
          }

          result.setMatches(result.parentMatch);

          return matchhere(pattern.substr(rpos + 1),
                           text.substr(pos + searchValue.length()), result,
                           lead);
        } else {
          return false;
        }
      }
    }
  }

  if (pattern.at(0) == '[') {
    bool neg = pattern.at(1) == '^' ? true : false;
    auto rpos = pattern.find_last_of("]");
    if (rpos == std::string::npos) {
      return false;
    } else {
      if (!neg) {
        std::string value = pattern.substr(1, rpos - 1);
        // find the composition of the character class
        if (pattern.substr(rpos + 1, 1) == "+") {
          if (value.find_first_of(text.at(0)) != std::string::npos) {
            // pattern similar to [abc]+
            if (result.getCurrentType() == CharType::CHAR) {
              result.ch = text.at(0);
              result.getCurrentCCl() += text.at(0);
            }
            int textEndIndex = 1;
            for (textEndIndex = 1; textEndIndex < text.length();
                 textEndIndex++) {
              if (value.find_first_of(text.at(textEndIndex)) !=
                  std::string::npos) {
                if (result.getCurrentType() == CharType::CHAR) {
                  result.ch = text.at(textEndIndex);
                  result.getCurrentCCl() += text.at(textEndIndex);
                }
              } else {
                break;
              }
            }
            if (result.getCurrentType() == CharType::CHAR) {
              return true;
            } else {
              return matchhere(pattern.substr(rpos + 1),
                               text.substr(textEndIndex), result);
            }
          } else {
            return matchhere(pattern, text.substr(1), result);
          }
        }
        if (pattern.substr(rpos + 1, 1) == "*") {
          if (value.find_first_of(text.at(0)) == std::string::npos) {
            // if dont find the character in the class
            if (result.getCurrentType() == CharType::CHAR) {
              return true;
            } else {
              return matchhere(pattern.substr(rpos), text, result);
            }
          } else {
            // if find the character in the class
            int textEndIndex = 1;
            for (textEndIndex = 1; textEndIndex < text.length();
                 textEndIndex++) {
              if (value.find_first_of(text.at(textEndIndex)) !=
                  std::string::npos) {
                result.ch = text.at(textEndIndex);
                result.getCurrentCCl() += text.at(textEndIndex);
              } else {
                break;
              }
            }
            if (result.getCurrentType() == CharType::CHAR) {
              return true;
            } else {
              return matchhere(pattern.substr(rpos + 1),
                               text.substr(textEndIndex), result);
            }
          }
        }
        if (pattern.substr(rpos + 1, 1) == "?") {
          if (value.find_first_of(text.at(0)) == std::string::npos) {
            if (result.getCurrentType() == CharType::CHAR) {
              return true;
            } else {
              return matchhere(pattern.substr(rpos + 1), text, result);
            }
          } else {
            if (result.getCurrentType() == CharType::CHAR) {
              result.ch = text.at(0);
              result.getCurrentCCl() += text.at(0);
              return true;
            }
            return matchhere(pattern.substr(rpos + 1), text.substr(1), result);
          }
        }
        if (value.find_first_of(text.at(0)) != std::string::npos) {
          return matchhere(pattern.substr(rpos + 1), text.substr(1), result);
        }
        return false;
      } else {
        std::string value = pattern.substr(2, rpos - 2);
        if (pattern.substr(rpos + 1, 1) == "+") {
          if (value.find_first_of(text.at(0)) == std::string::npos) {
            // pattern similar to [abc]+
            if (result.getCurrentType() == CharType::CHAR) {
              result.ch = text.at(0);
              result.getCurrentCCl() += text.at(0);
            }
            int textEndIndex = 1;
            for (textEndIndex = 1; textEndIndex < text.length();
                 textEndIndex++) {
              if (text.substr(textEndIndex, 1) == additionalStr) {
                break;
              }
              if (value.find_first_of(text.at(textEndIndex)) ==
                  std::string::npos) {
                if (result.getCurrentType() == CharType::CHAR) {
                  result.ch = text.at(textEndIndex);
                  result.getCurrentCCl() += text.at(textEndIndex);
                }
              } else {
                break;
              }
            }
            if (result.getCurrentType() == CharType::CHAR) {
              return true;
            } else {
              return matchhere(pattern.substr(rpos + 2),
                               text.substr(textEndIndex), result);
            }
          } else {
            // if (result.getCurrentType() == CharType::CHAR) {
            return matchhere(pattern, text.substr(1), result);
            // } else {
            //   return false;
            // }
          }
        }

        if (value.find_first_of(text.at(0)) != std::string::npos) {
          return false;
        } else if (result.getCurrentType() == CharType::CHAR) {
          return true;
        } else {
          return matchhere(pattern.substr(rpos + 1), text.substr(1), result);
        }
      }
    }
  }
  return false;
}

bool matchmark(const char &c, const std::string &pattern,
               const std::string &text, RE &result) {
  if (text.at(0) == c) {
    if (result.getCurrentType() == CharType::CHAR) {
      result.ch = text.at(0);
      result.getCurrentCCl() += text.at(0);
    }
    return matchhere(pattern, text.substr(1), result, true);
  } else {
    return matchhere(pattern, text, result);
  }
}

bool matchplus(const char &c, const std::string &pattern,
               const std::string &text, RE &result) {
  if ((text.at(0) != c) && (c != '.')) {
    return false;
  } else {
    if (result.getCurrentType() == CharType::CHAR) {
      result.ch = text.at(0);
      result.getCurrentCCl() += text.at(0);
    }
    int i = 1;
    for (i = 1; i < text.length(); i++) {
      if (text.at(i) == c) {
        if (result.getCurrentType() == CharType::CHAR) {
          result.ch = text.at(i);
          result.getCurrentCCl() += text.at(i);
        }
        continue;
      } else {
        break;
      }
    }
    return matchhere(pattern, text.substr(i), result, true);
  }
}
bool matchstar(const char &c, const std::string &pattern,
               const std::string &text, RE &result) {
  if (c == '.' || text.at(0) == c) {
    if (result.getCurrentType() == CharType::CHAR) {
      result.ch = c;
      result.getCurrentCCl() += c;
    }
    int i = 1;
    for (i = 1; i < text.length(); i++) {
      if (text.at(i) == c) {
        if (result.getCurrentType() == CharType::CHAR) {
          result.ch = c;
          result.getCurrentCCl() += c;
        }
        continue;
      } else {
        break;
      }
    }
    return matchhere(pattern, text.substr(i), result, true);
  } else {
    return matchhere(pattern, text, result);
  }
}

bool match_pattern(const std::string &input_line, const std::string &pattern) {
  RE result;
  if (pattern.empty()) {
    return true;
  }
  if (pattern.at(0) == '^') {
    return matchhere(pattern.substr(1), input_line, result, true);
  }

  if (matchhere(pattern, input_line, result)) {
    return true;
  }
  return false;
}

int main(int argc, char *argv[]) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cout << "Logs from your program will appear here" << std::endl;

  if (argc != 3) {
    std::cerr << "Expected two arguments" << std::endl;
    return 1;
  }

  std::string flag = argv[1];
  std::string pattern = argv[2];

  if (flag != "-E") {
    std::cerr << "Expected first argument to be '-E'" << std::endl;
    return 1;
  }

  // Uncomment this block to pass the first stage
  //
  std::string input_line;
  std::getline(std::cin, input_line);

  try {
    if (match_pattern(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
