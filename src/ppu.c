#include "ppu.h"
#include "cpu.h"
#include "memory_bus.h"
#include <stdbool.h>

uint8_t lcd_frame_buffer[(LCD_RES_X * LCD_RES_Y) / 4];

uint8_t vram[0x2000];
uint8_t oam[160];
int mode = MODE_OAM_SCAN;

uint8_t ppu_reg_lcdc = 0, ppu_reg_dma = 0;
uint8_t ppu_reg_scy = 0, ppu_reg_scx = 0, ppu_reg_wy = 0, ppu_reg_wx = 0;
uint8_t ppu_reg_bgp = 0, ppu_reg_obp0 = 0, ppu_reg_obp1 = 0;
uint8_t ppu_reg_ly = 0, ppu_reg_lyc = 0, ppu_reg_stat = 0;

static int dot = 0;
static uint32_t draw_length = 172;
static uint32_t window_line = 0;
static bool stat_line_prev = false;

static uint8_t sprites[10]; // OAM indices, sorted by priority
static int sprite_count = 0;

static void oam_scan(void) {
    int height = (ppu_reg_lcdc & 0x04) ? 16 : 8;
    sprite_count = 0;
    for (int i = 0; i < 40 && sprite_count < 10; i++) {
        int y = oam[i * 4] - 16;
        if (ppu_reg_ly >= y && ppu_reg_ly < y + height)
            sprites[sprite_count++] = i;
    }
    // DMG priority: lower X wins, ties go to lower OAM index.
    // Stable insertion sort keeps OAM order on ties.
    for (int i = 1; i < sprite_count; i++) {
        uint8_t s = sprites[i];
        int j = i - 1;
        while (j >= 0 && oam[sprites[j] * 4 + 1] > oam[s * 4 + 1]) {
            sprites[j + 1] = sprites[j];
            j--;
        }
        sprites[j + 1] = s;
    }
}

static bool window_visible_this_line(void) {
    return (ppu_reg_lcdc & 0x01) && (ppu_reg_lcdc & 0x20) &&
           ppu_reg_ly >= ppu_reg_wy && ppu_reg_wx <= 166;
}

static void estimate_draw_length(void) {
    // Approximation: 172 base + SCX fine scroll + 6/sprite + 6 for window.
    draw_length = 172 + (ppu_reg_scx & 7) + 6 * sprite_count;
    if (window_visible_this_line()) draw_length += 6;
}

// ---- Scanline rendering ----
static uint8_t bg_id[LCD_RES_X];     // raw BG/window color IDs (for sprite priority)
static uint8_t line_shade[LCD_RES_X];

static inline uint8_t tile_pixel(uint16_t tile_addr, int row, int col) {
    uint8_t lo = vram[tile_addr + row * 2];
    uint8_t hi = vram[tile_addr + row * 2 + 1];
    int bit = 7 - col;
    return (((hi >> bit) & 1) << 1) | ((lo >> bit) & 1);
}

static void render_bg_window(void) {
    bool bg_on  = ppu_reg_lcdc & 0x01;
    bool win_on = window_visible_this_line();
    bool win_used = false;

    for (int x = 0; x < LCD_RES_X; x++) {
        uint8_t id = 0;
        if (bg_on) {
            bool in_win = win_on && (x + 7 >= ppu_reg_wx);
            uint8_t px, py;
            uint16_t map;
            if (in_win) {
                px = x + 7 - ppu_reg_wx;
                py = window_line;
                map = (ppu_reg_lcdc & 0x40) ? 0x1C00 : 0x1800;
                win_used = true;
            } else {
                px = (uint8_t)(x + ppu_reg_scx);
                py = (uint8_t)(ppu_reg_ly + ppu_reg_scy);
                map = (ppu_reg_lcdc & 0x08) ? 0x1C00 : 0x1800;
            }
            uint8_t tile = vram[map + (py / 8) * 32 + (px / 8)];
            uint16_t addr = (ppu_reg_lcdc & 0x10)
                ? tile * 16                          // $8000 method
                : 0x1000 + (int8_t)tile * 16;        // $8800 method (base $9000)
            id = tile_pixel(addr, py & 7, px & 7);
        }
        bg_id[x] = id;
        line_shade[x] = (ppu_reg_bgp >> (id * 2)) & 3;
    }
    if (win_used) window_line++;
}

