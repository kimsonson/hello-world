/**
 * MAIN of RFTU
 */

#include "rftu.h"

// RFTU CONTROL FLAGS
uint8_t overwrite_flag = NO;
uint8_t verbose_mode   = NO;
uint8_t no_server      = NO;
uint8_t file_flag      = NO;
uint8_t ip_flag        = NO;
uint8_t run_mode       = RFTU_MODE_NON;


// RFTU Globla Variables
// these are to keep track file and connection in a  transaction
char *file_name    = "";
uint64_t file_size = 0;
uint8_t dest_ip[]  = "255.255.255.255";
uint16_t dest_port = 0;
uint16_t id;
uint16_t datasize;


int main()
{
	return 1;	
}

/**
 * Global functions
 */
void send_command(uint8_t cmd, struct sockaddr *sockaddr, unsigned int slen)
{
    struct rftu_package_t sender_rftu_pkg;
    switch(cmd) {
      case RFTU_CMD_INIT:
        break;
      case RFTU_CMD_READY:
        sender_rftu_pkg.cmd = cmd;
        sender_rftu_pkg.id = (id = gen_id());
        sender_rftu_pkg.seq = 0;
        sender_rftu_pkg.size = 0;
        memset(sender_rftu_pkg.data, 0, 1024);
        break;
      case RFTU_CMD_NOSPACE:
        sender_rftu_pkg.cmd = cmd;
        sender_rftu_pkg.id = 0;
        sender_rftu_pkg.seq = 0;
        memset(sender_rftu_pkg.data, 0, 1024);
        break;
      case RFTU_CMD_ACK:
        sender_rftu_pkg.cmd = cmd;
        sender_rftu_pkg.id = id;
        sender_rftu_pkg.seq = 
        break;
      case RFTU_CMD_ERROR:
        sender_rftu_pkg.cmd = cmd;
        break;
      case RFTU_CMD_COMPLETED:
        break;
      default:
        break;
    }
}

int check_space(int file_size)
{
    if (file_size>0)
    	return 1;
    else
	return 0;
}

uint16_t gen_id()
{
    return rand();
}

int write_file(struct rftu_package_t *rftu_package, FILE *file)
{
  return 1;
}

int create_file(char *filename, uint64_t size, uint8_t overwrite, FILE *file)
{
    if (check_file_exist(filename))
    {
        if(overwrite == YES)
          file = fopen(filename, "w+");
        else
          file = fopen(strcat(filename, "(1)"), "w");
    }
    else
    {
        file = fopen(filename, "w");
    }
}

uint8_t check_file_exist(char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}
