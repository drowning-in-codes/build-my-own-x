#include "router.hh"

#include <iostream>
#include <limits>
#include <ranges>
using namespace std;

// route_prefix: The "up-to-32-bit" IPv4 address prefix to match the datagram's
// destination address against prefix_length: For this route to be applicable,
// how many high-order (most-significant) bits of
//    the route_prefix will need to match the corresponding bits of the
//    datagram's destination address?
// next_hop: The IP address of the next hop. Will be empty if the network is
// directly attached to the router (in
//    which case, the next hop address should be the datagram's final
//    destination).
// interface_num: The index of the interface to send the datagram out on.
void Router::add_route(const uint32_t route_prefix, const uint8_t prefix_length,
                       const optional<Address> next_hop,
                       const size_t interface_num) {
  cerr << "DEBUG: adding route "
       << Address::from_ipv4_numeric(route_prefix).ip() << "/"
       << static_cast<int>(prefix_length) << " => "
       << (next_hop.has_value() ? next_hop->ip() : "(direct)")
       << " on interface " << interface_num << "\n";
  uint32_t masked_ip_addr = route_prefix & ~(UINT32_MAX >> prefix_length);
  jumpInfo info{.next_hop = next_hop, .interface_num = interface_num};
  router_table[ip_addr{.masked_ipaddr = masked_ip_addr,
                       .prefix_length = prefix_length}] = info;
}

// Go through all the interfaces, and route every incoming datagram to its
// proper outgoing interface.
void Router::route() {
  for (const auto& interface : _interfaces) {
    auto&& received_datagram{interface->datagrams_received()};
    while (!received_datagram.empty()) {
      InternetDatagram datagram{std::move(received_datagram.front())};
      received_datagram.pop();

      if (datagram.header.ttl <= 1) {
        continue;
      }
      datagram.header.ttl -= 1;
      datagram.header.compute_checksum();
      auto pos = findLongestMatch(datagram.header.dst);
      if (pos) {
        // 找到最长的匹配
        _interfaces[pos.value().interface_num]->send_datagram(
            datagram, pos.value().next_hop.value_or(
                          Address::from_ipv4_numeric(datagram.header.dst)));
      }
    }
  }
}