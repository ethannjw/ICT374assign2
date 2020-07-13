// File:	file_handler.c
// Author:      Ng Jing Wei 33804877
// Purpose:	Program that is able to retrive all attributes of file and print them out
// Usage: 	No arguments: Prints out all files in current working directory
//		Enter full path of file as argument: Prints out all attributes of each file
//	 		This file contains the following methods:
//			- pwd - to display the current directory of the server that is serving the client
//			- lpwd - to display the current directory of the client
//			- dir - to display the file names under the current directory of the server that is serving the client
//			- ldir - to display the file names under the current directory of the client
//			- cd [directory_pathname] - to change the current directory of the server that is serving the client
//			- lcd directory_pathname - to change the current directory of the client
//			- get [filename] - to download the named file from the current directory of the remote server and save it in the current directory of the client
//			- put [filename] - to upload the named file from the current directory of the client to the current directory of the remove server

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>            // for getting the usernames
#include <time.h>           // for formatting the time correctly
#include <sys/sysmacros.h>  // for major and minor functions getting the device numbers
#include <grp.h>            // for getting the group name

// Defining all colour
#define COLOUR_RED "\x1b[31m"
#define COLOUR_GREEN "\x1b[32m"
#define COLOUR_YELLOW "\x1b[33m"
#define COLOUR_BLUE "\x1b[34m"
#define COLOUR_MAGENTA "\x1b[35m"
#define COLOUR_CYAN "\x1b[36m"
#define COLOUR_RESET "\x1b[0m"

//declare global variables
struct passwd *pws;	// password object for getting user name
struct group *grp;      // group object for getting group name
struct tm *lt;		// declare the struct tm pointer which contains the localtime
struct stat filebuf;    // stat object of file
time_t timenow;         // variable holds the time_now
char timebuf[14];       // buffer that holds the file time
struct tm *tn;          // struct that holds the time now
char yearnowbuf[5];     // buffer that holds the current year
char yearfilebuf[5];    // buffer that holds the year of file param
int errno;              // define variable to store the errno

void print_readable_time(time_t t)
{
    	// Reference:
	// https://stackoverflow.com/questions/13542345/how-to-convert-st-mtime-which-get-from-stat-function-to-string-or-char#:~:text=You%20need%20localtime(3)%20to,%25c%22%2C%20%3C)%3B
	lt = localtime(&t);
    	strftime(yearfilebuf, sizeof(yearfilebuf), "%Y", lt);
    	if (strcmp(yearfilebuf, yearnowbuf) == 0)
        {
		// if the year is equal
		// prints out in format Jun 11 16:54
		strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", lt);
    	}
	else
	{
		// if the year is not equal
		// prints out in format Jun 11 2020
        	strftime(timebuf, sizeof(timebuf), "%b %d %Y", lt);
	}
    	printf("%12s ", timebuf);
}

void print_user_id(struct stat *f)
{
    	// getpwid takes in a dtype of uid_t
	// get the pwid and pass it into the struct pws
    	pws = getpwuid(f->st_uid);
    	if (pws != NULL)
        {
		printf("%7s ", pws->pw_name);
    	}
	else
        {
		printf(" ????? ");
	}
}

void print_group_id(struct stat *f)
{
    	// getpwid takes in a dtype of uid_t
	// get the pwid and pass it into the struct grp
    	grp = getgrgid(f->st_gid);
    	if (grp != NULL)
	{
        	printf("%7s ", grp->gr_name);
	}
    	else
        {
		printf(" ????? ");
	}
}

