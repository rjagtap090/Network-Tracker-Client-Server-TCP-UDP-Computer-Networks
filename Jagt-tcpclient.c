/* tcp_ client.c */
/* Programmed by Adarsh Sethi */
/* Sept. 19, 2022 */

#include <stdio.h>      /* for standard I/O functions */
#include <stdlib.h>     /* for exit */
#include <string.h>     /* for memset, memcpy, and strlen */
#include <netdb.h>      /* for struct hostent and gethostbyname */
#include <sys/socket.h> /* for socket, connect, send, and recv */
#include <netinet/in.h> /* for sockaddr_in */
#include <unistd.h>     /* for close */
#include <stdbool.h>    /* for bool */
#include <sys/types.h>  /*defines data types used in system source code*/
#include <sys/socket.h> /*indicates that the data array contains the access rights to be sent or received*/
#include <netinet/in.h> /*contains definitions for the internet protocol family*/
#include <arpa/inet.h>  /*contains definitions for internet operations*/
#include <errno.h>      /*defines the integer variable errno, which is set by system calls and some library functions in the event of an error to indicate what went wrong*/
#include <time.h>       /*definitions of functions to get and manipulate date and time information*/

const int C_port = 25507;             /*(using the port value from project 0)declares a constant integer variable C_port with a value of 25507*/
const int initial_num_port = 50000;   /*declares a constant integer variable initial_num_port with a value of 50000*/
const char *C_name = "RISHABH";      /* declares a constant character pointer variable C_name with a value of "RISHABH"*/
const char *Serv_h = "localhost";   /*declares a constant character pointer variable*/

#define STRING_SIZE 1024           /*defines the size of a string as 1024 characters*/
struct header_data find_record(unsigned short); /*declares a function called find_record that takes one unsigned short as a parameter*/
void locate_new_rec(struct header_data); /*declares a function called locate_new_rec that takes one struct header_data as a parameter*/
struct header_data                    /*declares a struct called header_data with four unsigned shorts and one char array of size 80*/
 {
   unsigned short following_num;
   unsigned short CP_num;
   unsigned short serv_num;
   unsigned short sec_code;
   char text[80];
};

