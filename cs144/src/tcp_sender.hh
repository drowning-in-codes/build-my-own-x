#pragma once

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>

#include "byte_stream.hh"
#include "tcp_receiver_message.hh"
#include "tcp_sender_message.hh"

class miniTimer {
 private:
  const uint64_t m_initial_RTO_ms{};
  uint64_t RTO_ms{};
  uint64_t passed_time{};
  bool is_ticking{};
  uint8_t retransmission{};

 public:
  explicit miniTimer(uint64_t t_initial_RTO_ms)
      : m_initial_RTO_ms(t_initial_RTO_ms), RTO_ms(t_initial_RTO_ms) {};
  void reload() {
    RTO_ms = m_initial_RTO_ms;
    retransmission = 0;
  }
  uint8_t get_retransmission() const { return retransmission; }
  void incre_retransmission(uint8_t increment = 1) {
    retransmission += increment;
  }
  void set_retransmission(uint8_t retrans) { retransmission = retrans; }
  void start() {
    passed_time = 0;
    is_ticking = true;
  }
  void stop() {
    passed_time = 0;
    is_ticking = false;
  }
  miniTimer operator+(uint64_t t_passed_time) {
    if (is_ticking) {
      passed_time += t_passed_time;
    }
    return *this;
  }
  void reset_timer() { passed_time = 0; }
  bool is_active() const { return is_ticking; }
  bool is_expired() { return is_active() && (passed_time >= RTO_ms); }
  void exponential_backoff() {
    RTO_ms *= 2;
    passed_time = 0;
  }
};

class TCPSender {
 public:
  /* Construct TCP sender with given default Retransmission Timeout and possible
   * ISN */
  TCPSender(ByteStream&& input, Wrap32 isn, uint64_t initial_RTO_ms)
      : input_(std::move(input)), isn_(isn), initial_RTO_ms_(initial_RTO_ms) {}

  /* Generate an empty TCPSenderMessage */
  TCPSenderMessage make_empty_message() const;

  /* Receive and process a TCPReceiverMessage from the peer's receiver */
  void receive(const TCPReceiverMessage& msg);

  /* Type of the `transmit` function that the push and tick methods can use to
   * send messages */
  using TransmitFunction = std::function<void(const TCPSenderMessage&)>;

  /* Push bytes from the outbound stream */
  void push(const TransmitFunction& transmit);

  /* Time has passed by the given # of milliseconds since the last time the
   * tick() method was called */
  void tick(uint64_t ms_since_last_tick, const TransmitFunction& transmit);

  // Accessors
  uint64_t sequence_numbers_in_flight()
      const;  // How many sequence numbers are outstanding?
  uint64_t consecutive_retransmissions()
      const;  // How many consecutive *re*transmissions have happened?
  Writer& writer() { return input_.writer(); }
  const Writer& writer() const { return input_.writer(); }

  // Access input stream reader, but const-only (can't read from outside)
  const Reader& reader() const { return input_.reader(); }

 private:
  // Variables initialized in constructor
  ByteStream input_;
  Wrap32 isn_;
  uint64_t initial_RTO_ms_;

  uint64_t outstanding_bytes{};
  std::queue<TCPSenderMessage> outstanding_byte_stream{};
  uint16_t windows_size{1};  // 默认window_size 为1
  uint64_t send_index{};
  bool SYN_sent{};
  bool FIN_sent{};
  uint64_t ack_abs_seqno{};
  miniTimer timer_{initial_RTO_ms_};
};
