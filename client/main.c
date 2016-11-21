/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "loggers.h"
#include "game_logic/mvc.h"
#include "server_logic/socket_controller.h"

// TODO: fix send / receive - set precision
// TODO: process key -> add to queue -> send -> simulate execution -> recieve -> apply (remove from the queue if ok)
// TODO: bullet reflection
// TODO: ?threading (reciever, socket mutex). Not needed for now. Will try to avoid it
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
    logger_init();
    MVC *mvc = mvc_init();
    if (mvc == NULL) {
        log_error("Failed to create mvc", __FUNCTION__, __LINE__);
        logger_destroy();
        exit(1);
    }
    SocketController *socketController = sc_init();
    if (socketController == NULL) {
        log_error("Failed to create socket controller", __FUNCTION__, __LINE__);
        logger_destroy();
        exit(1);
    }

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
        if (mvc_process_key(mvc, keystates)) {
            quit = true;
        }
        mvc_process_moving(mvc, curr_ticks - prev_ticks);

        // Returns 0 or 1
        sc_send_current_state(socketController, mvc->this_player);

        sc_receive_current_state(socketController);

        mvc_apply_response(mvc, &socketController->requests_list, &socketController->last_response);

        mvc_render(mvc);

        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mvc_destroy(mvc);
    sc_destroy(socketController);
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
        player_keystates_process(&this_player, time_elapsed(&time));

        player_render(&this_player);
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