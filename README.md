# T2CAN Universal for T-2CAN FD (MCP2518FD)

Pacchetto pronto per adattare il firmware T2CAN Universal alla tua scheda **LilyGO T-2CAN FD**.

## Passi rapidi (5 minuti)

1. Clona il sorgente originale:
   ```bash
   git clone https://github.com/06066060606060/t2can-universal-unlock.git
   cd t2can-universal-unlock
   ```

2. Copia questi 2 file da questo repo nella cartella del progetto:
   - `mcp2518fd_compat.h`
   - `pin_config.h` (sovrascrive quello originale)

3. Apri `T2CAN-Universal-v3.30.3-LP_YL.ino` e sostituisci la riga:
   ```cpp
   #include <mcp2515.h>
   ```
   con:
   ```cpp
   #include "mcp2518fd_compat.h"
   ```

4. In `can_core.h` cerca:
   ```cpp
   static constexpr CAN_CLOCK MCP_CLOCK = MCP_16MHZ;
   ```
   e sostituiscila con:
   ```cpp
   static constexpr uint32_t MCP_CLOCK = 40000000UL; // MCP2518FD
   ```

5. Installa la libreria Longan_CANFD:
   https://github.com/Longan-Labs/Longan_CANFD

6. In Arduino IDE:
   - Board: **ESP32S3 Dev Module**
   - Flash Size: 16MB
   - PSRAM: OPI PSRAM
   - Partition Scheme: 16M Flash (3MB APP...)
   - USB CDC On Boot: Enabled

7. Compila e carica (tieni premuto BOOT se serve).

## Note
- Funziona in Classic CAN 500 kbps (adatto a Tesla).
- Se la tua scheda ha cristallo da 20 MHz invece di 40 MHz, cambia `MCP_CLOCK` in `20000000UL`.
- Il log continuerà a dire "MCP2515" ma sta usando il MCP2518FD.

Sorgente originale: https://github.com/06066060606060/t2can-universal-unlock