int main(void)
{

   int sock_client; /* Socket used by client */

   struct sockaddr_in client_addr; /* Internet address structure that
                                      stores client address */

   struct sockaddr_in server_addr; /* Internet address structure that
                                       stores server address */
   struct hostent *server_hp;      /* Structure to store server's IP
                                       address */
   unsigned short serv_port_num;   /* Port number used by server */
   unsigned short cli_port;        /* Port number used by client */

   int bytes_sent, bytes_recd; /* number of bytes sent or received */

   struct header_data c_info, s_info;

   struct timeval session_close;

   /* Note: there is no need to initialize local client address information
            unless you want to specify a specific local port
            (in which case, do it the same way as in udpclient.c).
            The local address initialization and binding is done automatically
            when the connect function is called later, if the socket has not
            already been bound. */

   /* clear client address structure and initialize with client address */
   memset(&client_addr, 0, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* This allows choice of
                                        any host interface, if more than one
                                        are present */
   cli_port = C_port; /*assigns the value of the constant C_port to the variable cli_port*/
   client_addr.sin_port = htons(cli_port); /*assigns the value of cli_port to the sin_port field of the client_addr structure, using htons() to convert the value to the appropriate network byte order*/

   serv_port_num = initial_num_port; /*sets the value of serv_port_num to the constant initial_num_port*/
   do /* will continuously loop until the variable serv_port_num is greater than the sum of initial_num_port and 10000. The loop will start from the do statement, and end when the condition in the while statement is met.*/
   {
      serv_port_num = serv_port_num + 1; /*increments the value of the serv_port_num variable by 1*/

      memset(&c_info, 0, sizeof(c_info)); /*sets all the bytes in the client information to 0*/
      memset(&s_info, 0, sizeof(s_info)); /*sets all the bytes in the  server information to 0*/

      c_info = find_record(serv_port_num);   /*the find_record() function and passes serv_port_num as an argument. The return value of this function is stored in the c_info variable*/
      if (ntohs(c_info.following_num) == 4) /*checks if the value of the following_num field in the c_info structure is equal to 4. If it is, the loop continues and the statements below this line are skipped*/
      {
         continue;
      }

      /* open a socket */
      if ((sock_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      {
         perror("Client: can't open stream socket");
         exit(1);
      }

      /* bind the socket to the local client port */
      while (true)
      {
         if (bind(sock_client, (struct sockaddr *)&client_addr, sizeof(client_addr)) >= 0)
         {
            break;
         }
         sleep(3);
      }

      /* initialize server address information */
      server_hp = gethostbyname(Serv_h);

      /* Clear server address structure and initialize with server address */
      struct sockaddr_in server_addr;
      memset(&server_addr, 0, sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(serv_port_num);
      inet_pton(AF_INET, server_hp->h_addr, &(server_addr.sin_addr));

      /* connect to the server */
      if (connect(sock_client, (struct sockaddr *)&server_addr,
                  sizeof(server_addr)) < 0)
      {
         close(sock_client);
         continue;
      }

      /* send message */
      bytes_sent = send(sock_client, &c_info, sizeof(c_info), 0);

      session_close.tv_sec = 0.5;  /*assigning the tv_sec variable of the session_close struct to 0.5. This variable is used to indicate how long the socket will wait for data before timing out*/
      setsockopt(sock_client, SOL_SOCKET, SO_RCVTIMEO,  /*using the setsockopt() function to set the socket options for the socket sock_client. The SOL_SOCKET parameter is specified to indicate that the socket layer options should be set. The SO_RCVTIMEO option is specified to indicate that the timeout value for the socket should be set to the value of the session_close struct. The final parameter is the size of the session_close struct in bytes*/
                 (const void *)&session_close, sizeof(session_close));

      /* get response from server */
      bytes_recd = recv(sock_client, &s_info, sizeof(s_info), 0);

      if (bytes_recd <= 0) /*checks if the number of bytes received from a socket connection is less than or equal to 0. If it is, the code closes the socket connection and continues to the next iteration*/
      {
         close(sock_client);
         continue;
      }

      locate_new_rec(s_info);
      /*printing out a message sent and a message received.The first line calls a function, locate_new_rec, which takes in a parameter 's_info'*/
      printf("Message Sent:         %hu,     %hu,     %hu,     %hu,     %s\n",
             ntohs(c_info.following_num),
             ntohs(c_info.CP_num),
             ntohs(c_info.serv_num),
             ntohs(c_info.sec_code),
             c_info.text); 
             /* print out the message sent and received, using the parameters in the structs c_info and s_info. The ntohs function is used to convert from network byte order to host byte order.*/
      printf("Message Received:     %hu,     %hu,     %hu,     %hu,     %s\n",
             ntohs(s_info.following_num),
             ntohs(s_info.CP_num),
             ntohs(s_info.serv_num),
             ntohs(s_info.sec_code),
             s_info.text);

      /* close the socket */
      close(sock_client);

   } while (serv_port_num < initial_num_port + 10000);
}

struct header_data find_record(unsigned short port_number)

{
   FILE *C_f_data = fopen("client.txt", "r");  /*opens a text file called "client.txt" and reads data from it. */

   if (C_f_data == NULL)
   {
      perror("client.txt file could not be opened.");
      exit(1);
   }
   char filefollowing_num[6];
   char fileserv_num[6];
   char filesec_code[6];
   char file_serv_loc[80];
   char str_port[6];
   
   struct header_data record;        /*creates a struct called "record" which stores the following information: following_num, CP_num, serv_num, sec_code, and text. */
   record.following_num = htons(1);
   record.CP_num = htons((unsigned short)C_port);
   record.serv_num = htons(0);
   record.sec_code = htons(0);
   strcpy(record.text, "*");

   sprintf(str_port, "%hu", port_number);         /*stores the port number passed in as an argument to the function.*/

   while (fscanf(C_f_data, "%[^,], %[^,], %[^,], %[^\n]\n", filefollowing_num, fileserv_num, filesec_code, file_serv_loc) == 4)
   {
      if (strcmp(str_port, fileserv_num) == 0)          /*reads data from the text file until it finds a line with the same port number as the one stored in*/
      {                                                                      
         unsigned short temp = (unsigned short)atoi(filefollowing_num);         /*stores the following_num, serv_num, and sec_code as unsigned shorts in the struct "record" and if the following_num is equal to 3, 
                                                                                          it stores the client's name in the "text" field of the struct.*/
         record.following_num = htons(temp + 1);
         record.serv_num = htons((unsigned short)atoi(fileserv_num));
         record.sec_code = htons((unsigned short)atoi(filesec_code));
         if (temp + 1 == 3)
         {
            strcpy(record.text, C_name);                      /*stores the client's name in the "text" field of the struct*/
         }
         break;
      }
   }

   fclose(C_f_data);                           /*loses the text file and returns the struct "record".*/

   return record;
}

void locate_new_rec(struct header_data record) /*function definition that takes a header_data structure as an argument and is called locate_new_rec.*/
{
   FILE *C_f_data, *new_C_f_data;                  /*Two file pointers are declared, one for the file "client.txt" and one for the file "tempClient.txt".*/

   C_f_data = fopen("client.txt", "r");          /*The first file pointer is used to open the file "client.txt" in read mode.*/
   new_C_f_data = fopen("tempClient.txt", "w+");  /* The second file pointer is used to open the file "tempClient.txt" in write plus mode.*/
                                                /*It is checking to see if two files, "client.txt" and "tempClient.txt", can be opened.
                                                 If either of them cannot be opened, an error message is printed with the "perror()" function and the program is exited with the "exit()" function. */
   if (NULL == C_f_data)
   {
      perror("client.txt file could not be opened.");
      exit(1);
   }                                                          /*checks if the "C_f_data" pointer is equal to NULL, which would mean the "client.txt" file could not be opened. 
                                                                Then checks if the "new_C_f_data" pointer is equal to NULL, which would mean the "tempClient.txt" file could not be opened.*/
   if (NULL == new_C_f_data)
   {
      perror("tempClient.txt file could not be opened.");
      exit(1);
   }
                                     /*values are updated after loop*/
   char following_num[6];
   char serv_num[6];
   char sec_code[6];
   char serverTravelLocation[80];

   strcpy(serverTravelLocation, record.text);

   char filefollowing_num[6];
   char fileserv_num[6];
   char filesec_code[6];
   char file_serv_loc[80];

   bool record_found = false;
     
   sprintf(following_num, "%hu", ntohs(record.following_num));
   sprintf(serv_num, "%hu", ntohs(record.serv_num));
   sprintf(sec_code, "%hu", ntohs(record.sec_code));
/*The while loop will run until the fscanf function returns a value that is not equal to 4. 
The fscanf function scans the "C_f_data" file and assigns the values of each comma-separated value to the variables "filefollowing_num", "fileserv_num", "filesec_code", and "file_serv_loc". */
   while(fscanf(C_f_data, "%[^,], %[^,], %[^,], %[^\n]\n", filefollowing_num, fileserv_num, filesec_code, file_serv_loc) == 4)
   {
      switch (strcmp(fileserv_num, serv_num)) /*compares the value of "fileserv_num" to the value of "serv_num". 
*/
      {
      case 0:                    /*if values are equal, the "record_found" variable is set to true and the new values are written to the "new_C_f_data" file using the "fprintf" function. */
         record_found = true;
         fprintf(new_C_f_data, "%s, %s, %s, %s\n", following_num, serv_num, sec_code, serverTravelLocation);
         break;
      default:                  /* values are not equal, the old values for the comma-separated values are written to the "new_C_f_data" file using the "fprintf" function.*/
         fprintf(new_C_f_data, "%s, %s, %s, %s\n", filefollowing_num, fileserv_num, filesec_code, file_serv_loc);
         break;
      }
   }

   switch (record_found) /*It is searching for a record in a data file. The variable record_found is a boolean (true/false) that lets the code know whether the record was found.*/
   {
   case false:          /*"%s" giving the order to print the information,USING A FORMAT SPECIFIER :If the record was not found (record_found is false), then this line of code will print the following information to a new data file: following_num, serv_num, sec_code, and serverTravelLocation.*/
      fprintf(new_C_f_data, "%s, %s, %s, %s\n", following_num, serv_num, sec_code, serverTravelLocation);
   }

   fclose(C_f_data);    /*The file pointer for "client.txt" is closed.*/
   fclose(new_C_f_data);  /*The file pointer for "tempClient.txt" is closed.*/

   system("mv tempClient.txt client.txt");     /*"mv tempClient.txt client.txt" is executed which moves the contents of "tempClient.txt" to "client.txt".*/
}