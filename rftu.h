///////////////
// Libraries //
///////////////
#ifndef __RFTU_H__
#define __RFTU_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>       // IPPROTO_RAW, IPPROTO_IP, IPPROTO_UDP, INET_ADDRSTRLEN
#include <netinet/ip.h>       // struct ip and IP_MAXPACKET (which is 65535)
#include <netinet/udp.h>      // struct udphdr
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <sys/stat.h>

////////////
// MACROS //
////////////

// RFTU COMMANDS
#define RFTU_CMD_NULL      0x00
#define RFTU_CMD_INIT      0x01
#define RFTU_CMD_DATA      0x02
#define RFTU_CMD_READY     0x03
#define RFTU_CMD_NOSPACE   0x04
#define RFTU_CMD_ACK 	   0x05
#define RFTU_CMD_ERROR 	   0x06
#define RFTU_CMD_COMPLETED 0x07

// RFTU RETURNED VALUES
#define RFTU_RET_OK 		0
#define RFTU_RET_ERROR     -1

// RFTU CONSTANTS
#define YES 1
#define NO  0

// RFTU RUNNING MODES
#define RFTU_MODE_NON	   -1
#define RFTU_MODE_SENDER 	0
#define RFTU_MODE_RECEIVER 	1

// RFTU SETTINGS
#define RFTU_SEGMENT_SIZE   1024
#define RFTU_SERVER_PORT 	69
#define RFTU_RECEIVER_PORT  96
#define RFTU_RECEIVE_FOLDER "~/Download/"
#define RFTU_TIMEOUT		3
#define RFTU_RETRY			3
#define RFTU_WINDOW_SIZE	7

// RFTU CONTROL FLAGS
extern uint8_t overwrite_flag ;
extern uint8_t verbose_mode ;
extern uint8_t no_server;
extern uint8_t file_flag;
extern uint8_t ip_flag;
extern uint8_t run_mode;

// RFTU PACKAGE
struct rftu_package_t{
	uint8_t cmd;
	uint8_t id;
	uint32_t seq;
	uint16_t size;
	uint8_t data[RFTU_SEGMENT_SIZE];
} ;

struct file_info_t{
	char filename[256];
	uint64_t filesize;
	uint8_t overwrite_flag;
} ;

// RFTU Globla Variables
// these are to keep track file and connection in a  transaction
extern char *file_name;
extern uint64_t file_size;
extern uint8_t dest_ip[];
extern uint16_t dest_port;
extern uint16_t id;
extern uint16_t datasize;

/////////////////////////
// FUNCTIONS PROTOTYPE //
/////////////////////////

// Global functions
void disp_err(uint8_t *message);
uint8_t checksum_test();
uint16_t checksum_calculate();
void send_command(uint8_t cmd, struct sockaddr *sockaddr, unsigned int slen);

// Sender's functions
uint8_t get_filename(char *path);
uint64_t get_filesize(char *path);
uint8_t check_file_exist(char *path);
uint8_t check_ip(char *ip, char *port);
void send_data(uint8_t * data, uint16_t length, struct sockaddr_in *sockaddr);

// Receiver's functions
int create_file(char *filename, uint64_t size, uint8_t overwrite, FILE *file);
int write_file(struct rftu_package_t *rftu_package, FILE *file);
int check_space(int file_size);
uint16_t gen_id();

// Signal handler
void rftu_exit(void);

#endif