void print_file_type_perm(struct stat *f)
{
    	// Check for the type of file
    	if (S_ISDIR(f->st_mode))
	{
        	printf("d");
	}
    	else if (S_ISREG(f->st_mode))
	{
        	printf("-");
	}
    	else if (S_ISCHR(f->st_mode))
	{
        	printf("c");
	}
    	else if (S_ISBLK(f->st_mode))
	{
        	printf("b");
	}
    	else if (S_ISFIFO(f->st_mode))
	{
        	printf("p");
	}
    	else if (S_ISLNK(f->st_mode))
	{
        	printf("l");
	}
    	else if (S_ISSOCK(f->st_mode))
	{
        	printf("s");
	}
    	else
	{
        	printf("?");
	}

    	// Check for the permissions using Ternary Operators
    	// Reference:
	// http://codewiki.wikidot.com/c:system-calls:stat
	printf( (f->st_mode & S_IRUSR) ? "r" : "-");
	printf( (f->st_mode & S_IWUSR) ? "w" : "-");
	printf( (f->st_mode & S_IXUSR) ? "x" : "-");
	printf( (f->st_mode & S_IRGRP) ? "r" : "-");
	printf( (f->st_mode & S_IWGRP) ? "w" : "-");
	printf( (f->st_mode & S_IXGRP) ? "x" : "-");
	printf( (f->st_mode & S_IROTH) ? "r" : "-");
	printf( (f->st_mode & S_IWOTH) ? "w" : "-");
	printf( (f->st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
}

void print_file_size(struct stat *f)
{
    // Print out file size giving space for at least 5 spaces
    printf("%5ld ", f->st_size);
}

void print_no_links(struct stat *f)
{
    printf("%ld ", f->st_nlink);
}

void print_last_access(struct stat *f)
{
    printf(COLOUR_YELLOW);
    print_readable_time(f->st_atime);
    printf(COLOUR_RESET);
}

void print_last_mod(struct stat *f)
{
    printf(COLOUR_RESET);
    print_readable_time(f->st_mtime);
    printf(COLOUR_RESET);
}

void print_status_changed(struct stat *f)
{
    printf(COLOUR_GREEN);
    print_readable_time(f->st_ctime);
    printf(COLOUR_RESET);
}

void print_inode(struct stat *f)
{
    	if ( f->st_ino > 1000000)
	{
		// If inode is at least 7 digits long, give 18 spaces
        	printf(COLOUR_BLUE "%18ld " COLOUR_RESET, f->st_ino);
	}

    	else
	{
		// If less than 7 digits long, give 7 spaces
        	printf(COLOUR_BLUE "%7ld " COLOUR_RESET, f->st_ino);
	}
}

void print_dev_no(struct stat *f)
{
	printf("%d/%d ", major(f->st_dev), minor(f->st_dev));
    	if (S_ISCHR(f->st_mode) || S_ISBLK(f->st_mode))
    	{
		// if special file e.g. character or block, print out the rdev as well
        	printf("%d/%d ", major(f->st_rdev), minor(f->st_rdev));
    	}
}

void print_file_details(char * filename)
{
    	if (lstat(filename, &filebuf) < 0)
    	{
        	fprintf(stderr, "myls: Unable to access '%s': %s\n", filename, strerror( errno ));
		return;
    	}
	print_file_type_perm(&filebuf);  // 3,4
	print_no_links(&filebuf);        // 9
	print_user_id(&filebuf);         // 1
	print_group_id(&filebuf);        // 2
	print_file_size(&filebuf);       // 5
	print_last_mod(&filebuf);        // 11
	print_inode(&filebuf);           // 6
	print_dev_no(&filebuf);          // 7, 8
	print_last_access(&filebuf);     // 10
	print_status_changed(&filebuf);  // 12
	printf("%s\n", filename);
}

static int cmpstringp(const void *p1, const void *p2)
{
    	// cmp string pointer of pointers taken from qsort man page
    	// used in the qsort function
    	return strcmp(* (char * const *) p1, * (char * const *) p2);

}

int main(int argc, char *argv[])
{

	char dirname[256];
    	char * filename;
	// can only work with 100 files max. any more will require a use of dynamic arrays
    	char * filearray[100];
	// no of file counter
    	size_t fcount = 0;

	// pointer to directory
	DIR *dp;

	// struct pointer to directory entry
	struct dirent *direntp;

	// get the current time
	time(&timenow);
	// store it in the tn struct
	tn = localtime(&timenow);

	// get the current year
	strftime(yearnowbuf, sizeof(yearnowbuf), "%Y", tn);

	if (argc == 1)
	{
		// list the current directory files mode
		strcpy(dirname, ".");

        if ((dp = opendir(dirname)) == NULL)
	{
		// set to first entry
            	printf("Error in opening directory %s\n", dirname);
            	exit(EXIT_FAILURE);
	}

	// Loop through all files
        while ( ( direntp = readdir(dp) ) != NULL )
        {

        	filename = direntp->d_name;
            	if (filename[0] != '.')
		{
			// do not print out hidden files
	                // add filename to file array
        	        filearray[fcount++] = filename;
                	if (fcount >= 100)
                	{
                    		fprintf(stderr, "Exceeded program capacity, truncated\n");
                    		break;
                	}

            	} // endif

        } // end while

	// sorts the dir by filemane
        qsort(&filearray[0], fcount, sizeof(char *), cmpstringp);

	// Print out all files
        for (int j=0; j<fcount; j++)
        {
                print_file_details(filearray[j]);
        }

        closedir(dp);
	} // end if (argc == 1)

	// list the details of specified file mode
	else
	{
		for (int i=1 ; i < argc ; i++)
		{
			print_file_details(argv[i]);
       		}
    	} // end else

	exit(0);

} // end main
