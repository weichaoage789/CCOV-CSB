#include "Ping.h"


namespace NetWork
{
	Ping::Ping()
	{
		_MaxCnt = 4;
		_DataLen = 56;
		_SendCnt = 0;
		_RecvCnt = 0;
		_ICMP_SEQ = 0;
	}

	unsigned short Ping::getChksum(unsigned short *addr, int len)
	{
		int nleft = len;
		int sum = 0;
		unsigned short *w = addr;
		unsigned short answer = 0;

		/*把ICMP报头二进制数据以2字节为单位累加起来*/
		while (nleft > 1)
		{
			sum += *w++;
			nleft -= 2;
		}
		/*若ICMP报头为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
		if (nleft == 1)
		{
			*(unsigned char *)(&answer) = *(unsigned char *)w;
			sum += answer;
		}
		sum = (sum >> 16) + (sum & 0xffff);
		sum += (sum >> 16);
		answer = ~sum;
		return answer;
	}

	int Ping::packIcmp(int pack_no, struct icmp* icmp)
	{
		int packsize;
		struct icmp *picmp;
		struct timeval *tval;

		picmp = icmp;
		picmp->icmp_type = ICMP_ECHO;
		picmp->icmp_code = 0;
		picmp->icmp_cksum = 0;
		picmp->icmp_seq = pack_no;
		picmp->icmp_id = _Pid;
		packsize = 8 + _DataLen;
		tval = (struct timeval *)icmp->icmp_data;

		/** @记录发送时间 */
		gettimeofday(tval, NULL);

		/** @校验算法 */
		picmp->icmp_cksum = getChksum((unsigned short *)icmp, packsize);
		return packsize;
	}

