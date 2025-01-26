// copied from other's implementation

#include <algorithm>
#include <cctype>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#define NONE 0
#define DIGIT 1
#define ALPHANUMERIC 2
#define POSITIVE_GROUPS 4
#define NEGATIVE_GROUPS 8

const std::string special = R"(\[+.?)";

bool start_anchor = false;
std::deque<std::string> group_deque;
std::string groups_prev;
int grep_type_prev = NONE;
int number_prev = 0;
std::vector<std::string> my_split(const std::string &str,
                                  const std::string &flag) {
  std::vector<std::string> res;
  int space = flag.size();
  size_t slow = 0;
  size_t fast = str.find(flag);
  while (fast != std::string::npos) {
    res.emplace_back(str.substr(slow, fast - slow));
    slow = fast + space;
    fast = str.find(flag, slow);
  }
  res.emplace_back(str.substr(slow));
  return res;
}

bool match_groups(const std::string &input_line, const std::string &pattern,
                  bool flag) {
  std::size_t pos = input_line.find_first_of(pattern);
  if (flag) {
    return pos != std::string::npos && pos == 0;
  }
  return pos != std::string::npos;
}

std::vector<std::pair<int, int>> find_match_parent(const std::string &pattern) {
  std::vector<std::pair<int, int>> paren_vec;
  std::vector<int> stack;
  stack.push_back(0);

  for (int i = 0; i < pattern.length() && !stack.empty(); ++i) {
    if (pattern[i] == '(') {
      stack.push_back(i);
    } else if (pattern[i] == ')') {
      if (!stack.empty()) {
        int start = stack.back();
        stack.pop_back();
        paren_vec.push_back(std::make_pair(start, i));
      }
    }
  }
  return paren_vec;
}

