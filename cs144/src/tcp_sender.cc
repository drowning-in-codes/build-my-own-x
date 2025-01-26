#include "tcp_sender.hh"

#include <iostream>

#include "tcp_config.hh"
using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const {
  // Your code here.
  return outstanding_bytes;
}

uint64_t TCPSender::consecutive_retransmissions() const {
  // Your code here.
  return timer_.get_retransmission();
}

void TCPSender::push(const TransmitFunction& transmit) {
  while ((windows_size == 0 ? 1 : windows_size) > outstanding_bytes) {
    // window_size大于 outstanding_bytes,发送数据. 如果window_size ==0,当作1
    //  相当于
    //  如果还有一堆数据没有接受,而window_size小于这么多数据,就不继续发送了
    if (FIN_sent) {
      // 发送FIN后push直接退出
      break;
    }
    TCPSenderMessage msg{make_empty_message()};
    if (!SYN_sent) {
      // 初始化
      msg.SYN = true;
      SYN_sent = true;
    }
    // windows_size - outstanding_bytes   相当于还需要发的数据
    const uint64_t remaining{(windows_size == 0 ? 1 : windows_size) -
                             outstanding_bytes};
    // window_size如果为0,需要处理特殊情况,把它当作1读取
    const size_t len{min(
        TCPConfig::MAX_PAYLOAD_SIZE,
        remaining -
            msg.SYN)};  // 剩下的空闲空间再减去SYN(如果有的话)
                        // 相当于发送的数据是包含SYN和FIN这两个并没有实际数据位的
    read(input_.reader(), len, msg.payload);

    if (!FIN_sent && remaining > msg.sequence_length() &&
        reader().is_finished()) {
      // 结束   还有剩余数据  remaining是需要读取的数据
      // msg.sequence_length()是实际读取的数据
      // 如果实际读取的数据更少,说明已经读完
      // read读取完毕
      msg.FIN = true;
      FIN_sent = true;
    }
    if (msg.sequence_length() == 0) {
      break;
    }
    transmit(msg);
    if (!timer_.is_active()) {
      timer_.start();
    }
    send_index += msg.sequence_length();
    outstanding_bytes += msg.sequence_length();
    outstanding_byte_stream.push(std::move(msg));
  }
}

TCPSenderMessage TCPSender::make_empty_message() const {
  // Your code here.
  TCPSenderMessage sender_msg{};
  sender_msg.seqno = Wrap32::wrap(send_index, isn_);
  sender_msg.RST = input_.has_error();
  return sender_msg;
}

void TCPSender::receive(const TCPReceiverMessage& msg) {
  if (input_.has_error()) {
    return;
  }
  if (msg.RST) {
    input_.set_error();
    return;
  }
  windows_size = msg.window_size;
  if (msg.ackno) {
    const uint64_t abs_ackno{msg.ackno.value().unwrap(isn_, send_index)};
    if (abs_ackno > send_index) {
      return;
    }
    bool has_ack{};
    while (!outstanding_byte_stream.empty()) {
      const auto& message = outstanding_byte_stream.front();
      if (ack_abs_seqno + message.sequence_length() > abs_ackno) {
        // 必须要完全ack才能删掉,否则需要重传
        break;
      }
      has_ack = true;
      ack_abs_seqno += message.sequence_length();
      outstanding_bytes -= message.sequence_length();
      outstanding_byte_stream.pop();
    }
    if (has_ack) {
      // ack之后重置timer和retransmisson, 如果还有outstanding_bytes
      // 开启timer_,否则取消
      timer_.reload();
      outstanding_byte_stream.empty() ? timer_.stop() : timer_.start();
    }
  }
}

void TCPSender::tick(uint64_t ms_since_last_tick,
                     const TransmitFunction& transmit) {
  if ((timer_ + ms_since_last_tick).is_expired()) {
    if (!outstanding_byte_stream.empty()) {
      if (timer_.get_retransmission() > TCPConfig::MAX_RETX_ATTEMPTS) {
        reader().has_error();
        writer().close();
        return;
      }
      // 如果超时并且还有数据需要重传
      // 重传数据
      // 重传之后double等待时间并reset timer
      // 如果window_size = 0,不需要.
      transmit(outstanding_byte_stream.front());
      if (windows_size != 0) {
        timer_.incre_retransmission();
        timer_.exponential_backoff();
      }
      timer_.start();
    }
  }
}
