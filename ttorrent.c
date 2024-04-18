
// Trivial Torrent

// TODO: some includes here

#include "file_io.h"
#include "logger.h"
#include "string.h"
#include <sys/socket.h>
#include <unistd.h>


// TODO: hey!? what is this? I don't know my friend







/**
 * This is the magic number (already stored in network byte order).
 * See https://en.wikipedia.org/wiki/Magic_number_(programming)#In_protocols
 */
static const uint32_t MAGIC_NUMBER = 0xde1c3233; // = htonl(0x33321cde);

static const uint8_t MSG_REQUEST = 0;
static const uint8_t MSG_RESPONSE_OK = 1;
static const uint8_t MSG_RESPONSE_NA = 2;

#define SIZE 2048
#define SIZE_BUFFER 65535

enum { RAW_MESSAGE_SIZE = 13 };


/**
 * Main function.
 *
 */
int main(int argc, char **argv) {

	set_log_level(LOG_DEBUG);

	log_printf(LOG_INFO, "Trivial Torrent (build %s %s) by %s", __DATE__, __TIME__, "B.Moran and R.Sala");

	// ==========================================================================
	// Parse command line
	// ==========================================================================

	// TODO: some magical lines of code here that call other functions and do various stuff.

	// The following statements most certainly will need to be deleted at some point...
	(void) argc;
	(void) argv;
	(void) MAGIC_NUMBER;
	(void) MSG_REQUEST;
	(void) MSG_RESPONSE_NA;
	(void) MSG_RESPONSE_OK;

    struct torrent_t torrent;

    char downloads[SIZE];
    for (int i = 0; i < (int) strlen((const char *) argv); i++)
    {
        if(argv[i] == '.')
        {
            downloads[i] = '\0';
            break;
        }
        else
        {
            downloads[i] = argv[1][i];
        }
    }

    if (create_torrent_from_metainfo_file((const char *) argv, (struct torrent_t *) &torrent, (const char *) downloads) == -1) {
        perror("Metainfo error");
        return 1;
    }

    struct sockaddr_in serverAddr;
    for (int i = 0; i < (int) torrent.peer_count; i++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("Socket Error");
            return 1;
        }
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = torrent.peers[i].peer_port;

        if (connect(sock, (const struct *) &serverAddr, sizeof(serverAddr)) == -1) {
            perror("Connect Error");
            return 1;
        }
        else {

            for (uint64_t j = 0; j < torrent.block_count; j++) {
                uint8_t missatge[RAW_MESSAGE_SIZE];
                printf("Inici\n");
                missatge[0] = (uint8_t) ((MAGIC_NUMBER >> 24) & 0xff);
                missatge[1] = (uint8_t) ((MAGIC_NUMBER >> 16) & 0xff);
                missatge[2] = (uint8_t) ((MAGIC_NUMBER >> 8) & 0xff);
                missatge[3] = (uint8_t) (MAGIC_NUMBER & 0xff);
                missatge[4] = MSG_REQUEST;
                missatge[5] = (uint8_t) ((j >> 56) & 0xff);
                missatge[6] = (uint8_t) ((j >> 48) & 0xff);
                missatge[7] = (uint8_t) ((j >> 40) & 0xff);
                missatge[8] = (uint8_t) ((j >> 32) & 0xff);
                missatge[9] = (uint8_t) ((j >> 24) & 0xff);
                missatge[10] = (uint8_t) ((j >> 16) & 0xff);
                missatge[11] = (uint8_t) ((j >> 8) & 0xff);
                missatge[12] = (uint8_t) (j & 0xff);
                printf("Fi\n")

                if (send(sock, &missatge, sizeof(missatge), 0) == -1) {
                    perror("Send Error");
                }
                else {
                    sleep(1);

                    uint8_t resposta[SIZE_BUFFER];

                    ssize_t n = recv(sock, resposta, sizeof(resposta), 0);
                    if (n == -1) {
                        perror("recv func error");
                    }
                    else {
                        uint32_t 
                    }
                }
            }
            if(close(sock) == -1)
            {
                perror("Error al tancar socket");
                return 1;
            }
        }

        (void) argv;
        return 0;
    }

	return 0;
}