bool match_pattern(const std::string &input_line, const std::string &pattern,
                   bool flag) {
  if (pattern.length() == 0 ||
      // 正则表达式匹配完毕 或者 匹配到末尾并且匹配字符串为空
      (pattern[0] == '$' && input_line.length() == 0)) {
    return true;
  }
  if (pattern.length() > 1 && pattern[1] == '?') {
    // 匹配 ?
    if (pattern[0] == input_line[0]) {
      // 如果存在
      number_prev++;
      return match_pattern(input_line.substr(1), pattern.substr(2), true);
    } else {
      // 如果没有
      return match_pattern(input_line, pattern.substr(2), flag);
    }
  }
  if (input_line.length() == 0) {
    // 匹配字符串为空但是正则不为空
    return false;
  }
  if (pattern[0] == '^') {
    // 匹配开头
    start_anchor = true;
    return match_pattern(input_line, pattern.substr(1), true);
  } else if (pattern.substr(0, 2) == R"(\d)") {
    //  匹配数字 \d
    grep_type_prev |= DIGIT;
    if (std::isdigit(input_line[0])) {
      number_prev++;
      return match_pattern(input_line.substr(1), pattern.substr(2), true);
    }
    // flag is whether previous char matched
    return flag ? false : match_pattern(input_line.substr(1), pattern, false);
  } else if (pattern.substr(0, 2) == R"(\w)") {
    // 匹配字母+数字
    grep_type_prev |= ALPHANUMERIC;
    if (std::isalnum(input_line[0]) || input_line[0] == '_') {
      number_prev++;
      return match_pattern(input_line.substr(1), pattern.substr(2), true);
    }
    return flag ? false : match_pattern(input_line.substr(1), pattern, flag);
  } else if (pattern[0] == '[') {
    std::size_t pos = pattern.find_first_of(']');
    groups_prev = pattern.substr(0, pos + 1);
    if (pos != std::string::npos) {
      if (pattern[1] == '^') {
        grep_type_prev |= NEGATIVE_GROUPS;
        if (std::isalpha(input_line[0]) &&
            !match_groups(input_line, pattern.substr(2, pos - 2), flag)) {
          number_prev++;
          return match_pattern(input_line.substr(1), pattern.substr(pos + 1),
                               true);
        }
        return false;
      } else {
        grep_type_prev |= POSITIVE_GROUPS;
        if (match_groups(input_line, pattern.substr(1, pos - 1), flag)) {
          number_prev++;
          return match_pattern(input_line.substr(1), pattern.substr(pos + 1),
                               flag);
        }
        return false;
      }
    }
  } else if (pattern.length() > 1 && pattern[1] == '+') {
    std::size_t pos = 0;
    for (; pattern[0] == input_line[pos]; pos++) {
      number_prev++;
    }
    if (pos) {
      return match_pattern(input_line.substr(pos), pattern.substr(2), true);
    }
    // 如果之前已经匹配上了,后续必须继续匹配
    return flag ? false
                : match_pattern(input_line.substr(pos), pattern.substr(2),
                                false);
  } else if (pattern[0] == '.') {
    // 匹配 .
    number_prev++;
    return match_pattern(input_line.substr(1), pattern.substr(1), true);
  } else if (pattern[0] == '(') {
    // 得到匹配的括号的序号
    std::vector<std::pair<int, int>> paren_vec = find_match_parent(pattern);
    auto iter = paren_vec.front();
    if (paren_vec.size() > 1 &&
        paren_vec.front().second > paren_vec.back().first) {
      // 获得最里层的
      iter = paren_vec.back();
    }
    std::size_t start = iter.first;
    std::size_t end = iter.second;
    if (end != std::string::npos) {
      // 获得括号内容
      std::string pattern_temp = pattern.substr(start + 1, end - start - 1);
      std::vector<std::string> multiple;
      multiple = my_split(pattern_temp, "|");
      if (paren_vec.size() > 1 &&
          paren_vec.front().second > paren_vec.back().first) {
        // 如果是() () 嵌套格式,获取最开始的匹配内容匹配的内容
        // 否则获取到其中的每块内容 (| |)
        multiple = {pattern_temp};
      }
      std::size_t index = 0;
      int number_prev_temp = number_prev; // 匹配的索引
      for (; index < multiple.size(); index++) {
        if (match_pattern(input_line, multiple[index], flag)) {
          break;
        }
      }

      if (index < multiple.size() && end < pattern.size()) {
        if (number_prev > number_prev_temp) {
          if (paren_vec.size() > 1 &&
              paren_vec.front().second > paren_vec.back().first) {
            // () ()
            group_deque.emplace_front(
                input_line.substr(0, number_prev - number_prev_temp));
          } else {
            // ( () )
            group_deque.emplace_back(
                input_line.substr(0, number_prev - number_prev_temp));
          }
          std::string input_line_next =
              input_line.substr(number_prev - number_prev_temp);
          return match_pattern(input_line_next, pattern.substr(end + 1), true);
        }
        return match_pattern(input_line.substr(multiple[index].size()),
                             pattern.substr(end + 1), true);
      }
      return false;
    }
  } else if (pattern[0] == '\\') {
    // 匹配 \1 \2 ...
    size_t pos = 1;
    for (; pos < pattern.size() && std::isdigit(pattern[pos]); pos++) {
    }
    if (pos - 1 == 0) {
      throw std::runtime_error("Unhandled pattern " + pattern);
    }
    // 获取index
    int index = std::stoi(pattern.substr(1, pos - 1));
    std::string temp = "";
    if (index > group_deque.size()) {
      temp = group_deque[group_deque.size() - 1];
    } else {
      temp = group_deque[index - 1];
    }
    // temp 匹配到的内容
    if (match_pattern(input_line, temp, flag)) {
      return match_pattern(input_line.substr(temp.size()), pattern.substr(pos),
                           flag);
    }
    return false;
  } else if (pattern[0] == '+') {

    std::string temp = input_line;
    if (grep_type_prev & DIGIT) {
      for (; match_pattern(temp, R"(\d)", true); temp = temp.substr(1)) {
      }
    } else if (grep_type_prev & ALPHANUMERIC) {
      for (; match_pattern(temp, R"(\w)", true); temp = temp.substr(1)) {
      }
    } else if (grep_type_prev & POSITIVE_GROUPS) {
      for (; match_pattern(temp, groups_prev, true); temp = temp.substr(1)) {
      }
    } else if (grep_type_prev & NEGATIVE_GROUPS) {
      for (; match_pattern(temp, groups_prev, true); temp = temp.substr(1)) {
      }
    }
    grep_type_prev = NONE;
    if (temp.empty()) {
      return true;
    }
    return match_pattern(temp.substr(1), pattern.substr(1), true);
  }
  if (pattern[0] == input_line[0]) {
    // 相等
    number_prev++;
    return match_pattern(input_line.substr(1), pattern.substr(1), true);
  } else {
    return flag ? false : match_pattern(input_line.substr(1), pattern, false);
  }
}

bool match_patterns(std::string &input_line, const std::string &pattern) {
  // input_line 不断遍历每个字符
  for (; !input_line.empty() && !start_anchor;
       input_line = input_line.substr(1)) {
    if (match_pattern(input_line, pattern, false)) {
      // 如果匹配继续
      return true;
    }
    // 清空匹配的[]
    group_deque.clear();
  }
  return false;
}

int main(int argc, char *argv[]) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  // You can use print statements as follows for debugging, they'll be visible
  // when running tests. std::cout << "Logs from your program will appear here"
  // << std::endl;
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
    if (match_patterns(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