static void render_sprites(void) {
    if (!(ppu_reg_lcdc & 0x02)) return;
    int height = (ppu_reg_lcdc & 0x04) ? 16 : 8;

    // Draw lowest priority first so higher priority overwrites.
    for (int i = sprite_count - 1; i >= 0; i--) {
        const uint8_t *s = &oam[sprites[i] * 4];
        int y = s[0] - 16, xpos = s[1] - 8;
        uint8_t tile = s[2], attr = s[3];
        if (height == 16) tile &= 0xFE;

        int row = ppu_reg_ly - y;
        if (attr & 0x40) row = height - 1 - row;           // Y flip
        uint16_t addr = tile * 16 + row * 2;
        uint8_t pal = (attr & 0x10) ? ppu_reg_obp1 : ppu_reg_obp0;

        for (int px = 0; px < 8; px++) {
            int sx = xpos + px;
            if (sx < 0 || sx >= LCD_RES_X) continue;
            int bit = (attr & 0x20) ? px : 7 - px;         // X flip
            uint8_t id = (((vram[addr + 1] >> bit) & 1) << 1) |
                         ((vram[addr] >> bit) & 1);
            if (id == 0) continue;                         // transparent
            if ((attr & 0x80) && bg_id[sx] != 0) continue; // BG priority
            line_shade[sx] = (pal >> (id * 2)) & 3;
        }
    }
}

static void render_scanline(void) {
    render_bg_window();
    render_sprites();
    for (int x = 0; x < LCD_RES_X; x++) {
        int idx = ppu_reg_ly * LCD_RES_X + x;
        int shift = (idx & 3) * 2;
        uint8_t *b = &lcd_frame_buffer[idx >> 2];
        *b = (*b & ~(3 << shift)) | (line_shade[x] << shift);
    }
}

// ---- Timing ----
static void advance_dot(void) {
    dot++;
    if (dot >= 456) {
        dot = 0;
        ppu_reg_ly = (ppu_reg_ly + 1) % 154;
        if (ppu_reg_ly == 144) {
            window_line = 0;   // reset at VBlank (or at LY==0; either works)
            memory_bus_write(IF_ADDR, memory_bus_read(IF_ADDR) | 0x01);
        }
    }
}

static void update_mode(void) {
    if (ppu_reg_ly >= 144) {
        mode = MODE_VBLANK;
    } else if (dot == 0) {
        mode = MODE_OAM_SCAN;
        oam_scan();                 // doing it all at once is fine for most games
    } else if (dot == 80) {
        mode = MODE_DRAWING;
        estimate_draw_length();
    } else if (dot == 80 + draw_length) {
        mode = MODE_HBLANK;
        render_scanline();
    }
}

static void update_stat_interrupt(void) {
    bool lyc_match = (ppu_reg_ly == ppu_reg_lyc);

    ppu_reg_stat = (ppu_reg_stat & ~0x07) | (lyc_match ? 0x04 : 0) | mode;

    bool line =
        (lyc_match             && (ppu_reg_stat & 0x40)) ||
        (mode == MODE_OAM_SCAN && (ppu_reg_stat & 0x20)) ||
        (mode == MODE_VBLANK   && (ppu_reg_stat & 0x10)) ||
        (mode == MODE_HBLANK   && (ppu_reg_stat & 0x08));

    if (line && !stat_line_prev)
        memory_bus_write(IF_ADDR, memory_bus_read(IF_ADDR) | 0x02);
    stat_line_prev = line;
}

void clock_ppu(void) {
    if (!(ppu_reg_lcdc & 0x80)) {   // LCD off: PPU held in reset
        dot = 0;
        ppu_reg_ly = 0;
        mode = MODE_HBLANK;
        window_line = 0;
        stat_line_prev = false;
        ppu_reg_stat &= ~0x07;
        return;
    }
    for (int ii = 0; ii < 4; ii++) {
        advance_dot();
        update_mode();
        update_stat_interrupt();
    }
}
