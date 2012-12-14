/**
 *       File       main.c
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *       Version    1.0
 *       Author:    Zenki (zhajun), zenki2001cn@163.com
 *       Date:      2010年06月02日 12时49分04秒
 *
 * =====================================================================================
 */
#include "main.h"
#include "class.h"

static void initialize(MAIN *this)
{
    demo_init(&this->Attr.demo1, 7, 28, "zenki");

    demo_init(&this->Attr.demo2, 8, 22, "jacky");
}

static void destroy(MAIN *this)
{
    demo_free(&this->Attr.demo1);

    demo_free(&this->Attr.demo2);
}

static int Main(MAIN *this, int argc, const char *argv[])
{
	this->Attr.demo1.Iface.display(&this->Attr.demo1);
    this->Attr.demo1.number += 100;
    PDEBUG("number: %d\n", this->Attr.demo1.number);

	this->Attr.demo2.Iface.display(&this->Attr.demo2);
    this->Attr.demo2.number += 101;
    PDEBUG("number: %d\n", this->Attr.demo2.number);

    return 0;
}

void main_init(MAIN *this)
{
    this->initialize = initialize;
    this->destroy = destroy;

    this->Iface.main = Main;

    this->initialize(this);
}

void main_free(MAIN *this)
{
    this->destroy(this);
    PDEBUG("main free\n");    
}

int main(int argc, const char *argv[])
{
    MAIN Main;

    main_init(&Main);

    Main.Iface.main(&Main, argc, argv);

    main_free(&Main);

	return 0;
}
