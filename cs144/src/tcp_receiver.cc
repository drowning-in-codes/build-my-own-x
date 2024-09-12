#include "tcp_receiver.hh"

#include <cassert>
using namespace std;

void TCPReceiver::receive(TCPSenderMessage message) {
  // Your code here.
  if (message.RST) {
    reassembler_.reader().set_error();
  } else if (message.SYN) {
    zero_point = message.seqno;
    reassembler_.insert(0, message.payload, message.FIN);
  } else if (zero_point) {
    uint64_t bytes_pushed = writer().bytes_pushed();
    uint64_t first_index =
        message.seqno.unwrap(zero_point.value(), bytes_pushed + 1) - 1;
    reassembler_.insert(first_index, message.payload, message.FIN);
  }
  if (writer().is_closed()) {
    FIN = true;
  } else {
    FIN = false;
  }
}

TCPReceiverMessage TCPReceiver::send() const {
  // Your code here.
  TCPReceiverMessage recv_message{};
  if (reassembler_.reader().has_error()) {
    recv_message.RST = true;
  }
  if (zero_point) {
    recv_message.ackno = zero_point.value() + 1 + writer().bytes_pushed() + FIN;
  }

  recv_message.window_size = writer().available_capacity() > UINT16_MAX
                                 ? UINT16_MAX
                                 : writer().available_capacity();
  return recv_message;
}
