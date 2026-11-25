#pragma once
// T2CAN FD compatibility shim for LilyGO T-2CAN FD (MCP2518FD)
// Drop-in replacement for the original MCP2515 API used by T2CAN Universal.

#include <Arduino.h>
#include <SPI.h>
#include <mcp2518fd_can.h>

struct can_frame {
  uint32_t can_id;
  uint8_t  can_dlc;
  uint8_t  data[8] __attribute__((aligned(8)));
};

#ifndef CAN_EFF_FLAG
#define CAN_EFF_FLAG 0x80000000U
#endif
#ifndef CAN_RTR_FLAG
#define CAN_RTR_FLAG 0x40000000U
#endif
#ifndef CAN_ERR_FLAG
#define CAN_ERR_FLAG 0x20000000U
#endif

namespace MCP2515Compat {
  enum ERROR {
    ERROR_OK = 0,
    ERROR_FAIL = 1,
    ERROR_ALLTXBUSY = 2,
    ERROR_FAILINIT = 3,
    ERROR_FAILTX = 4,
    ERROR_NOMSG = 5
  };
  static constexpr uint8_t EFLG_RX0OVR = 0x40;
  static constexpr uint8_t EFLG_RX1OVR = 0x80;
  static constexpr uint8_t EFLG_TXBO   = 0x20;
  static constexpr uint8_t EFLG_TXWAR  = 0x10;
  static constexpr uint8_t EFLG_RXWAR  = 0x08;
}

class MCP2515 {
public:
  using ERROR = MCP2515Compat::ERROR;

  static constexpr uint8_t EFLG_RX0OVR = MCP2515Compat::EFLG_RX0OVR;
  static constexpr uint8_t EFLG_RX1OVR = MCP2515Compat::EFLG_RX1OVR;
  static constexpr uint8_t EFLG_TXBO   = MCP2515Compat::EFLG_TXBO;
  static constexpr uint8_t EFLG_TXWAR  = MCP2515Compat::EFLG_TXWAR;
  static constexpr uint8_t EFLG_RXWAR  = MCP2515Compat::EFLG_RXWAR;

  MCP2515(uint8_t cs, uint32_t spiHz = 10000000, SPIClass* spi = &SPI)
    : csPin(cs), spiSpeed(spiHz), pSpi(spi), fd(cs) {}

  ERROR reset() { return ERROR_OK; }

  ERROR setBitrate(uint32_t bitrate, uint32_t clock = 40000000) {
    (void)bitrate; (void)clock;
    lastClock = clock;
    return ERROR_OK;
  }

  ERROR setNormalMode() { return doBegin(false); }
  ERROR setListenOnlyMode() { return doBegin(true); }

  ERROR sendMessage(const struct can_frame* frame) {
    if (!frame) return ERROR_FAIL;
    bool ext = (frame->can_id & CAN_EFF_FLAG) != 0;
    uint32_t id = frame->can_id & (ext ? 0x1FFFFFFFUL : 0x7FFUL);
    uint8_t dlc = frame->can_dlc > 8 ? 8 : frame->can_dlc;
    byte res = fd.sendMsgBuf(id, ext ? 1 : 0, 0, dlc, frame->data);
    return (res == CAN_OK) ? ERROR_OK : ERROR_FAIL;
  }

  ERROR readMessage(struct can_frame* frame) {
    if (!frame) return ERROR_FAIL;
    unsigned long id = 0;
    byte len = 0;
    byte buf[8] = {};
    byte ext = 0;
    byte rtr = 0;
    byte status = fd.readRxTxStatus();
    if (fd.readMsgBufID(status, &id, &ext, &rtr, &len, buf) != CAN_OK)
      return ERROR_NOMSG;
    frame->can_id = id & (ext ? 0x1FFFFFFFUL : 0x7FFUL);
    if (ext) frame->can_id |= CAN_EFF_FLAG;
    if (rtr) frame->can_id |= CAN_RTR_FLAG;
    frame->can_dlc = len > 8 ? 8 : len;
    memcpy(frame->data, buf, frame->can_dlc);
    return ERROR_OK;
  }

  uint8_t getErrorFlags() {
    CAN_ERROR_STATE st = {};
    fd.mcp2518fd_ErrorStateGet(&st);
    uint8_t eflg = 0;
    if (st.TxErrorStateBusOff) eflg |= EFLG_TXBO;
    if (st.TxErrorStateWarning) eflg |= EFLG_TXWAR;
    if (st.RxErrorStateWarning) eflg |= EFLG_RXWAR;
    return eflg;
  }

  void clearRXnOVR() {}

private:
  uint8_t csPin;
  uint32_t spiSpeed;
  SPIClass* pSpi;
  mcp2518fd fd;
  uint32_t lastClock = 40000000;

  ERROR doBegin(bool listenOnly) {
    byte clk = (lastClock >= 30000000) ? MCP2518FD_40MHz : MCP2518FD_20MHz;
    byte speed = CAN20_500KBPS;
    byte res = fd.begin(speed, clk);
    if (res != CAN_OK) return ERROR_FAILINIT;
    if (listenOnly) fd.setMode(CAN_LISTEN_ONLY_MODE);
    else fd.setMode(CAN_NORMAL_MODE);
    fd.__flgFDF = 0;
    return ERROR_OK;
  }
};

#ifndef MCP_16MHZ
#define MCP_16MHZ 16000000UL
#endif
#ifndef MCP_8MHZ
#define MCP_8MHZ 8000000UL
#endif
#ifndef MCP_40MHZ
#define MCP_40MHZ 40000000UL
#endif
#ifndef CAN_500KBPS
#define CAN_500KBPS 500000UL
#endif
