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
				ser_fdr(sd);
				break;
			case OP_CD:
				break;
			default:
				break;
		}
	}
}

// Send filenames present in current folder
void ser_fdr(int sd)
{
	// Define filename and filename array
	char filenames[BUF_SIZE] = "";
	int filecount = 0;
	
	// write the opcode to socket
	//if (write_opcode(des->sd, OP_PWD) == -1){
	if (write_opcode(sd, OP_FDR) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

	// Open current dir and struct
	DIR *dp; 
	struct dirent *direntp;
	dp = opendir(".");

	// insert the filenames 
	while (( direntp = readdir(dp)) != NULL )
	{
		strcat(filenames, direntp->d_name);
		strcat(filenames, "  ");
		filecount++;
	}
	
	rmReturnChar(filenames);

    int buflen = strlen(filenames);

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
	// send the file info
	//if (writen(des->sd, buf, strlen(buf)) == -1){
	if (writen(sd, filenames, buflen) == -1){
		fprintf(stderr, "Failed to send filenames\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful send filenames info\n");
	}

	fprintf(stdout, "Send FDR success\n");
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
		return;
	}
	else
    {
        fprintf(stdout, "Successful read length of %d\n", file_len);
	}

	// read the file name
	char file_name[file_len + 1];
	if (readn(sd, file_name, file_len) == -1)
	{
		fprintf(stderr, "Failed to send filename\n");
		return;
	}
	else
    {
		file_name[file_len] = '\0';
        fprintf(stdout, "Successful read filename of %s\n", file_name);
	}

	// set last character to null
	//file_name[strcspn(file_name, "\n")] = '\0';

	// check for file exist or error creating file
	ack_code = SUCCESS_CODE;
	if ((fd = open(file_name, O_RDONLY)) != -1)
	{
		ack_code = FILE_EXIST;
		fprintf(stderr, "Filename exist\n");
	}
	else if ((fd = open(file_name, O_WRONLY|O_CREAT, 0766)) == -1)
	{
		ack_code = ERROR_CODE;
		fprintf(stderr, "Failed to create file\n");
	}

	// write the opcode to socket
	if (write_opcode(sd, OP_PUT) == -1)
	{
		fprintf(stderr, "Failed to write opcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written opcode\n");
	}

	// write the ackcode to socket
	if (write_opcode(sd, ack_code) == -1)
	{
		fprintf(stderr, "Failed to write ackcode\n");
		return;
	}
    else
    {
        fprintf(stdout, "Successful written ackcode\n");
	}

	// check if sucessful ackcode was sent to client
	if (ack_code != SUCCESS_CODE)
	{
		fprintf(stderr, "PUT request failed\n");
		return;
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
		fprintf(stderr, "Failed to read size\n");
		return;
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
			fprintf(stdout, "Failed to read\n");
			return; // connection broken down
		}

		if ((nw = writen(fd, buf, nr)) < nr)
		{
			fprintf(stdout, "Failed to write\n");
			return;
		}
		file_size -= nw;
	}

	close(fd);
	fprintf(stdout, "File received\n");
}
