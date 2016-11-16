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
#include "processors.h"
#include "initialization.h"
#include "loggers.h"
#include "bullet.h"

// TODO: put bullets in a circle queue !!!
// TODO: shooting (not high fire rate; put bullets in a /-queue-/ array, process them for O(n2))
// TODO: bullet reflection
// TODO: threading? (reciever, socket mutex). Not needed for now. Will try to avoid it
// TODO: process key -> add to queue -> send -> simulate execution -> recieve -> apply (remove from the queue if ok)
// pthread_mutex_t socket_lock;

//void reciever_thread(void *sock)
//{
//    // pthread_mutex_lock(&socket_lock);
//    char server_reply[2000];
//    if (recieve_reply_from_the_server(sock, server_reply, 2000))
//    {
//        puts("No message recieved");
//    }
//    // pthread_mutex_unlock(&socket_lock);
//}

int main(int argc , char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Bullets *bullets;
    Player *player;
    initialize(&window, &renderer, &player, &bullets);

    bool quit = false;
    unsigned curr_ticks = 0, prev_ticks = 0;
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                default:;
            }
        }
        if (process_key(player, bullets, keystates)) {
            quit = true;
        }
        process_moving(player, bullets, curr_ticks - prev_ticks);

        SDL_RenderClear(renderer);
        player_render(player, renderer);
        bullet_render_all(bullets, renderer);
        SDL_RenderPresent(renderer);
        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    player_destroy(player);
    bullet_destroy(bullets);
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
        player_keystates_process(&player, time_elapsed(&time));

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