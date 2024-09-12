#include "network_interface.hh"

#include <iostream>
#include <ranges>

#include "arp_message.hh"
#include "exception.hh"

using namespace std;

//! \param[in] ethernet_address Ethernet (what ARP calls "hardware") address of
//! the interface \param[in] ip_address IP (what ARP calls "protocol") address
//! of the interface
NetworkInterface::NetworkInterface(string_view name,
                                   shared_ptr<OutputPort> port,
                                   const EthernetAddress& ethernet_address,
                                   const Address& ip_address)
    : name_(name),
      port_(notnull("OutputPort", move(port))),
      ethernet_address_(ethernet_address),
      ip_address_(ip_address) {
  cerr << "DEBUG: Network interface has Ethernet address "
       << to_string(ethernet_address) << " and IP address " << ip_address.ip()
       << "\n";
}

//! \param[in] dgram the IPv4 datagram to be sent
//! \param[in] next_hop the IP address of the interface to send it to (typically
//! a router or default gateway, but may also be another host if directly
//! connected to the same network as the destination) Note: the Address type can
//! be converted to a uint32_t (raw 32-bit IP address) by using the
//! Address::ipv4_numeric() method.
void NetworkInterface::send_datagram(const InternetDatagram& dgram,
                                     const Address& next_hop) {
  // get ip address
  const auto ipv4_numeric = next_hop.ipv4_numeric();
  if (address_mappings_.count(ipv4_numeric)) {
    // 包括这个ip地址对应的mac地址 发送数据
    EthernetAddress& dst = address_mappings_[ipv4_numeric];
    EthernetHeader ethernet_header{.dst = dst,
                                   .src = ethernet_address_,
                                   .type = EthernetHeader::TYPE_IPv4};
    EthernetFrame ethernetframe{.header = ethernet_header,
                                .payload = serialize(dgram)};
    transmit(ethernetframe);
  } else {
    // mappings中没有
    if (arp_timer_.count(ipv4_numeric) == 0 ||
        arp_timer_[ipv4_numeric] >= 5 * 1000UL) {
      queued_datagrams_.push_back(
          IP_datagrams{.next_hop = ipv4_numeric, .datagrams = dgram});
      // 如果已经发送过arp 并且时间超过5s或者没有发送过,则继续发送arp数据 并将待发送ip数据包暂存
      EthernetHeader ethernet_header{.dst = ETHERNET_BROADCAST,
                                     .src = ethernet_address_,
                                     .type = EthernetHeader::TYPE_ARP};
      ARPMessage arp_msg{.opcode = ARPMessage::OPCODE_REQUEST,
                         .sender_ethernet_address = ethernet_address_,
                         .sender_ip_address = ip_address_.ipv4_numeric(),
                         .target_ethernet_address = EthernetAddress{},
                         .target_ip_address = ipv4_numeric};
      EthernetFrame ethernetframe{.header = ethernet_header,
                                  .payload = serialize(arp_msg)};

      // 重置计时  发送arp并将信息保存
      arp_timer_[ipv4_numeric] = 0;
      transmit(ethernetframe);
    }
  }
}

//! \param[in] frame the incoming Ethernet frame
void NetworkInterface::recv_frame(const EthernetFrame& frame) {
  if (frame.header.type == EthernetHeader::TYPE_IPv4 &&
      frame.header.dst == ethernet_address_) {
    // 如果是数据
    // 接收数据, 更新mappings
    // 更新map
    InternetDatagram parsed_datagram{};
    auto parse_success = parse(parsed_datagram, frame.payload);
    if (parse_success == true) {
      uint32_t sender_ip = parsed_datagram.header.src;
      EthernetAddress sender_mac = frame.header.src;
      address_mappings_[sender_ip] = sender_mac;
      if (!mapping_timer.count(sender_ip)) {
        mapping_timer[sender_ip] = 0;
      }
      auto it = std::ranges::remove_if(
          queued_datagrams_,
          [this, &sender_ip, &sender_mac](const IP_datagrams& p) {
            // 发送的数据的头等于之前找不到mac地址的数据的ip头
            if (p.next_hop == sender_ip) {
              EthernetHeader ethernet_header{.dst = sender_mac,
                                             .src = ethernet_address_,
                                             .type = EthernetHeader::TYPE_IPv4};
              EthernetFrame ethernet_frame{.header = ethernet_header,
                                           .payload = serialize(p.datagrams)};
              transmit(ethernet_frame);
              return true;
            }
            return false;
          });
      queued_datagrams_.erase(it.begin(), queued_datagrams_.end());
      datagrams_received_.push(std::move(parsed_datagram));
    }
  } else {
    ARPMessage parsed_arp{};
    bool parse_success = parse(parsed_arp, frame.payload);
    if (parse_success) {
      // 不管是否是请求还是回应
      // 记住mappings
      auto target_ip = parsed_arp.target_ip_address;
      auto sender_ip = parsed_arp.sender_ip_address;
      if (!mapping_timer.count(sender_ip)) {
        mapping_timer[sender_ip] = 0;
      }
      auto sender_address = parsed_arp.sender_ethernet_address;
      address_mappings_[sender_ip] = sender_address;
      if (parsed_arp.opcode == ARPMessage::OPCODE_REPLY) {
        arp_timer_.erase(sender_ip);
        auto it = std::ranges::remove_if(
            queued_datagrams_,
            [this, &sender_ip, &sender_address](const IP_datagrams& data) {
              if (data.next_hop == sender_ip) {
                EthernetHeader ethernet_header{
                    .dst = sender_address,
                    .src = ethernet_address_,
                    .type = EthernetHeader::TYPE_IPv4};
                EthernetFrame ethernetframe{
                    .header = ethernet_header,
                    .payload = serialize(data.datagrams)};
                transmit(ethernetframe);
                return true;
              }
              return false;
            });
        queued_datagrams_.erase(it.begin(), queued_datagrams_.end());
      } else if (parsed_arp.opcode == ARPMessage::OPCODE_REQUEST &&
                 target_ip == ip_address_.ipv4_numeric()) {
        // 如果是请求并且请求的是自己 就发送arp回应
        // 需要这个网口的mac地址 发送arp
        ARPMessage arp_res{.opcode = ARPMessage::OPCODE_REPLY,
                           .sender_ethernet_address = ethernet_address_,
                           .sender_ip_address = ip_address_.ipv4_numeric(),
                           .target_ethernet_address = sender_address,
                           .target_ip_address = sender_ip};
        EthernetHeader ethernet_header{.dst = sender_address,
                                       .src = ethernet_address_,
                                       .type = EthernetHeader::TYPE_ARP};
        EthernetFrame ethernet_frame{.header = ethernet_header,
                                     .payload = serialize(arp_res)};
        transmit(ethernet_frame);
      }
    }
  }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last
//! call to this method
void NetworkInterface::tick(const size_t ms_since_last_tick) {
  // Your code here.
  for (auto& pos : arp_timer_) {
    pos.second += ms_since_last_tick;
  }
  auto mapping_timer_pos = mapping_timer.begin();
  while (mapping_timer_pos != mapping_timer.end()) {
    mapping_timer_pos->second += ms_since_last_tick;
    // 超过30秒,删除对应mappings
    if (mapping_timer_pos->second >= 30 * 1000UL) {
      std::erase_if(
          address_mappings_,
          [mapping_timer_pos](const std::pair<uint32_t, EthernetAddress>& m) {
            return m.first == mapping_timer_pos->first;
          });
      // 删除mappings同时计时器删除
      mapping_timer_pos = mapping_timer.erase(mapping_timer_pos);
    } else {
      mapping_timer_pos++;
    }
  }
}
