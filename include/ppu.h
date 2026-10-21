#ifndef PPU_H
#define PPU_H

#include "stdint.h"

#define LCD_RES_X 160 // 160 pixels wide
#define LCD_RES_Y 144 // 144 pixels tall

#define PPU_REG_LCDC 0xFF40
#define PPU_REG_STAT 0xFF41
#define PPU_REG_SCY 0xFF42
#define PPU_REG_SCX 0xFF43
#define PPU_REG_LY 0xFF44
#define PPU_REG_LYC 0xFF45
#define PPU_REG_DMA 0xFF46
#define PPU_REG_BGP 0xFF47
#define PPU_REG_OBP0 0xFF48
#define PPU_REG_OBP1 0xFF49
#define PPU_REG_WY 0xFF4A
#define PPU_REG_WX 0xFF4B

#define MODE_HBLANK 0
#define MODE_VBLANK 1
#define MODE_OAM_SCAN 2
#define MODE_DRAWING 3

#define VRAM_BEGIN 0x8000
#define VRAM_END 0x9FFF
#define VRAM_SIZE (VRAM_END - VRAM_BEGIN + 1)

#define OAM_BEGIN 0xFE00
#define OAM_END 0xFE9F
#define OAM_SIZE (OAM_END - OAM_BEGIN + 1)

// each 8-bit num represents 4 2-bit pallete indexes for that
// equivalent pixel, "big-endian"
// also of note, DMG stores the 2-bit pallete indexes in reverse order,
// however rather than use operations to reverse the bits, the pallete order has been changed to reflect this
// i.e. 0b01100011 -> pixel 0: 3, 1: 0, 2: 1, 3: 2
extern uint8_t lcd_frame_buffer[(LCD_RES_X * LCD_RES_Y) / 4];
extern uint8_t vram[VRAM_SIZE];
extern uint8_t oam[OAM_SIZE];

extern uint8_t ppu_reg_lcdc, ppu_reg_dma;
extern uint8_t ppu_reg_scy, ppu_reg_scx, ppu_reg_wy, ppu_reg_wx;
extern uint8_t ppu_reg_bgp, ppu_reg_obp0, ppu_reg_obp1;
extern uint8_t ppu_reg_ly, ppu_reg_lyc, ppu_reg_stat;

extern int mode;

void clock_ppu(); // emulate a clock tick on the picture processing unit


#endif // PPU_H
