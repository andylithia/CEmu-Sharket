#include "../../core/cemu.h"
#include "../../core/backlight.h"
#include "keymap_sharket.h"

#include <SDL.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>     
#include <unistd.h>
#include <fcntl.h>

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

typedef struct {
    char *rom;
    char *image;
    int spi;
    int limit;
    int fullscreen;
} cemu_sdl_t;

SDL_Surface* swin;
SDL_Surface* semu;
int rmask, gmask, bmask, amask;
FILE* fp;
char cbuf[5];
uint8_t last_brightness = 0;
bool    hw_brightness = 0;
const char* FN = "./TI84CE.ROM";const char* mipi_brightness_path = "/sys/class/backlight/32e10000.dsi_controller.0/brightness";

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

SDL_Rect rsrc = {
    .x=0,
    .y=0,
    .w=LCD_WIDTH,
    .h=LCD_HEIGHT
};        
SDL_Rect rdst = {
    .x=(WIN_WIDTH-LCD_WIDTH*2)/2,
    .y=(WIN_HEIGHT-LCD_HEIGHT*2)/2,
    .w=LCD_WIDTH*2,
    .h=LCD_HEIGHT*2
};

void gui_console_clear() {}
void gui_console_printf(const char *format, ...) { (void)format; }
void gui_console_err_printf(const char *format, ...) { (void)format; }

// extern bool _rgb;

void sdl_update_lcd(void *data) {
    void *pixels;
    int pitch;
    pixels = semu->pixels;
    // This function draws in a RGBA8888 buffer, with a=255
    emu_lcd_drawframe(pixels);
    if (!hw_brightness) {
        // Software brightness control
        for (int i=0;i<LCD_SIZE;i++) {
            uint32_t p = ((uint32_t*)pixels)[i];
            uint32_t b = (p & 0x00FF0000)>>16;
            uint32_t g = (p & 0x0000FF00)>>8;
            uint32_t r = (p & 0x000000FF);
            r = (uint32_t)((float)r * (last_brightness)/(255.0));
            g = (uint32_t)((float)g * (last_brightness)/(255.0));
            b = (uint32_t)((float)b * (last_brightness)/(255.0));
            p = (r<<16) | (g<<8) | b | 0xFF000000;
            ((uint32_t*)pixels)[i] = p;
        }
    }
}

void sdl_cemu_configure(cemu_sdl_t *cemu) {
    emu_set_run_rate(1000);
    emu_set_lcd_callback(sdl_update_lcd, NULL);
    emu_set_lcd_spi(cemu->spi);
}

bool sdl_cemu_load(cemu_sdl_t *cemu) {
    if (EMU_STATE_VALID != emu_load(EMU_DATA_ROM, cemu->rom)) {
        fprintf(stderr, "could not load rom.\n");
        return false;
    }
    sdl_cemu_configure(cemu);
    return true;
}

#define MAX_FRAME_SKIP 5
#define TICK_K         1000

void sdl_event_loop(cemu_sdl_t *cemu) {
    SDL_Event event;
    bool done = false;
    uint32_t last_ticks, speed_ticks;
    uint32_t max_ticks, ticks, expected_ticks, actual_ticks;
    unsigned speed_count = 0;
    float speed = 0.0f;
    const int fmtflag = 0;
    char buf[20];

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "could not initialize sdl: %s\n", SDL_GetError());
        return;
    }
    
    swin = SDL_SetVideoMode(WIN_WIDTH,WIN_HEIGHT,32,false);
    semu = SDL_CreateRGBSurface(SDL_SWSURFACE,LCD_WIDTH, LCD_HEIGHT,32,rmask,gmask,bmask,amask);
    if ((semu == NULL) || (swin == NULL)) {
        fprintf(stderr, "could not create surface: %s\n", SDL_GetError());
        return;
    }

    if (cemu->image) {
        if (EMU_STATE_VALID != emu_load(EMU_DATA_IMAGE, cemu->image)) {
            fprintf(stderr, "could not load image.\n");
            if (sdl_cemu_load(cemu) == false) {
                return;
            }
        }
    } else if (sdl_cemu_load(cemu) == false) {
        return;
    }

    sdl_cemu_configure(cemu);

    last_ticks = SDL_GetTicks();
    speed_ticks = last_ticks + 1000;
        
    while (done == false) {
        SDL_SoftStretch(semu,&rsrc,swin,&rdst);
        SDL_Flip(swin);
        // Frame Rate Control
        int status;
        max_ticks = TICK_K * MAX_FRAME_SKIP / 60;
        ticks = SDL_GetTicks();
        expected_ticks = ticks - last_ticks;
        last_ticks = ticks;
        actual_ticks = expected_ticks < max_ticks ? expected_ticks : max_ticks;
        actual_ticks = actual_ticks * cemu->limit / 100;
        speed += expected_ticks ? 100.0f * actual_ticks / expected_ticks : 100.0f;
        speed_count++;
        emu_run(actual_ticks);
        
        // Update brightness
        uint8_t brightness = 255 - backlight.brightness;
        if (last_brightness != brightness) {
            last_brightness = brightness;
            if(hw_brightness){
                SDL_itoa(brightness,buf,10);
                int fd = open(mipi_brightness_path, O_WRONLY);
                write(fd, buf, strlen(buf));
                close(fd);
                // fprintf(fp,"%d",brightness);
                // fflush(fp);
            }
            printf("Brightness Updated: %d\n", brightness);
        }
        if (ticks > speed_ticks) {
            speed_ticks += 1000;
            speed_count = 0;
            speed = 0.0f;
        }

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if ((event.key.keysym.mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT)) == KMOD_NONE) {
                        switch (event.key.keysym.sym) {
                            case SDLK_BREAK:
                                if (!sdl_cemu_load(cemu)) {
                                    done = true;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                    fallthrough;
                case SDL_KEYUP:
                    for (const cemu_sdl_key_t* k = sharket_keymap; k && (k->row | k->col) >= 0; k++) {
                        if(k->key==event.key.keysym.sym) {
                            emu_keypad_event(k->row+1, k->col, event.type == SDL_KEYDOWN);
                        }
                    }
                    break;
                case SDL_QUIT:
                    done = true;
            
                    break;
                default:
                    break;
            }
        }
    }
    emu_save(EMU_DATA_IMAGE, cemu->image);
    SDL_Quit();
}

int main(int argc, char **argv) {
    static cemu_sdl_t cemu;
    cemu.limit = 100;
	cemu.rom = FN;
    
    if (fp = open(mipi_brightness_path, "w") == NULL) {
        hw_brightness = false; 
        close(fp);      
    } else {
        printf("Using MIPI brightness control\n");
        hw_brightness = true;
    }
    sdl_event_loop(&cemu);
    return 0;
}
