#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/file.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>

static volatile sig_atomic_t doneflag = 0;

static void setdoneflag(int signo)
{
	doneflag = 1;
}

static int initSig(void)
{
	struct sigaction act;
	act.sa_handler = setdoneflag;
	act.sa_flags = 0;
	if((sigemptyset(&act.sa_mask) == -1)||
		(sigaction(SIGINT,&act,NULL) == -1))
	{
		perror("Failed to set SIGINT handler");
		return 1;
	}

	return 0;
}

int setSerial(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
	struct termios newtio,oldtio;

	if(tcgetattr(fd,&oldtio) != 0)
	{
		perror("SetupSerial 1");
		return 1;
	}

	bzero(&newtio,sizeof(newtio));
	
	//1:Setting char size
	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;

	//2:Setting bits
	switch(nBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;

	case 8:
		newtio.c_cflag |= CS8;
		break;
	}
	
	//3:Setting parity
	switch(nEvent)
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_cflag |= (INPCK | ISTRIP);
		break;
	
	case 'E':
		newtio.c_cflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;

	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	}

	//4:Setting speed
	switch(nSpeed)
	{
	case 2400:
		cfsetispeed(&newtio,B2400);
		cfsetospeed(&newtio,B2400);
		break;
	
	case 4800:
		cfsetispeed(&newtio,B4800);
		cfsetospeed(&newtio,B4800);
		break;

	case 9600:
		cfsetispeed(&newtio,B9600);
		cfsetospeed(&newtio,B9600);
		break;

	case 115200:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;

	case 460800:
		cfsetispeed(&newtio,B460800);
		cfsetospeed(&newtio,B460800);
		break;

	default:
		cfsetispeed(&newtio,B115200);
		cfsetospeed(&newtio,B115200);
		break;
	}

	//5:Setting stop bits
	switch(nStop)
	{
	case 1:
		newtio.c_cflag &= ~CSTOPB;
		break;

	case 2:
		newtio.c_cflag |= CSTOPB;	
		break;
	}

	//6:Setting recvice timeout and mini bytes  size of recvice
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	//7:flush input and output buffer
	tcflush(fd,TCIOFLUSH);
	
	if(tcsetattr(fd,TCSANOW,&newtio) != 0)
	{
		perror("serial set error");
		return 2;
	}
	
	printf("serial setting done\n");
	
	return 0;
}

int openSerial(int fd,unsigned char *SerialName)
{
	fd = open(SerialName,O_RDWR|O_NOCTTY|O_NDELAY);
	if(fd <0)
	{
		perror("open");
		return fd;
	}
#if 1
	if(fcntl(fd,F_SETFL,0) < 0)
		printf("fcntl failed!\n");
	else
		printf("fcntl=%d\n",fcntl(fd,F_SETFL,0));	
#endif
	return fd;
}

int rwTest(int fd)
{
	int rLen=0,wLen=0,ret=0;
	unsigned char buf[3000];
    unsigned int i=0,j=0;
	unsigned int r_count=0;
#if 1	
	fd_set rd;	

	FD_ZERO(&rd);
	FD_SET(fd,&rd);
#endif
	while(doneflag ==0)
    {
#if 1
		while(!FD_ISSET(fd,&rd));
		if(select(fd+1,&rd,NULL,NULL,NULL) <0)
		continue;
#endif
        rLen= read(fd,buf,sizeof(buf));
        if(rLen < 0)
        {
            perror("read");
            return 1;
        }

        if(rLen == 0)
        {
            //sleep(1);
//            printf("sleep\n");
            continue;
        }

		//r_count++;
		//if(r_count%100 == 0)
       	//	 printf("rLen:%d\n",rLen);
        
        wLen = 0;
        for(j=0;j<5;j++)
        {
            ret = write(fd, buf + wLen, rLen - wLen);
            if(ret < 0)
            {
                perror("write");
                return 2;
            }
            wLen += ret;

            if(wLen == rLen)
                break;
        }

        if(wLen == rLen)
		{ 
			//if(r_count%100 ==0)
	   		//	printf("wLen:%d\n",wLen);
		}
		else
        {
            printf("write err 1\n");
            return 3;
        }
    }
	
	return 0;
}

int main(int argc, char *argv[])
{
	int fd;
    int flags;
	int ret;
    unsigned char SerialName[30]={0};

	printf("Version:%s\n","2010.12.30_00");

	if(argc == 1)
	{
		strcpy(SerialName,"/dev/ttyPos0");
	}
	else if(argc == 2)
	{
		strcpy(SerialName,argv[1]);
	}
	else
	{
		printf("ARG ERROR\n");
		return 1;
	}

	printf("SerialName:%s\n",SerialName);

	ret = initSig();
	if(ret)
		return ret;	        

    
	fd = openSerial(fd,SerialName);//open(SerialName,O_RDWR);

	if(fd <0)
	{
		exit(1);
	}
	
	ret = setSerial(fd,115200,8,'N',1);
	if(ret != 0)
	{
		printf("setSerial:%d\n");
		goto byebye;
	}

    if(ioctl(fd,TIOCMGET,&flags)<0)
    {
        perror("ioctl failed");
        goto byebye;
    }

    printf("flags:%X\n",flags);

	ret = rwTest(fd);
	printf("rwTest:%d\n",ret);

byebye:
	printf("doneflag:%d\n",doneflag);
	ret = close(fd);
	if(ret <0)
	{
		perror("close");
	}
	else
	{
		printf("close success!\n");
	}
	exit(0);
}
