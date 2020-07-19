/** File: 		    commandd.c for assignment 2 myftp (server side)
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
*/

#include "commandd.h"		/* head file all command function */

// log interactions with the clients
void log_message(char *file, const char *format, ...)
{
	// variables
	va_list ap;
	pid_t pid = getpid();
	char mytime[100];
	time_t raw_time;
	struct tm *time_info;

	// get the current time
	time(&raw_time);
	time_info = localtime(&raw_time);
	strftime(mytime, sizeof(mytime), "%b %d %R", time_info);

	FILE *fp;
	fp = fopen(file, "a");

	if (fp == NULL)
	{
		perror("Server: Log file");
		exit(0);
	}

	fprintf(fp, "%d %s - ", pid, mytime);

	// reference - https://www.tutorialspoint.com/c_standard_library/c_macro_va_start.htm
	// print all the arguments provided
	va_start(ap, format);
	vfprintf(fp, format, ap);
	va_end(ap);

	fclose(fp);
}

// process OPCODE recieved from client
void serve_a_client(int socket_desc, struct sockaddr_in cli_addr, char *file)
{
    printf("Client connection PID: %d IP: %s \n", getpid(), inet_ntoa(cli_addr.sin_addr));
	log_message(file, "Client connection PID: %d IP: %s \n", getpid(), inet_ntoa(cli_addr.sin_addr));
	char op_code;

	while (read_opcode(socket_desc, &op_code) > 0)
	{
		printf("OPCODE: %c\n", op_code);
		switch (op_code)
		{
			case OP_PUT:
				ser_put(socket_desc, file);
				break;
			case OP_GET:
				ser_get(socket_desc, file);
				break;
			case OP_PWD:
				ser_pwd(socket_desc, file);
				break;
			case OP_FDR:
				ser_fdr(socket_desc, file);
				break;
			case OP_CD:
			    ser_cd(socket_desc, file);
				break;
			default:
				break;
		}
	}
	return;
}

