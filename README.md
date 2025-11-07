# MonkaDriver

*(Trình gà) Tập tành code lại phần mềm điều khiển bàn phím Monka 3075*  
*(Đã bán)*

---

## Tổng quan / Overview

Để gửi **một lệnh đổi LED (apply preset)** tới firmware của **Monka 3075**, cần tổng cộng **5 packet**, mỗi packet **64 bytes** (`HID SET_REPORT / Output Report`):

1. Start packet #1  
2. Start packet #2  
3. Payload packet *(chứa thông tin preset / màu / speed / brightness / direction)*  
4. End packet #1  
5. End packet #2  

---

## Layout chi tiết (0-indexed offsets)

### Payload mẫu (64 bytes)

| Offset | Name | Description |
|:------:|------|-------------|
| 0 | `preset-id` | ID của hiệu ứng (preset) |
| 1 | `R` | Red |
| 2 | `G` | Green |
| 3 | `B` | Blue |
| 4–7 | reserved (`0x00`) | không dùng |
| 8 | `color mode` | `0x00` = one-color, `0x01` = RGB/rainbow, ... |
| 9 | `brightness` | độ sáng (0 -> 5) |
| 10 | `speed` | tốc độ hiệu ứng (1 -> 5) |
| 11 | `direction` | 0 = RIGHT, 1 = LEFT, 2 = UP, 3 = DOWN |
| 12–13 | reserved |
| 14 | `0xAA` | magic byte #1 |
| 15 | `0x55` | magic byte #2 |
| 16–63 | padding (`0x00`) |

> Byte index ở trên là **0-indexed** (tức byte đầu tiên là offset 0).  
> **R, G, B** là 3 kênh màu (Red, Green, Blue).  
> `magic bytes (0xAA 0x55)` là marker để firmware xác nhận gói payload hợp lệ.

---

## Start / End packets (64 bytes)

| Packet | Description | Values |
|--------|--------------|---------|
| Start #1 (`s1`) | Begin sequence | `s1[0] = 0x04`, `s1[1] = 0x18`, others `0x00` |
| Start #2 (`s2`) | Handshake stage 2 | `s2[0] = 0x04`, `s2[1] = 0x01`, `s2[8] = 0x01`, others `0x00` |
| End #1 (`e1`) | End stage 1 | `e1[0] = 0x04`, `e1[1] = 0x02`, others `0x00` |
| End #2 (`e2`) | End stage 2 | `e2[0] = 0x04`, `e2[1] = 0xF0`, others `0x00` |

> Các packet `start` và `end` **cố định** (đã reverse).  
> Firmware **yêu cầu đủ 5 packet** theo thứ tự để nhận và apply thay đổi LED.

---

## Example — pseudo C++ code

```cpp
// giả sử hid.WriteReport(buf, 64) gửi 1 HID output report

uint8_t s1[64] = {0};
s1[0] = 0x04; s1[1] = 0x18;

uint8_t s2[64] = {0};
s2[0] = 0x04; s2[1] = 0x01; s2[8] = 0x01;

uint8_t payload[64] = {0};
payload[0] = presetId;
payload[1] = r; payload[2] = g; payload[3] = b;
payload[8] = colorMode;
payload[9] = brightness;
payload[10] = speed;
payload[11] = direction;
payload[14] = 0xAA; payload[15] = 0x55;

uint8_t e1[64] = {0};
e1[0] = 0x04; e1[1] = 0x02;

uint8_t e2[64] = {0};
e2[0] = 0x04; e2[1] = 0xF0;

// send sequence (with small delay)
hid.WriteReport(s1, 64); Sleep(5);
hid.WriteReport(s2, 64); Sleep(5);
hid.WriteReport(payload, 64); Sleep(5);
hid.WriteReport(e1, 64); Sleep(5);
hid.WriteReport(e2, 64); Sleep(5);
