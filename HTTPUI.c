#include <string.h>
#include <stdio.h>
#include <io.h>

#include <dev/board.h>

#include <sys/socket.h>
#include <sys/confnet.h>

#include <net/route.h>
#include <netdb.h>

#include <pro/dhcp.h>

#define MY_MAC          {0x00,0x06,0x98,0x20,0x00,0x00}
#define MY_IP           "10.10.7.25"
#define MY_MASK         "255.255.255.0"

static int HTTPUI_ShowMenu(FILE * stream, REQUEST * req)
{
    NUTTHREADINFO *tdp = nutThreadList;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Test</TITLE>" "</HEAD><BODY><h1>Nut/OS Test</h1>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Name</TH><TH>Priority</TH>"
        "<TH>Status</TH><TH>Event<BR>Queue</TH>" "<TH>Timer</TH><TH>Stack-<BR>pointer</TH>" "<TH>Free<BR>Stack</TH></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD>"
        "<TD>%s</TD><TD>%04X</TD><TD>%04X</TD>" "<TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    fputs_P(ttop_P, stream);
    while (tdp) {
        fprintf_P(stream, tfmt_P, (u_int)tdp, tdp->td_name, tdp->td_priority,
                  states[tdp->td_state], (u_int)tdp->td_queue, (u_int)tdp->td_timer,
                  (u_int)tdp->td_sp,
                  (u_int) tdp->td_sp - (u_int) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
        tdp = tdp->td_next;
    }
    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

static int HTTPUI_setShips(FILE * stream, REQUEST * req)
{
    NUTTHREADINFO *tdp = nutThreadList;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Test</TITLE>" "</HEAD><BODY><h1>Nut/OS Test</h1>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Name</TH><TH>Priority</TH>"
        "<TH>Status</TH><TH>Event<BR>Queue</TH>" "<TH>Timer</TH><TH>Stack-<BR>pointer</TH>" "<TH>Free<BR>Stack</TH></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD>"
        "<TD>%s</TD><TD>%04X</TD><TD>%04X</TD>" "<TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    fputs_P(ttop_P, stream);
    while (tdp) {
        fprintf_P(stream, tfmt_P, (u_int)tdp, tdp->td_name, tdp->td_priority,
                  states[tdp->td_state], (u_int)tdp->td_queue, (u_int)tdp->td_timer,
                  (u_int)tdp->td_sp,
                  (u_int) tdp->td_sp - (u_int) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
        tdp = tdp->td_next;
    }
    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

static int HTTPUI_ownField(FILE * stream, REQUEST * req)
{
    NUTTHREADINFO *tdp = nutThreadList;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Test</TITLE>" "</HEAD><BODY><h1>Nut/OS Test</h1>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Name</TH><TH>Priority</TH>"
        "<TH>Status</TH><TH>Event<BR>Queue</TH>" "<TH>Timer</TH><TH>Stack-<BR>pointer</TH>" "<TH>Free<BR>Stack</TH></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD>"
        "<TD>%s</TD><TD>%04X</TD><TD>%04X</TD>" "<TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    fputs_P(ttop_P, stream);
    while (tdp) {
        fprintf_P(stream, tfmt_P, (u_int)tdp, tdp->td_name, tdp->td_priority,
                  states[tdp->td_state], (u_int)tdp->td_queue, (u_int)tdp->td_timer,
                  (u_int)tdp->td_sp,
                  (u_int) tdp->td_sp - (u_int) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
        tdp = tdp->td_next;
    }
    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}

static int HTTPUI_enemyField(FILE * stream, REQUEST * req)
{
    NUTTHREADINFO *tdp = nutThreadList;
    static prog_char head_P[] = "<HTML><HEAD><TITLE>Nut/OS Test</TITLE>" "</HEAD><BODY><h1>Nut/OS Test</h1>\r\n";
    static prog_char ttop_P[] = "<TABLE BORDER><TR><TH>Handle</TH>"
        "<TH>Name</TH><TH>Priority</TH>"
        "<TH>Status</TH><TH>Event<BR>Queue</TH>" "<TH>Timer</TH><TH>Stack-<BR>pointer</TH>" "<TH>Free<BR>Stack</TH></TR>\r\n";
    static prog_char tbot_P[] = "</TABLE></BODY></HTML>";
    static prog_char tfmt_P[] = "<TR><TD>%04X</TD><TD>%s</TD><TD>%u</TD>"
        "<TD>%s</TD><TD>%04X</TD><TD>%04X</TD>" "<TD>%04X</TD><TD>%u</TD><TD>%s</TD></TR>\r\n";

    NutHttpSendHeaderTop(stream, req, 200, "Ok");
    NutHttpSendHeaderBot(stream, "text/html", -1);

    fputs_P(head_P, stream);

    fputs_P(ttop_P, stream);
    while (tdp) {
        fprintf_P(stream, tfmt_P, (u_int)tdp, tdp->td_name, tdp->td_priority,
                  states[tdp->td_state], (u_int)tdp->td_queue, (u_int)tdp->td_timer,
                  (u_int)tdp->td_sp,
                  (u_int) tdp->td_sp - (u_int) tdp->td_memory, *((u_long *) tdp->td_memory) != DEADBEEF ? "Corr" : "OK");
        tdp = tdp->td_next;
    }
    fputs_P(tbot_P, stream);
    fflush(stream);

    return 0;
}


THREAD(httpui, arg) 
{
	TCPSCOKET *sock;
	FILE* stream;
	u_long ip_addr;
	
	ip_addr = inet_addr(MY_IP);
	
	NutNetIfConfig("eth0", my_mac, ip_addr, inet_addr(MY_MASK));
    // NutIpRouteAdd(0, 0, inet_addr(MY_GATE), &DEV_ETHER);
	//
	// 1. menut
	// 2. laivojen asetus
	// 3. ownField
	// 4. oppField
	//
	NutRegisterCgi("menu.cgi", 		HTTPUI_ShowMenu);
	NutRegisterCgi("setShips.cgi", 	HTTPUI_setShips);
	NutRegisterCgi("ownField.cgi", 	HTTPUI_ownField);
	NutRegisterCgi("enemyField.cgi", 	HTTPUI_enemyField);
	
	NutRegisterDevice(&DEV_ETHER, 0, 0);
	
	for (;;) {
		sock = NutTcpCreateSocket();
		NutTcpAccept(sock, 80);
		stream = _fdopen((int) sock, "r+b");
		NutHttpProcessRequest(stream);
		fclose(stream);
		NutTcpCloseSocket(sock);
	}
}