// DIR from client to list files in server
void ser_fdr(int socket_desc, char *file)
{
    // Define arrays
    char filestring[MAX_STRING_LENGTH];
	char * filearray[MAX_FILES_BUF];
	int filecount = 0;
	int buflen;
	// Define acknowledgement code
	char ack_code;

	// write the opcode to socket
	//if (write_opcode(des->socket_desc, OP_PWD) == -1){
	if (write_opcode(socket_desc, OP_FDR) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		log_message(file, "[DIR] Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
		log_message(file, "[DIR] Successful written opcode\n");
	}

	// Open current dir and struct
	DIR *dp;
	struct dirent *direntp;

    if ((dp = opendir(".")) == NULL)
	{
		// set to first entry
       	perror("Server: Error in opening directory\n");
       	ack_code = ERROR_CODE;
	}
    else
    {
    	ack_code = SUCCESS_CODE;
    }

    if (ack_code == SUCCESS_CODE || ack_code == EXCEED_LENGTH)
    {
        /* insert the filenames while at the same time limit the max files to 100. */
        while (( direntp = readdir(dp)) != NULL )
        {
            filearray[filecount] = direntp->d_name;
            filecount++;
            if (filecount >= MAX_FILES_BUF - 1)
            {
                fprintf(stderr, "Exceeded program capacity, truncated\n");
				log_message(file, "[DIR] Exceeded program capacity, truncated\n");
                ack_code = EXCEED_LENGTH;
                break;
            }
        }

        // Sorts the files by filename
        qsort(&filearray[0], filecount, sizeof(char *), cmpstringp);

        strcpy(filestring, " ");

        for (int i = 0; i < filecount; i++)
        {
            strcat(filestring, filearray[i]);
            strcat(filestring, "\t");
        }

        rmReturnChar(filestring);
        buflen = strlen(filestring);
    }
    else
    {
        buflen = 0;
    }
    // write the ackcode to socket
	//if (write_opcode(des->socket_desc, OP_PWD) == -1){
	if (write_opcode(socket_desc, ack_code) == -1)
	{
		fprintf(stderr, "Failed to write ackcode\n");
		log_message(file, "[DIR] Failed to write ackcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written ackcode\n");
		log_message(file, "[DIR] Successful written ackcode\n");
	}

    // send the length of working dir
    if (write_length(socket_desc, buflen) == -1)
    {
        fprintf(stderr, "Failed to send length\n");
		log_message(file, "[DIR] Failed to send length\n");
        return;
    }
    else
    {
        fprintf(stdout, "Successful written length of %d\n", buflen);
		log_message(file, "[DIR] Successful written length of %d\n", buflen);
    }

    // Send the file info only if success code
    if (ack_code == SUCCESS_CODE || ack_code == EXCEED_LENGTH)
    {
        // send the file info
        //if (writen(des->socket_desc, buf, strlen(buf)) == -1){
        if (writen(socket_desc, filestring, buflen) == -1){
            fprintf(stderr, "Failed to send filenames\n");
			log_message(file, "[DIR] Failed to send filenames\n");
            return;
        }
        else
        {
            fprintf(stdout, "Successful send filenames info\n");
			log_message(file, "[DIR] Successful send filenames info\n");
        }

        fprintf(stdout, "Send FDR success\n");

		log_message(file, "[DIR] Send FDR success\n");

    }
}

// PWD from client to display current working directory of server
void ser_pwd(int socket_desc, char *file)
{
	char buf[BUF_SIZE];

	getcwd(buf, sizeof(buf));
	printf("BUF: %s\n", buf);

	// write the opcode to socket
	//if (write_opcode(des->socket_desc, OP_PWD) == -1){
	if (write_opcode(socket_desc, OP_PWD) == -1){
		fprintf(stderr, "Failed to write opcode\n");
		log_message(file, "[PWD] Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
		log_message(file, "[PWD] Successful written opcode\n");
	}

    int buflen = strlen(buf);
	// send the length of working dir
	//if (write_length(des->socket_desc, strlen(buf)) == -1){
	if (write_length(socket_desc, buflen) == -1){
		fprintf(stderr, "Failed to send length\n");
		log_message(file, "[PWD] Failed to send length\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written length of %d\n", buflen);
		log_message(file, "[PWD] Successful written length of %d\n", buflen);
	}
	// send the dir info
	//if (writen(des->socket_desc, buf, strlen(buf)) == -1){
	if (writen(socket_desc, buf, buflen) == -1){
		fprintf(stderr, "Failed to send directory info\n");
		log_message(file, "[PWD] Failed to send directory info\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful send directory info\n");
		log_message(file, "[PWD] Successful send directory info\n");
	}

	fprintf(stdout, "Send PWD success\n");
	log_message(file, "[PWD] Send PWD success\n");
}

// CD from client to change the directory of server
void ser_cd(int socket_desc, char *file)
{
    int file_size;
	char ack_code;

	if(read_length(socket_desc,&file_size) == -1)
    {
		perror("Server: Failed to read file size from client\n");
		log_message(file, "[CD] Server: Failed to read file size from client\n");
		return;
	}

	char buf[file_size+1];

	if(readn(socket_desc, buf, file_size) == -1)
    {
		perror("Server: Failed to read cd dir from client\n");
		log_message(file, "[CD] Server: Failed to read cd dir from client\n");
		return;
	}
    else
    {
        buf[file_size] = '\0';
        fprintf(stdout, "Server received cd instruction: %s\n", buf);
		log_message(file, "[CD] Server received cd instruction: %s\n", buf);
    }

    // change directory here and set the ack code to send back to client
	if(chdir(buf) == 0)
    {
		ack_code = SUCCESS_CODE;
		fprintf(stdout,"Server done cd to: %s\n", buf);
		log_message(file, "[CD] Server done cd to: %s\n", buf);
	}
	else
	{
		ack_code = ERROR_CODE;
		fprintf(stderr,"Server unable to cd to: %s\n", buf);
		log_message(file, "[CD] Server unable to cd to: %s\n", buf);
	}

	if(write_opcode(socket_desc, OP_CD) == -1)
    {
		perror("Server: Failed to send opcode to client\n");
		log_message(file, "[CD] Server: Failed to send opcode to client\n");
		return;
	}

	if(write_opcode(socket_desc, ack_code) == -1)
    {
		perror("Server: Failed to send ackcode to client\n");
		log_message(file, "[CD] Server: Failed to send ackcode to client\n");
		return;
	}

}

// PUT from client to upload files to server
void ser_put(int socket_desc, char *file)
{
	char op_code, ack_code;
	int file_len, fd, file_size, block_size, nr, nw;
	char buf[BUF_SIZE];

	// read the file length
	if (read_length(socket_desc, &file_len) == -1)
	{
		fprintf(stderr, "Failed to read length\n");
		log_message(file, "[PUT] \n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful read length of %d\n", file_len);
		log_message(file, "[PUT] \n");
	}

	// read the file name
	char file_name[file_len + 1];
	if (readn(socket_desc, file_name, file_len) == -1)
	{
		fprintf(stderr, "Failed to read filename\n");
		log_message(file, "[PUT] \n");
		return;
	}
	else
    {
		file_name[file_len] = '\0';
        fprintf(stdout, "Successful read filename of %s\n", file_name);
		log_message(file, "[PUT] \n");
	}

	// check for file exist or error creating file
	ack_code = SUCCESS_CODE;

	if ((fd = open(file_name, O_RDONLY)) >= 0)
	{
		ack_code = FILE_EXIST;
		fprintf(stderr, "Filename exist\n");
		log_message(file, "[PUT] Filename exist\n");
	}
	else if ((fd = open(file_name, O_WRONLY|O_CREAT, 0766)) == -1)
	{
		ack_code = ERROR_CODE;
		fprintf(stderr, "Failed to create file\n");
		log_message(file, "[PUT] Failed to create file\n");
	}

	// write the opcode to socket
	if (write_opcode(socket_desc, OP_PUT) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		log_message(file, "[PUT] Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
		log_message(file, "[PUT] Successful written opcode\n");
	}

	// write the ackcode to socket
	if (write_opcode(socket_desc, ack_code) == -1)
	{
		fprintf(stderr, "Failed to write ackcode\n");
		log_message(file, "[PUT] Failed to write ackcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written ackcode\n");
		log_message(file, "[PUT] Successful written ackcode\n");
	}

	// check if sucessful ackcode was sent to client
	if (ack_code != SUCCESS_CODE)
	{
		fprintf(stderr, "PUT request failed\n");
		log_message(file, "[PUT] PUT request failed\n");
		return;
	}

	// reading the respond from client
	if (read_opcode(socket_desc, &op_code) == -1)
	{
		fprintf(stderr, "Failed to read opcode\n");
		log_message(file, "[PUT] Failed to read opcode\n");
	}
	else
    {
        fprintf(stdout, "Successful reading opcode\n");
		log_message(file, "[PUT] Successful reading opcode\n");
	}

	// read the file size
	if (read_length(socket_desc, &file_size) == -1)
	{
		fprintf(stderr, "Failed to read size\n");
		log_message(file, "[PUT] Failed to read size\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful read file size of %d\n", file_size);
		log_message(file, "[PUT] Successful read file size of %d\n", file_size);
	}

	block_size = BUF_SIZE;

	while (file_size > 0)
	{
		if (block_size > file_size)
        	{
            		block_size = file_size;
        	}

		if ((nr = readn(socket_desc, buf, block_size)) == -1)
		{
			fprintf(stdout, "Failed to read\n");
			log_message(file, "[PUT] Failed to read\n");
			return; // connection broken down
		}

		if ((nw = writen(fd, buf, nr)) < nr)
		{
			fprintf(stdout, "Failed to write\n");
			log_message(file, "[PUT] Failed to write\n");
			return;
		}
		file_size -= nw;
	}

	close(fd);
	fprintf(stdout, "File received\n");
	log_message(file, "[PUT] File received\n");
}

// GET from client to download named file from server
void ser_get(int socket_desc, char *file)
{
	char ack_code;
	int fd, file_size, file_len, nr;
	struct stat stats;
	char buf[BUF_SIZE];

	// read the file length
	if (read_length(socket_desc, &file_len) == -1)
	{
		fprintf(stderr, "Failed to read length\n");
		log_message(file, "[GET] Failed to read length\n");
		return;
	}
	else
    {
        fprintf(stdout, "Successful read length of %d\n", file_len);
		log_message(file, "[GET] Successful read length of %d\n", file_len);
	}

	// read the file name
	char file_name[file_len + 1];
	if (readn(socket_desc, file_name, file_len) == -1)
	{
		fprintf(stderr, "Failed to read filename\n");
		log_message(file, "[GET] Failed to read filename\n");
		return;
	}
	else
    {
		file_name[file_len] = '\0';
        fprintf(stdout, "Successful read filename of %s\n", file_name);
		log_message(file, "[GET] Successful read filename of %s\n", file_name);
	}

	// check for file not exist or error reading file
	ack_code = SUCCESS_CODE;

	if (access(file_name, F_OK) != 0)
	{
		ack_code = FILE_NOT_EXIST;
		fprintf(stderr, "File does not exist\n");
		log_message(file, "[GET] File does not exist\n");
	}
	else if ((fd = open(file_name, O_RDONLY)) < 0)
	{
		ack_code = ERROR_CODE;
		fprintf(stderr, "Failed to read file\n");
		log_message(file, "[GET] Failed to read file\n");
	}
    else if(lstat(file_name, &stats) < 0) // check for fstat
    {
		ack_code = ERROR_CODE;
        fprintf(stderr, "Failed to read fstat\n");
		log_message(file, "[GET] Failed to read fstat\n");
		//return;
    }

	// write the opcode to socket
	if (write_opcode(socket_desc, OP_GET) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		log_message(file, "[GET] Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
		log_message(file, "[GET] Successful written opcode\n");
	}

	// write the ackcode to socket
	if (write_opcode(socket_desc, ack_code) == -1)
	{
		fprintf(stderr, "Failed to write ackcode\n");
		log_message(file, "[GET] Failed to write ackcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written ackcode\n");
		log_message(file, "[GET] Successful written ackcode\n");
	}

	// check if sucessful ackcode was sent to client
	if (ack_code == SUCCESS_CODE)
	{
		if (write_opcode(socket_desc, OP_GET) == -1)
		{
			fprintf(stderr, "Failed to write opcode\n");
			log_message(file, "[GET] Failed to write opcode\\n");
			return;
		}
		else
		{
			fprintf(stdout, "Successful written opcode\n");
			log_message(file, "[GET] Successful written opcode\\n");
		}

		file_size = stats.st_size; // set file size

		if (write_length(socket_desc, file_size) == -1)
		{
			fprintf(stderr, "Failed to send length\n");
			log_message(file, "[GET] Failed to send length\n");
			return;
		}
		else
		{
			fprintf(stdout, "Successful read filesize of %d\n", file_size);
			log_message(file, "[GET] Successful read filesize of %d\n", file_size);
		}

		while ((nr = readn(fd, buf, BUF_SIZE)) > 0)
		{
			if (writen(socket_desc, buf, nr) == -1)
			{
				fprintf(stdout, "Failed to send file content\n");
				log_message(file, "[GET] Failed to send file content\n");
				return;
			}
		}

		fprintf(stdout, "File sent\n");
		log_message(file, "[GET] File sent\n");
	}
	else
	{
		fprintf(stderr, "GET request failed\n");
		log_message(file, "[GET] GET request failed\n");
		return;
	}

	close(fd);
}
