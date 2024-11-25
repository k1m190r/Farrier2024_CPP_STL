#include <cstdint>
#include <print>
#include <span>

using std::print, std::span;

int main() {
  /************************************************************************************/
  {
    using ui16 = uint16_t;
    using ui8 = uint8_t;

    struct UDP_H {
      ui16 src_port{0}, dst_port{0}, length{0}, checksum{0};

      void display() const {
        print(
            "Source Port: {}\n"
            "Destination Port: {}\n"
            "Length: {}\n"
            "Checksum: {}\n",
            src_port, dst_port, length, checksum);
      }
    };

    auto process_UDP_packet = [](span<const ui8> packet) {
      print("packet size: {}\n", packet.size());

      if (packet.size() < sizeof(UDP_H)) {
        print(stderr, "Invalid packet size!\n");
        return;
      }

      auto header_span = packet.subspan(0, sizeof(UDP_H));

      auto cast = [](auto x) { return *reinterpret_cast<const UDP_H *>(x); };
      const UDP_H &header = cast(header_span.data());

      header.display();

      auto data_span = packet.subspan(sizeof(UDP_H));
      print("Data size: {} bytes\n", data_span.size());
    };

    ui8 udp_packet[] = {
        //
        0x08, 0x15,        // Source port
        0x09, 0x16,        // Destination port
        0x00, 0x10,        // Length
        0x12, 0x34,        // Checksum
        0x01, 0x02, 0x03,  // Some data
        0x04, 0x05, 0x06,  //
    };

    process_UDP_packet(udp_packet);
  }
}