	bool Ping::unpackIcmp(char *buf, int len, struct IcmpEchoReply *icmpEchoReply)
	{
		int i, iphdrlen;
		struct ip *ip;
		struct icmp *icmp;
		struct timeval *tvsend, tvrecv, tvresult;
		double rtt;

		ip = (struct ip *)buf;
		iphdrlen = ip->ip_hl << 2;    /*求ip报头长度,即ip报头的长度标志乘4*/
		icmp = (struct icmp *)(buf + iphdrlen);  /*越过ip报头,指向ICMP报头*/
		len -= iphdrlen;            /*ICMP报头及ICMP数据报的总长度*/
		if (len < 8)                /*小于ICMP报头长度则不合理*/
		{
			printf("ICMP packets\'s length is less than 8\n");
			return false;
		}

		/*确保所接收的是我所发的的ICMP的回应*/
		if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == _Pid))
		{

			tvsend = (struct timeval *)icmp->icmp_data;
			gettimeofday(&tvrecv, NULL);  /*记录接收时间*/
			tvresult = tvSub(tvrecv, *tvsend);  /*接收和发送的时间差*/
			rtt = tvresult.tv_sec * 1000 + tvresult.tv_usec / 1000;  /*以毫秒为单位计算rtt*/
			icmpEchoReply->rtt = rtt;
			icmpEchoReply->icmpSeq = icmp->icmp_seq;
			icmpEchoReply->ipTtl = ip->ip_ttl;
			icmpEchoReply->icmpLen = len;
			return true;
		}

		return false;
	}

	struct timeval Ping::tvSub(struct timeval timeval1, struct timeval timeval2)
	{
		struct timeval result;
		result = timeval1;
		if (result.tv_usec < timeval2.tv_usec < 0)
		{
			--result.tv_sec;
			result.tv_usec += 1000000;
		}
		result.tv_sec -= timeval2.tv_sec;
		return result;
	}

	bool Ping::sendPacket()
	{
		size_t packetsize;
		while (_SendCnt < _MaxCnt)
		{
			_SendCnt++;
			_ICMP_SEQ++;

			/*设置ICMP报头*/
			packetsize = packIcmp(_ICMP_SEQ, (struct icmp*)_SendBuf); 
			if (sendto(_SocketFd, _SendBuf, packetsize, 0, (struct sockaddr *) &_DestAddr, sizeof(_DestAddr)) < 0)
			{
				continue;
			}
		}
		return true;
	}

	bool Ping::recvPacket(PingResult &pingResult)
	{
		int len;
		struct IcmpEchoReply icmpEchoReply;
		int maxfds = _SocketFd + 1;
		int nfd = 0;
		fd_set rset;
		FD_ZERO(&rset);
		socklen_t fromlen = sizeof(_HostAddr);
		struct timeval timeout;
		timeout.tv_sec = 4;
		timeout.tv_usec = 0;

		for (int recvCount = 0; recvCount < _MaxCnt; recvCount++)
		{
			FD_SET(_SocketFd, &rset);
			if ((nfd = select(maxfds, &rset, NULL, NULL, &timeout)) == -1) 
			{
				perror("select error");
				continue;
			}
			if (nfd == 0)
			{
				icmpEchoReply.isReply = false;
				pingResult.icmpEchoReplys.push_back(icmpEchoReply);
				continue;
			}
			if (FD_ISSET(_SocketFd, &rset)) 
			{
				if ((len = recvfrom(_SocketFd, _RecvBuf, sizeof(_RecvBuf), 0, (struct sockaddr *)&_HostAddr, &fromlen)) < 0)
				{
					if (errno == EINTR)
						continue;
					perror("receive from error");
					continue;
				}
				icmpEchoReply.fromAddr = inet_ntoa(_HostAddr.sin_addr);
				if (icmpEchoReply.fromAddr != pingResult.ip) 
				{
					recvCount--;
					continue;
				}
			}
			if (unpackIcmp(_RecvBuf, len, &icmpEchoReply) == -1) 
			{
				recvCount--;
				continue;
			}
			icmpEchoReply.isReply = true;
			pingResult.icmpEchoReplys.push_back(icmpEchoReply);
			_RecvCnt++;
		}
		return true;
	}

	bool Ping::getsockaddr(const char * hostOrIp, struct sockaddr_in* sockaddr)
	{
		struct hostent *host;
		struct sockaddr_in dest_addr;
		unsigned long inaddr = 0l;
		bzero(&dest_addr, sizeof(dest_addr));
		dest_addr.sin_family = AF_INET;
		/*判断是主机名还是ip地址*/
		if (inaddr = inet_addr(hostOrIp) == INADDR_NONE)
		{
			if ((host = gethostbyname(hostOrIp)) == NULL) 
			{
				/*是主机名*/
				return false;
			}
			memcpy((char *)&dest_addr.sin_addr, host->h_addr, host->h_length);
		}
		else if (!inet_aton(hostOrIp, &dest_addr.sin_addr)) 
		{
			/*是ip地址*/
			return false;
		}
		*sockaddr = dest_addr;
		return true;
	}

	bool Ping::ping(std::string host, PingResult& pingResult)
	{
		return ping(host, 1, pingResult);
	}

	bool Ping::ping(std::string host, int count, PingResult& pingResult)
	{
		struct protoent *protocol;
		int size = 50 * 1024;
		IcmpEchoReply icmpEchoReply;
		bool ret;

		_SendCnt = 0;
		_RecvCnt = 0;
		pingResult.icmpEchoReplys.clear();
		_MaxCnt = count;
		_Pid = getpid();

		pingResult.dataLen = _DataLen;

		if ((protocol = getprotobyname("icmp")) == NULL)
		{
			perror("getprotobyname");
			return false;
		}
		/*生成使用ICMP的原始套接字,这种套接字只有root才能生成*/
		if ((_SocketFd = socket(AF_INET, SOCK_RAW, protocol->p_proto)) < 0)
		{
			//extern int errno;
			//pingResult.error = strerror(errno);
			return false;
		}
		/*扩大套接字接收缓冲区到50K这样做主要为了减小接收缓冲区溢出的
		  的可能性,若无意中ping一个广播地址或多播地址,将会引来大量应答*/
		setsockopt(_SocketFd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

		/*获取main的进程id,用于设置ICMP的标志符*/
		if (!getsockaddr(host.c_str(), &_DestAddr)) 
		{
			pingResult.error = "unknow host " + host;
			return false;
		}
		pingResult.ip = inet_ntoa(_DestAddr.sin_addr);
		sendPacket();  /*发送所有ICMP报文*/
		recvPacket(pingResult);  /*接收所有ICMP报文*/
		pingResult.nsend = _SendCnt;
		pingResult.nreceived = _RecvCnt;
		close(_SocketFd);

		return true;
	}
}
