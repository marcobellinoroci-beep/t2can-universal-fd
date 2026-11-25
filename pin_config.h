/*
 * T-2CAN / T-2CAN FD pin map
 * Compatible with both LilyGO T-2CAN (MCP2515) and T-2CAN FD (MCP2518FD)
 * SPI / CS / RST pins are identical on H784 and H784-A.
 */
#pragma once

// CAN B / ESP32-S3 TWAI
#define CAN_TX 7
#define CAN_RX 6

// Shared SPI pins
#define SPI_SCLK 12
#define SPI_MOSI 11
#define SPI_MISO 13

// CAN A / MCP2518FD (or MCP2515)
#define MCP2515_CS 10
#define MCP2515_SCLK SPI_SCLK
#define MCP2515_MOSI SPI_MOSI
#define MCP2515_MISO SPI_MISO
#define MCP2515_RST 9

#define MCP2518FD_CS MCP2515_CS
#define MCP2518FD_RST MCP2515_RST
