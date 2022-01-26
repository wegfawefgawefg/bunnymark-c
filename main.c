#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// const int SCREEN_WIDTH = 1920;
// const int SCREEN_HEIGHT = 1080;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


struct point {
    int x, y;
};
struct dimensions {
    int width, height;
};
struct entity {
    int x, y;
    int vx, vy;
};

const float rand_range(float min, float max) {
    return min + (max - min) * (rand() % RAND_MAX / (float)RAND_MAX);
}

void draw_text(
        const SDL_Renderer *renderer, 
        TTF_Font *font, 
        const char *text,
        const SDL_Color color, 
        const int x, const int y) {
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

int main(int argc, char *argv[])
{

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 16);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_Window *window = SDL_CreateWindow("BUNNYMARK", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Event event;

    // Load font
    TTF_Font *font = TTF_OpenFont("../assets/FreeSans.ttf", 24);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    // load bunny
    SDL_Texture *bunny_texture = IMG_LoadTexture(renderer, "../assets/wabbit_alpha.png");
    int width, height;
    SDL_QueryTexture(bunny_texture, NULL, NULL, &width, &height);
    const struct dimensions bunny_shape = {width, height};

    // initialize bunnies
    const int NUM_BUNNIES = 180000;
    const int MAX_VELOCITY = 10;
    struct entity bunnies[NUM_BUNNIES];
    for (int i = 0; i < NUM_BUNNIES; i++) {
        bunnies[i].x = rand_range(0, SCREEN_WIDTH);
        bunnies[i].y = rand_range(0, SCREEN_HEIGHT);
        bunnies[i].vx = rand_range(-MAX_VELOCITY, MAX_VELOCITY);
        bunnies[i].vy = rand_range(-MAX_VELOCITY, MAX_VELOCITY);
    }

    Uint64 frame_time_now = SDL_GetPerformanceCounter();
    Uint64 frame_time_last = 0;
    double dt = 0;

    while(1) {
        frame_time_last = frame_time_now;
        frame_time_now = SDL_GetPerformanceCounter();
        dt = (double)((frame_time_now - frame_time_last)*1000 / (double)SDL_GetPerformanceFrequency() );
        // calculate fps
        const double fps = 1000.0 / dt;        

        // handle user inputs
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_Q]) {
            break;
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // update and draw bunnies
        for(int i = 0; i < NUM_BUNNIES; i++) {
            struct entity bunny = bunnies[i];
            bunny.x += bunny.vx;
            bunny.y += bunny.vy;
            // apply gravity
            bunny.vy += 1;
            if (bunny.x < 0) {
                bunny.x = 0;
                bunny.vx = -bunny.vx;
            }
            if (bunny.x > SCREEN_WIDTH - bunny_shape.width) {
                bunny.x = SCREEN_WIDTH - bunny_shape.width;
                bunny.vx = -bunny.vx;
            }
            if (bunny.y < 0) {
                bunny.y = 0;
                bunny.vy = -bunny.vy;
            }
            if (bunny.y > SCREEN_HEIGHT - bunny_shape.height) {
                bunny.y = SCREEN_HEIGHT - bunny_shape.height;
                bunny.vy = -bunny.vy;
            }
            bunnies[i] = bunny;
            const SDL_Rect bunny_rect = {
                bunny.x,
                bunny.y,
                bunny_shape.width,
                bunny_shape.height
            };
            SDL_RenderCopy(renderer, bunny_texture, NULL, &bunny_rect);

        }
        const SDL_Color color = {255, 255, 255, 255};
        // convert dt to string
        char fps_str[32];
        sprintf(fps_str, "%f", fps);
        draw_text(renderer, font, fps_str, color, 0, 0);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

