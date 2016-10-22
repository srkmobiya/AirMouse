#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#if defined(OS_LINUX) || defined(OS_MACOSX)
#include <sys/ioctl.h>
#include <termios.h>
#elif defined(OS_WINDOWS)
#include <windows.h>
#include <conio.h>
#endif

#include<time.h>
#include "hid.h"

char cmd[64];

void mousemove(int x, int y)
{
	INPUT ip;
	ip.type=INPUT_MOUSE;
	ip.mi.dx = x;
	ip.mi.dy = y;
	ip.mi.dwFlags = MOUSEEVENTF_MOVE;
	ip.mi.time = 0;
	ip.mi.dwExtraInfo= 0;
	SendInput(1, &ip, sizeof(INPUT));

}

int main()
{
	int i, r, num;
	char c, buf[64];

	// C-based example is 16C0:0480:FFAB:0200
	r = rawhid_open(1, 0x16C0, 0x0480, 0xFFAB, 0x0200);
	if (r <= 0) {
		// Arduino-based example is 16C0:0486:FFAB:0200
		r = rawhid_open(1, 0x16C0, 0x0486, 0xFFAB, 0x0200);
		if (r <= 0) {
			printf("no rawhid device found\n");
			return -1;
		}
	}
	printf("found rawhid device\n");

	float pre = 0;
	num =0;

	
	char buf1[64];
	char *a;
	float curr[3];
	while (1) {
		if (kbhit() && _getch() == 'q')
		{
			break;
		}
		/*buf[0] = 'r';
                buf[1] = 'a';
                for (i=2; i<64; i++) {
                                buf[i] = 0;
                }
                rawhid_send(0, buf, 64, 100);*/
		num = rawhid_recv(0, buf, 64, 30);
		//printf("Getting %d\n", num);
                if (num < 0) {
                        printf("\nerror reading, device went offline\n");
                        rawhid_close(0);
                        return 0;
                }
                if (num > 0) {
                        //printf("\nRecv %d bytes:\n", num);
			//printf("%s\n", buf);
			float x,y,z;
			i=0;
                        a = strtok(buf, ",");
			while(a!=NULL)
			{
				curr[i++] = atoi(a)/100.0;
				a= strtok(NULL, ",");
			}
			x=curr[0];
			y=curr[1];
			z=curr[2];
			if (abs(pre - z) > 0)
			{
			        if (z > 0.03 && z < 1.0)
					z = 1;
				if (z < -0.03 && z > -1.0)
	 				z = -1;	

				if (y > 0.03 && y < 1.0)
					y = 1;
				if (y < -0.03 && y > -1.0)
	 				y = -1;
				printf("%f \n", ( z));
				mousemove(-(z), y);
				pre = z;
			}
                        
                }
		//Sleep(30);
		//printf("Enter Commands :\r\n");
		//scanf("%s",cmd);
		//strcpy(cmd, "gcc");
                /*if(strstr(cmd,"acc"))
		{
			buf[0] = 'r';
			buf[1] = 'a';
                        for (i=2; i<64; i++) {
                                buf[i] = 0;
                        }
                        rawhid_send(0, buf, 64, 100);
		        printf("Getting ACC");
		}     
		else if(strstr(cmd,"gcc"))*/
                {
                        /*buf[0] = 'r';
                        buf[1] = 'a';
                        for (i=2; i<64; i++) {
                                buf[i] = 0;
                        }
                        rawhid_send(0, buf, 64, 100);*/
                        
			//printf("Getting GCC");
                }
           	/*else if(strstr(cmd,"pinmode"))
                {
                        buf[0] = 'w';
                        buf[1] = 'm';
                        buf[2] = 8  ;
                        buf[3] = 1  ;
                        for (i=4; i<64; i++) {
                                buf[i] = 0;
                        }
                        rawhid_send(0, buf, 64, 100);
                }
		else if(strstr(cmd,"pinlow"))  
                {
                        buf[0] = 'w';
                        buf[1] = 'w';
			buf[2] = 8  ;
			buf[3] = 0  ;
                        for (i=4; i<64; i++) {
                                buf[i] = 0;
                        }
                        rawhid_send(0, buf, 64, 100);
                }
		else if(strstr(cmd,"pinhigh"))
                {
                        buf[0] = 'w';
                        buf[1] = 'w';
                        buf[2] = 8  ;
                        buf[3] = 1  ;
                        for (i=4; i<64; i++) {
                                buf[i] = 0;
                        }
                        rawhid_send(0, buf, 64, 100);
                }*/

	}
}



