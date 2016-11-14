/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdbool.h>
#include "server_logic.h"
#include <sys/timeb.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.h"
#include "default_values.h"
#include "cleaner.h"

// TODO: threading (reciever, socket mutex)

// pthread_mutex_t socket_lock;

int time_elapsed(struct timeb *time)
{
    struct timeb prev_time = *time;
    ftime(time);
    return (int) (1000.0 * (time->time    - prev_time.time) +
                           (time->millitm - prev_time.millitm));
}

void reciever_thread(void *sock)
{
    // pthread_mutex_lock(&socket_lock);
    char server_reply[2000];
    if (recieve_reply_from_the_server(sock, server_reply, 2000))
    {
        puts("No message recieved");
    }
    // pthread_mutex_unlock(&socket_lock);
}

int initialize(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init error: %s", SDL_GetError());
        exit(1);
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        fprintf(stderr, "IMG_Init error: %s", SDL_GetError());
        cleanup(NULL, NULL, true, false);
        exit(1);
    }
    *window = SDL_CreateWindow("The Final Project", 100, 100,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        fprintf(stderr, "SDL_CreateWindow error: %s", SDL_GetError());
        cleanup(NULL, NULL, true, true);
        exit(1);
    }

    *renderer = SDL_CreateRenderer(*window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s", SDL_GetError());
        cleanup(*window, NULL, true, true);
        exit(1);
    }
}

int process_key(Player *player, unsigned delta_ticks, SDL_Keycode key)
{
    switch(key)
    {
        case SDLK_ESCAPE:
            return 1;
        case SDLK_UP:
        case SDLK_DOWN:
        case SDLK_LEFT:
        case SDLK_RIGHT:
            player_key_process(player, delta_ticks, key);
        default:
            return 0;
    }
}

int main(int argc , char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    initialize(&window, &renderer);

    Player player;
    if (player_create(&player, renderer) != 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    bool quit = false;
    unsigned curr_ticks, prev_ticks = 0;
    while (!quit)
    {
        SDL_Event event;
        curr_ticks = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (process_key(&player, curr_ticks - prev_ticks, event.key.keysym.sym)) {
                        quit = true;
                    }
                    break;
                default:;
            }
        }
        SDL_RenderClear(renderer);
        player_render(&player, renderer);
        SDL_RenderPresent(renderer);
        printf("%g\n", 1000.0f / (SDL_GetTicks() - curr_ticks));
        prev_ticks = curr_ticks;
    }

    player_destroy(&player);

    cleanup(window, renderer, true, true);
    return 0;

    /*
    if (pthread_mutex_init(&socket_lock, NULL) != 0) {
        puts("Mutex init failed");
        return 2;
    }
    */

    //---------------
    /*
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    if (connect_to_server(&sock, &server)) {
        return 1;
    }

    struct timeb time;
    ftime(&time);
    while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE)) {
        player_key_process(&player, time_elapsed(&time));

        player_render(&player);
        slRender();

        message[0] = '1';
        message[1] = '\0';
        if (send_data_to_the_server(&sock, message)) {
            break;
        }

        reciever_thread(&sock);

        puts("Server reply :");
        puts(server_reply);
    }

    // pthread_cancel(...);
    if (shutdown(sock, 2) == -1) {
        perror("Failed to close the socket");
    };
    slClose();
     */
    return 0;
}