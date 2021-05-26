#include "Utilities/FileWork/LogFileWork.h"
using namespace FileWork;

#include "Led.h"


CLEDCtrl::CLEDCtrl()
{
	LED_Init();
}

int CLEDCtrl::LED_Init()
{
	int fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "LED_ERR: Radio hard reset pin open error";
		return EXIT_FAILURE;
	}

	write(fd, "99", sizeof("99"));
	write(fd, "100", sizeof("100"));
	write(fd, "101", sizeof("101"));
	write(fd, "115", sizeof("115"));
	write(fd, "116", sizeof("116"));
	close(fd);


	fd = open(LED1_DIR, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "LED1_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "out", sizeof("out"));
	close(fd);

	fd = open(LED2_DIR, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "LED2_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "out", sizeof("out"));
	close(fd);

	fd = open(LED3_DIR, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "LED3_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "out", sizeof("out"));
	close(fd);

	fd = open(OE_DIR, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "LED4_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "out", sizeof("out"));
	close(fd);

	fd = open(LED4_DIR, O_WRONLY);
	if (fd == -1)
	{
		LOG(ERROR) << "OE_DIR_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "out", sizeof("out"));
	close(fd);

	fd = open(LED4_VAL, O_RDWR);
	if (fd == -1)
	{
		LOG(ERROR) << "OE_VAL_ERR: Radio hard reset pin direction open error";
		return EXIT_FAILURE;
	}
	write(fd, "0", sizeof("0"));
	close(fd);

	return EXIT_SUCCESS;
}

int CLEDCtrl::LED_A2(int val)
{
	int fd = open(LED1_VAL, O_RDWR);
	if (fd == -1)
	{
		LOG(ERROR) << "LED_A2 open error";
		return EXIT_FAILURE;
	}

	if (1 == val)
		write(fd, "1", sizeof("1"));
	else if (0 == val)
		write(fd, "0", sizeof("0"));
	else 
	{
		LOG(ERROR) << "LED_A2 : wrong value";
		close(fd);
		return EXIT_FAILURE;
	}
	close(fd);

	return EXIT_SUCCESS;
}

int CLEDCtrl::LED_A3(int val)
{
	int fd = open(LED2_VAL, O_RDWR);
	if (fd == -1)
	{
		LOG(ERROR) << "LED_A3 open error";
		return EXIT_FAILURE;
	}

	if (1 == val)
		write(fd, "1", sizeof("1"));
	else if (0 == val)
		write(fd, "0", sizeof("0"));
	else 
	{
		LOG(ERROR) << "LED_A3 : wrong value";
		close(fd);
		return EXIT_FAILURE;
	}
	close(fd);

	return EXIT_SUCCESS;
}

int CLEDCtrl::LED_B2(int val)
{
	int fd = open(LED3_VAL, O_RDWR);
	if (fd == -1)
	{
		LOG(ERROR) << "LED_B2 open error";
		return EXIT_FAILURE;
	}

	if (1 == val)
		write(fd, "1", sizeof("1"));
	else if (0 == val)
		write(fd, "0", sizeof("0"));
	else 
	{
		LOG(ERROR) << "LED_B2 : wrong value";
		close(fd);
		return EXIT_FAILURE;
	}
	close(fd);

	return EXIT_SUCCESS;
}

int CLEDCtrl::LED_B3(int val)
{
	int fd = open(OE_VAL, O_RDWR);
	if (fd == -1)
	{
		LOG(ERROR) << "LED_B3 open error";
		return EXIT_FAILURE;
	}

	if (1 == val)
		write(fd, "1", sizeof("1"));
	else if (0 == val)
		write(fd, "0", sizeof("0"));
	else 
	{
		LOG(ERROR) << "LED_B3 : wrong value";
		close(fd);
		return EXIT_FAILURE;
	}
	close(fd);

	return EXIT_SUCCESS;
}
