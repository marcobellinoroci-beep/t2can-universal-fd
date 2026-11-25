# T2CAN Universal — T-2CAN FD (MCP2518FD) Compatibility

Firmware T2CAN Universal adattato per la scheda **LilyGO T-2CAN FD** (H784-A) con controller **MCP2518FD**.

## Come usarlo

1. Clona il repo originale:
   ```bash
   git clone https://github.com/06066060606060/t2can-universal-unlock.git
   cd t2can-universal-unlock
   ```

2. Copia il file `mcp2518fd_compat.h` da questo repository nella cartella del progetto originale.

3. Apri `T2CAN-Universal-v3.30.3-LP_YL.ino` e sostituisci:
   ```cpp
   #include <mcp2515.h>
   ```
   con:
   ```cpp
   #include "mcp2518fd_compat.h"
   ```

4. (Opzionale) In `can_core.h` cambia il clock:
   ```cpp
   static constexpr uint32_t MCP_CLOCK = 40000000UL;
   ```

5. Installa la libreria **Longan_CANFD**:
   https://github.com/Longan-Labs/Longan_CANFD

6. In Arduino IDE seleziona **ESP32S3 Dev Module**, Flash 16MB, PSRAM OPI, e compila.

## Note
- Configurato in Classic CAN 500 kbps (adatto ai bus Tesla).
- I pin SPI/CS/RST sono gli stessi della versione non-FD.
- Se il cristallo della tua scheda è da 20 MHz invece di 40 MHz, modifica `MCP_CLOCK` di conseguenza.

Repo originale: https://github.com/06066060606060/t2can-universal-unlock
