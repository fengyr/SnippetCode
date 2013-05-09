/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  
 *        Created:  五月 02, 2013
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zenki (Zenki.J.Zha), zenki2001cn@163.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include "connection.h"
#include "ui_interface.h"
#include "handler_process.h"
#include "command_line.h"

#define UI_PORT 11014

void command_test(int argc, const char *argv[])
{
    int returnval;

	/* initialize a struct with that to look for */
	tag t[3];

	int flag;
	t[0].name = "-m";
	t[0].type = TAGTYPE_BOOL;
	t[0].data = &flag;

	char filename[10];
	t[1].name = "-file";
	t[1].type = TAGTYPE_STRING;
	t[1].data = filename;

	float init;
	t[2].name = "-init";
	t[2].type = TAGTYPE_FLOAT;
	t[2].data = &init;

	returnval = parse_params(argc, argv, 3, t);

	printf("\nreturn value = %d\n", returnval);

	printf("\n%d\n%s\n%f\n", flag, filename, init);
}

int main(int argc, const char *argv[])
{
    command_test(argc, argv);

    Socket sock_ui; 

    EventHandler control_handler;
    control_handler.handler_type = HANDLER_TYPE_CONTROL;
    control_handler.onRecvAndReplay = ui_control_handler;

    EventHandler data_handler;
    data_handler.handler_type = HANDLER_TYPE_DATA;
    data_handler.onRecvAndReplay = ui_data_handler;

    TcpServer server = INIT_TCP_SERVER;

    server.init(&sock_ui, NULL, UI_PORT);
    server.register_handler(&sock_ui, &control_handler);
    server.register_handler(&sock_ui, &data_handler);

    server.run(&sock_ui, 1);

    while (1) {
        sleep(1);
    }

    return 0;
}
