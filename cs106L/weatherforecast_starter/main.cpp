/*
Assignment 4: Weather Forecast

This assignment works with template functions and lambdas to become the best
weather forecaster at the Stanford Daily. Be sure to read the assignment details
in the PDF.

Submit to Paperless by 11:59pm on 5/10/2024.
*/

// TODO: import anything from the STL that might be useful!
#include <algorithm>
#include <complex>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

// TODO: write convert_f_to_c function here. Remember it must be a template
// function that always returns a double. [function definition here]

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
double convert_f_to_c(T f) {
  return static_cast<double>((f - 32) * 5 / 9);
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
double get_max(std::vector<T> temps) {
  T max{std::numeric_limits<T>::min()};
  for (const T &temp : temps) {
    if (temp > max) {
      max = temp;
    }
  }
  return max;
}

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
double get_min(std::vector<T> temps) {
  T min{std::numeric_limits<T>::max()};
  for (const T &temp : temps) {
    if (temp < min) {
      min = temp;
    }
  }
  return min;
}

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;
template <Arithmetic T> double get_avg(std::vector<T> temps) {
  // for (const T &temp : temps) {
  //   avg += temp;
  // }
  // T sum = std::ranges::fold_left(temps.begin(), temps.end(), 0,
  // std::plus<T>()); // c++23 T avg = sum / temps.size(); return avg;
  return std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();
}

template <typename Function>
std::vector<double> get_forecast(std::vector<std::vector<double>> readings,
                                 Function fn) {
  std::vector<double> temp;
  // TODO: write get_forecast here!
  for (const std::vector<double> &reading : readings) {
    auto result = fn(reading);
    temp.push_back(result);
  }
  return temp;
}

int main() {
  std::vector<std::vector<double>> readings = {
      {70, 75, 80, 85, 90}, {60, 65, 70, 75, 80}, {50, 55, 60, 65, 70},
      {40, 45, 50, 55, 60}, {30, 35, 40, 45, 50}, {50, 55, 61, 65, 70},
      {40, 45, 50, 55, 60}};

  // TODO: Convert temperatures to Celsius and output to output.txt

  std::ofstream output("output.txt", std ::ios::out | std::ios::trunc);
  if (!output.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  output << "Celsius Temperatures: \n";
  for (auto reading : readings) {
    for (auto r : reading) {
      auto cel_temp = convert_f_to_c<double>(r);
      output << cel_temp << " ";
    }
    output << "\n";
  }

  output << "maxium temperatures: \n";
  // TODO: Find the maximum temperature for each day and output to output.txt
  auto max_temp = get_forecast(readings, get_max<double>);
  std::ranges::for_each(max_temp, [&output](double temp) {
    output << temp << " ";
    std::cout << temp << std::endl;
  });
  output << "\n";

  output << "minimum temperatures: \n";
  // TODO: Find the minimum temperature for each day and output to
  // output.txt
  auto min_temp = get_forecast(readings, get_min<double>);
  std::ranges::for_each(min_temp, [&output](double temp) {
    output << temp << " ";
    std::cout << temp << std::endl;
  });

  output << "\n";
  output << "average temperatures: \n";
  // TODO: Find the average temperature for each day and output to output.txt
  auto avg_temp = get_forecast(readings, get_avg<double>);
  std::ranges::for_each(avg_temp, [&output](double temp) {
    output << temp << " ";
    std::cout << temp << std::endl;
  });
  output.close();

  return 0;
}
