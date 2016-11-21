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
        mvc_destroy(mvc);
        logger_destroy();
        exit(1);
    }
    if (sc_receive_start_signal(socketController) != SC_NO_ERROR) {
        log_error("sc_receive_start_signal failed", __FUNCTION__, __LINE__);
        mvc_destroy(mvc);
        sc_destroy(socketController);
        logger_destroy();
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

        sc_send_current_state(socketController, mvc->this_player);

        int res = sc_receive_current_state(socketController);
        if (res == SC_CONNECTION_CLOSED) {
            log_error("Connection closed", __FUNCTION__, __LINE__);
            break;
        }

        mvc_apply_response(mvc, &socketController->requests_list, &socketController->last_response);

        mvc_render(mvc);

        prev_ticks = curr_ticks;
        curr_ticks = SDL_GetTicks();
    }

    mvc_destroy(mvc);
    sc_destroy(socketController);
    logger_destroy();
    return 0;
}