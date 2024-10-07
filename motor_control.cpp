#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

// 위치(Position) 요청 함수
void requestPosition(int sock) {
    struct can_frame frame;
    frame.can_id = 0x601;  // 송신 ID 설정
    frame.can_dlc = 8;     // 데이터 길이 설정
    frame.data[0] = 0x40;  // Command specifier: Read (SDO)
    frame.data[1] = 0x64;  // Index: 6064 (Position)
    frame.data[2] = 0x60;  // Index (continued)
    frame.data[3] = 0x00;  // Subindex
    frame.data[4] = 0x00;  // Reserved (not used)
    frame.data[5] = 0x00;
    frame.data[6] = 0x00;
    frame.data[7] = 0x00;

    // 데이터 송신
    if (write(sock, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        std::cerr << "Error in sending position request frame: " << strerror(errno) << std::endl;
        return;
    }
    std::cout << "Position request SDO sent successfully!" << std::endl;

    // 응답 수신
    struct can_frame recv_frame;
    int nbytes = read(sock, &recv_frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        std::cerr << "Error in receiving frame: " << strerror(errno) << std::endl;
    } else if (recv_frame.can_id == 0x581) {
        std::cout << "Received reply from ID: " << std::hex << recv_frame.can_id << std::endl;
        std::cout << "Data: ";
        for (int i = 0; i < recv_frame.can_dlc; i++) {
            std::cout << std::hex << (int)recv_frame.data[i] << " ";
        }
        std::cout << std::endl;

        // 포지션 값 추출
        int32_t positionValue = recv_frame.data[4] |
                               (recv_frame.data[5] << 8) |
                               (recv_frame.data[6] << 16) |
                               (recv_frame.data[7] << 24);
        std::cout << "Position Value: " << positionValue << std::endl;
    }
}

int main() {
    // 소켓 생성
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        std::cerr << "Error while opening socket: " << strerror(errno) << std::endl;
        return -1;
    }

    // 인터페이스 설정
    struct ifreq ifr;
    std::strcpy(ifr.ifr_name, "can0");
    ioctl(sock, SIOCGIFINDEX, &ifr);

    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "Error in socket bind: " << strerror(errno) << std::endl;
        close(sock);
        return -1;
    }

    // 위치 요청 함수 호출
    requestPosition(sock);

    close(sock);
    return 0;
}
