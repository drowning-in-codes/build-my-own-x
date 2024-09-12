#pragma once

#include <compare>
#include <iostream>
#include <memory>
#include <optional>

#include "exception.hh"
#include "network_interface.hh"

// \brief A router that has multiple network interfaces and
// performs longest-prefix-match routing between them.
class Router {
 public:
  // Add an interface to the router
  // \param[in] interface an already-constructed network interface
  // \returns The index of the interface after it has been added to the router
  size_t add_interface(std::shared_ptr<NetworkInterface> interface) {
    _interfaces.push_back(notnull("add_interface", std::move(interface)));
    return _interfaces.size() - 1;
  }

  // Access an interface by index
  std::shared_ptr<NetworkInterface> interface(const size_t N) {
    return _interfaces.at(N);
  }

  // Add a route (a forwarding rule)
  void add_route(uint32_t route_prefix, uint8_t prefix_length,
                 std::optional<Address> next_hop, size_t interface_num);

  // Route packets between the interfaces
  void route();

 private:
  // The router's collection of network interfaces
  std::vector<std::shared_ptr<NetworkInterface>> _interfaces{};
  struct jumpInfo {
    std::optional<Address> next_hop{};
    size_t interface_num{};
  };
  struct ip_addr {
    uint32_t masked_ipaddr{};
    uint8_t prefix_length{};
    constexpr bool operator<(const ip_addr& ip_) const {
      if(prefix_length != ip_.prefix_length) {
        return prefix_length<ip_.prefix_length;
      }
      return masked_ipaddr<ip_.masked_ipaddr;
    };
  };
  std::map<ip_addr, jumpInfo> router_table{};
  std::optional<jumpInfo> findLongestMatch(uint32_t dst_ip) {
    uint8_t longest_prefix{};
    std::optional<jumpInfo> longestMatch{};
    for (const auto& [masked_ip_addr, jump_info] : router_table) {
      if ((dst_ip & ~(UINT32_MAX >> masked_ip_addr.prefix_length)) ==
          masked_ip_addr.masked_ipaddr) {
        if (masked_ip_addr.prefix_length >= longest_prefix) {
          longestMatch = jump_info;
          longest_prefix = masked_ip_addr.prefix_length;
        }
      }
    }
    return longestMatch;
  }
};
