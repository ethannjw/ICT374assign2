/** File: 		    commandd.c
*   Authors: 		Neo Kim Heok (33747085) and Ng Jing Wei (33804877)
*   Date:		    25th July 2020
*   Purpose:		This is the command driver code for running the commands for FTP
*/

#include "commandd.h"

// Process OPCODE recieved from client
void serve_a_client(int sd)
{
	char op_code;

	while (read_opcode(sd, &op_code) > 0)
	{
		printf("OPCODE: %c\n", op_code);
		switch (op_code)
		{
			case OP_PUT:
				ser_put(sd);
				break;
			case OP_GET:
				break;
			case OP_PWD:
				ser_pwd(sd);
				break;
			case OP_FDR:
				break;
			case OP_CD:
				break;
			default:
				break;
		}
	}
}

// PWD from client to display cwd of server.
//void ser_pwd(cli_desc *des)
void ser_pwd(int sd)
{
	char buf[BUF_SIZE];

	getcwd(buf, sizeof(buf));
	printf("BUF: %s\n", buf);

	// write the opcode to socket
	//if (write_opcode(des->sd, OP_PWD) == -1){
	if (write_opcode(sd, OP_PWD) == -1){
		fprintf(stderr, "Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

    int buflen = strlen(buf);
	// send the length of working dir
	//if (write_length(des->sd, strlen(buf)) == -1){
	if (write_length(sd, buflen) == -1){
		fprintf(stderr, "Failed to send length\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written length of %d\n", buflen);
	}
	// send the dir info
	//if (writen(des->sd, buf, strlen(buf)) == -1){
	if (writen(sd, buf, buflen) == -1){
		fprintf(stderr, "Failed to send directory info\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful send directory info\n");
	}

	fprintf(stdout, "Send PWD success\n");
}

// PUT from client to upload files to server.
void ser_put(int sd)
{
	char op_code, ack_code;
	int file_len, fd, file_size, block_size, nr, nw;
	char buf[BUF_SIZE];

	// read the file length
	if (read_length(sd, &file_len) == -1)
	{
		fprintf(stderr, "Failed to send length\n");
		exit(1);
	}
	else
    {
        fprintf(stdout, "Successful read length of %d\n", file_len);
	}

	// read the file name
	char file_name[file_len];
	if (readn(sd, file_name, file_len) == -1)
	{
		fprintf(stderr, "Failed to send filename\n");
		exit(1);
	}
	else
    {
        fprintf(stdout, "Successful read filename of %s\n", file_name);
	}

	// set last character to null
	file_name[strcspn(file_name, "\n")] = '\0';

	// check for file exist or error creating file
	ack_code = SUCCESS_CODE;
	if ((fd = open(file_name, O_RDONLY)) != -1)
	{
		ack_code = FILE_EXIST;
		fprintf(stderr, "Filename exist\n");
	}
	else if ((fd = open(file_name, O_WRONLY|O_CREAT, 0766)) != -1)
	{
		ack_code = ERROR_CODE;
		fprintf(stderr, "Failed to create file\n");
	}

	// write the opcode to socket
	if (write_opcode(sd, OP_PUT) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		exit(1);
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

	// write the ackcode to socket
	if (write_opcode(sd, ack_code) == -1)
	{
		fprintf(stderr, "Failed to write ackcode\n");
		exit(1);
	}
    else
    {
        fprintf(stdout, "Successful written ackcode\n");
	}

	// check if sucessful ackcode was sent to client
	if (ack_code != SUCCESS_CODE)
	{
		fprintf(stderr, "PUT request failed\n");
		exit(1);
	}

	// reading the respond from client
	if (read_opcode(sd, &op_code) == -1)
	{
		fprintf(stderr, "Failed to read opcode\n");
	}
	else
    {
        fprintf(stdout, "Successful reading opcode\n");
	}

	// read the file size
	if (read_length(sd, &file_size) == -1)
	{
		fprintf(stderr, "Failed to send size\n");
		exit(1);
	}
	else
    {
        fprintf(stdout, "Successful read file size of %d\n", file_size);
	}

	block_size = BUF_SIZE;

	while (file_size > 0)
	{
		if (block_size > file_size)
			block_size = file_size;
		
		if ((nr = readn(sd, buf, block_size)) == -1)
		{
			return; // connection broken down
		}

		if ((nw = writen(fd, buf, nr)) < 0)
		{
			return;
		}
		file_size -= nw;
	}

	close(fd);
}