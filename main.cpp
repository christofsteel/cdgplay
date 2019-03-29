#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>

#include "libCDG/include/libCDG.h"

void end_function [[noreturn]] () {
    exit(0);
}

int main(int argc, char ** argv) {
    if (argc == 1) {
        exit(1);
    }
    std::string cdgfilename(argv[1]);
    uint monitor = stol(argv[2]);

    std::string mp3filename(cdgfilename);
    uint64_t fnl = mp3filename.size();
    mp3filename.resize(fnl - 3, '.');
    mp3filename.append("mp3");
    SDL_Window *window = nullptr;
    SDL_Surface *screenSurface = nullptr; // Initialize SDL

    const int SCREEN_WIDTH = 300;
    const int SCREEN_HEIGHT = 216;

    int result = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    } else {
        uint32_t window_flags = SDL_WINDOW_FULLSCREEN_DESKTOP; //SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
         
        window = SDL_CreateWindow("cdgplay", 
                SDL_WINDOWPOS_UNDEFINED_DISPLAY(monitor),
                                  SDL_WINDOWPOS_UNDEFINED_DISPLAY(monitor), 
                                  SCREEN_WIDTH,
                                  SCREEN_HEIGHT, window_flags);
    }
    int flags = MIX_INIT_MP3;
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if (flags != (result = Mix_Init(flags))) {
        printf("Could not initialize mixer (result: %d).\n", result);
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(
                            renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 300, 216);

    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    Mix_Music *music = Mix_LoadMUS(mp3filename.c_str());
    Mix_PlayMusic(music, 1);
    uint32_t starttime = SDL_GetTicks();
    CDG cdgfile;
    cdgfile.FileOpen(cdgfilename.c_str());
    cdgfile.Process();
    unsigned char *pixmap;
    cdgfile.SkipFrame(static_cast<int>(0));
    pixmap = cdgfile.GetImageByTime(0);
    int pitch = 900;
    screenSurface = SDL_GetWindowSurface(window);
    Mix_HookMusicFinished(end_function);

    SDL_Event e;
    bool quit = false;
    try{
        while(!quit) {
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    quit = true;
                }
            }

            uint32_t position = SDL_GetTicks() - starttime;
            cdgfile.SkipFrame(static_cast<int>(position));
            SDL_LockTexture(texture, nullptr, reinterpret_cast<void**>(&pixmap), &pitch);
            uint8_t* pixmap2 = cdgfile.GetImageByTime(position);
            memcpy(pixmap, pixmap2, 900 * 216);
            SDL_UnlockTexture(texture);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            delete (pixmap2);
            SDL_Delay(1);
        }
    } catch (const std::out_of_range) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}
