#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int
set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // ignore break signal
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void
set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

char *portname = "/dev/ttymxc3";
int fd = 0;

void sendcmd(int r, int g, int b){
	char data[] = {0xff,0,0,0};
	
	if(r > 254)
		r = 254;
	if(g > 254)
		g = 254;
	if(b > 254)
		b = 254;
	
	data[1] = (char) r;
	data[2] = (char) g;
	data[3] = (char) b;
	
	write(fd, data, 4);           		// send 1 character greeting
}



int main(int argc, char **argv) {
	fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0)
	{
			printf("error %d opening %s: %s", errno, portname, strerror (errno));
			return;
	}

	set_interface_attribs (fd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);                // set no blocking

	if(argc == 2){
		if(strcmp(argv[1], "red") == 0)
			sendcmd(254, 0, 0);
		else if(strcmp(argv[1], "green")== 0)
			sendcmd(0, 254, 0);
		else if(strcmp(argv[1], "blue")== 0)
			sendcmd(0, 0, 254);
		else if(strcmp(argv[1], "yellow")== 0)
			sendcmd(254, 254, 0);
		else if(strcmp(argv[1], "pink")== 0)
			sendcmd(254, 0, 254);
		else if(strcmp(argv[1], "cyan")== 0)
			sendcmd(0, 254, 254);
		else if(strcmp(argv[1], "black")== 0)
			sendcmd(0, 0, 0);
		else if(strcmp(argv[1], "white")== 0)
			sendcmd(254, 254, 254);
		else
			printf("\nUnknown argument\n");
	}
	else if(argc == 4){
		sendcmd(atoi(argv[0]), atoi(argv[1]),atoi(argv[2]));
	}
	else{	
		printf("GionjiLed\n");
		printf("Usage:\n");
		printf("gionjiled [red | green | blue | yellow | pink | cyan | white | black]\n");
		printf("gionjiled R G B [from 0 to 254]\n");
		printf("Enjoy");
	}

	
	
}


