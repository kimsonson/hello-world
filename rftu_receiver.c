/**
 * RFTU RECEIVER
 */
#define PORT 8888

#include "rftu.h"

void die(char *s)
{
	perror(s);
	exit(1);
}

int receiver()
{
	/**
	 * Receiver's variables
	 */
	uint8_t receiving_flag = NO;
	uint8_t counter = 0;
	uint32_t seq = 0;
	uint32_t Rn = 0;
	uint64_t received_size = 0;
	uint8_t data[1024];
	uint8_t *pdata;
	FILE *file;

	struct rftu_package_t  receiver_rftu_pkg;

	//variable for socket
	struct sockaddr_in si_me, si_other;  //address of receiver and sender
	unsigned int slen = sizeof(si_other);
	int recv_len;  
	int socket_d;  //socket file descriptor

	//variable for select function
	int result;		//return value from select function
	fd_set inputs, testfds;
	struct timeval timeout;
	FD_ZERO(&inputs);
	FD_SET(0,&inputs);

	/**
	 * Start here
	 */
	//create a UDP socket
	if ((socket_d=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	//addressing for socket
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind socket to port
	if (bind(socket_d, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
	{
		die("bind");
	}

	while(1)
	{
		testfds = inputs;
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		FD_ZERO(&testfds);
		FD_SET(socket_d, &testfds);

		result = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);
		switch(result) {
			case 0:
				printf("timeout\n");
				if (receiving_flag==YES)
				{
					counter++;
					if (counter==3)
					{
						send_command(RFTU_CMD_ERROR, (struct sockaddr *)&si_other, slen);
						receiving_flag = NO;
					}
				}
				break;
			case -1:
				perror("sellect");
				exit(1);
			default:
				if (FD_ISSET(socket_d, &testfds))
				{
					recvfrom(socket_d, (struct RFTU_packet *)&receiver_rftu_pkg, 1034, 0, (struct sockaddr *) &si_other, &slen);
              		switch(receiver_rftu_pkg.cmd) {
              			case RFTU_CMD_INIT:
              				if (receiving_flag == NO)
              				{
              					pdata = receiver_rftu_pkg.data;
              					file_name = receiver_rftu_pkg.data;
              					pdata +=256;
              					file_size = *pdata;
              					pdata +=64;
              					overwrite_flag = *pdata;

              					if (check_space(file_size))
              					{
              						create_file(file_name, file_size, overwrite_flag, file);
              						id = gen_id();
              						send_command(RFTU_CMD_READY, (struct sockaddr *)&si_other, slen);
              						receiving_flag = YES;
              						Rn = 0;
              					}
              					else
              					{
              						send_command(RFTU_CMD_NOSPACE, (struct sockaddr *)&si_other, slen);
              					}
              				}
              				else
              				{

              				}
              				break;
              			case RFTU_CMD_DATA:
              				//id = receiver_rftu_pkg.id;
              				if (id==receiver_rftu_pkg.id)
              				{
              					seq = receiver_rftu_pkg.seq;
              					datasize = receiver_rftu_pkg.size;
              					memcpy(data, receiver_rftu_pkg.data, datasize);
              					if (seq==Rn)
              					{
              						counter = 0;
              						write_file(&receiver_rftu_pkg, file);
              						received_size += datasize;
              						send_command(RFTU_CMD_ACK, (struct sockaddr *)&si_other, slen);
              						Rn++;
              					}
              				}
              				break;
              			default:
              				break;
              		}
				}
		}
	}

	close(socket_d);
	return RFTU_RET_OK;
}
