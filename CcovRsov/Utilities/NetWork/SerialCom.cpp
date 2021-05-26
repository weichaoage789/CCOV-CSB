#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>

#include "SerialCom.h"


namespace NetWork
{
	int CCOMCtrl::Open(int id)
	{
		memset(_Name, 0, sizeof(_Name));
		sprintf(_Name, "/dev/ttyS%d", id);

		_Fd = open(_Name, O_RDWR | O_NOCTTY | O_NONBLOCK);
		return _Fd;
	}

	void CCOMCtrl::Close()
	{
		if (_Fd > 0)
		{
			close(_Fd);
			_Fd = -1;
		}
	}

	int CCOMCtrl::SetOptions(int baudrate, int databits, char parity, int stopbits)
	{
		struct termios newtio, oldtio;
		if (tcgetattr(_Fd, &oldtio) != 0) { return -1; }

		bzero(&newtio, sizeof(newtio));
		newtio.c_cflag |= CLOCAL | CREAD;
		newtio.c_cflag &= ~CSIZE;

		switch (databits)
		{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
		default:
			newtio.c_cflag |= CS8;
			break;
		}

		switch (parity)
		{
		case 'O':
			newtio.c_cflag |= PARENB;
			newtio.c_cflag |= PARODD;
			newtio.c_iflag |= (INPCK | ISTRIP);
			break;
		case 'E':
			newtio.c_iflag |= (INPCK | ISTRIP);
			newtio.c_cflag |= PARENB;
			newtio.c_cflag &= ~PARODD;
			break;
		case 'N':
			newtio.c_cflag &= ~PARENB;
			break;
		default:
			newtio.c_cflag &= ~PARENB;
			break;
		}

		switch (baudrate)
		{
		case 2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case 4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case 9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case 115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		case 230400:
			cfsetispeed(&newtio, B230400);
			cfsetospeed(&newtio, B230400);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		}

		if (stopbits == 1)
			newtio.c_cflag &= ~CSTOPB;
		else if (stopbits == 2)
			newtio.c_cflag |= CSTOPB;

		newtio.c_cc[VTIME] = 0;
		newtio.c_cc[VMIN] = 0;
		tcflush(_Fd, TCIFLUSH);
		return tcsetattr(_Fd, TCSANOW, &newtio);
	}

	int CCOMCtrl::Recv(char *buf, int len)
	{
		int ret, left, bytes;
		left = len;

		while (left > 0)
		{
			ret = 0;
			bytes = 0;

			ioctl(_Fd, FIONREAD, &bytes);
			if (bytes > 0)
			{
				ret = read(_Fd, buf, left);
			}

			if (ret > 0)
			{
				left -= ret;
				buf += ret;
			}
			usleep(100);
		}

		return len - left;
	}

	int CCOMCtrl::Send(char *data, int len)
	{
		int ret = 0;
		int nleft = len;
		char *ptmp = data;

		while (nleft > 0)
		{
			ret = write(_Fd, ptmp, nleft);
			if (ret > 0)
			{
				nleft -= ret;
				ptmp += ret;
			}
		}

		return len - nleft;
	}
}