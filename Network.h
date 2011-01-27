/**
 * \file Network.h
 *  Network is a kind of network listener - it handles incoming data
 */
#ifndef NETWORK_H
#define NETWORK_H

#include <sys/thread.h>
#include <cfg/crt.h>    /* Floating point configuration. */

#include <string.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdio.h>
#include <io.h>

#include "NetHandler.h"
#include "snmp.h"

bool Network_init(NH * nh);


THREAD(udpTrapReceiveThread, arg);

THREAD(udpReceiveThread, arg);

#endif
