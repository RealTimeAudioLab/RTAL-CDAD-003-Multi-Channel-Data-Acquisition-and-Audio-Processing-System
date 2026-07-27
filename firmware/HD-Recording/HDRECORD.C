#include <math.h>
#include <graph.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <dos.h>

/* Function Declarations */

    long   decin32(void);
    float  floatin(void);
    void   filenamin(void);

    void   main(void), record(void), playback(void), test_disk(void);
    void   remove_file(void), quit(void);

    short  disable_verify(void), restore_verify(char);
    short  get_load_value(double);

/* Variable Declarations */

    short           rec;
    int             addr, load, done, ok;
    unsigned        old_drive, drive, number_of_drives;
    unsigned        load_value, no_of_blocks, status;
    unsigned        no_of_errors;
    double          disk_space, disk_cap;
    double          max_time, rec_time, play_time;
    unsigned long   s_frequency;
    struct diskfree_t d_free;
    char            drive_letter, fname[80], intext[80];
    int             c, file_handle;


void main()
{
    _dos_getdrive(&old_drive);                   /* 1=A, 2=B, etc          */
    drive = old_drive;                           /* get old default drive  */
    drive_letter = 0;
    printf("\nPlease enter the new default disk drive (C:, D:) : ");
    scanf("%s", intext);
    drive_letter = intext[0]; /* Get first character of intext (drive letter) */
			      /* i.e. character of drive c, C, d, D, etc   */
    drive_letter = drive_letter & 0x0DF;         /* make letter upper case */
    drive = drive_letter - 64;                   /* convert to drive no.   */
    if (drive > 15)
    {                                            /* error trapping         */
	drive = old_drive;
	drive_letter = drive + 64;
    }
    _dos_setdrive(drive, &number_of_drives);
    while (1 == 1)
    {                                            /* Loop forever           */
	_dos_getdiskfree(drive, &d_free);
	disk_cap   = 1.0 * d_free.sectors_per_cluster;
	disk_cap   = disk_cap * d_free.total_clusters;
	disk_cap   = disk_cap * d_free.bytes_per_sector;
	disk_space = 1.0 * d_free.avail_clusters;
	disk_space = disk_space * d_free.sectors_per_cluster;
	disk_space = disk_space * d_free.bytes_per_sector;
	_setvideomode(_DEFAULTMODE);

	printf("                           12 BIT AD/DA BOARD\n");
	printf("                         ~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("                             (c) 1992 A.H.\n");
	printf("\n");
	printf("          Demonstration Program for a Hard Disk based Recording\n");
	printf("          Recording by A/D & Playback by D/A\n");
	printf("\n");
	printf("          Current Drive =  %c:\n", drive_letter);
	printf("          Free Space    = %9.0f bytes\n", disk_space);
	printf("          Capacity      = %9.0f bytes\n", disk_cap);
	printf("\n");
	printf("          Record/Playback Menu\n");
	printf("\n");
	printf("               1)   Record\n");
	printf("               2)   Playback\n");
	printf("               3)   Test Hard Disk for Speed\n");
	printf("               4)   Delete File\n");
	printf("               5)   Quit\n");
	printf("\n");
	printf("          Enter Choice : ");
	done = 0;
	while (done == 0)
	{
	    c = getch();
	    switch(c) {
	    case '1':                  /* RECORD */
		printf("1\n");
		record();
		done = 1;
		break;
	    case '2':                  /* PLAYBACK */
		printf("2\n");
		playback();
		done = 1;
		break;
	    case '3':                  /* Test Hard Disk for Speed */
		printf("3\n");
		test_disk();
		done = 1;
		break;
	    case '4':                  /* Delete File in new default drive */
		printf("4\n");
		remove_file();
		done = 1;
		break;
	    case 'q':
	    case 'Q':
	    case 27:                   /* ESCAPE */
	    case '5':                  /* QUIT TO DOS */
		printf("5\n");
		quit();
		break;
	    default: /* do nothing */
		break;
	    }  /* End of SWITCH */
	}
    }          /* Loop Forever  */
}              /* End of MAIN PROGRAM */


void record()
{                                      /* START RECORDING Low-Level Calls  */
    char        old_verify;
    ok=1;
    status = 0;                                  /* Record-to-Disk         */
    _dos_setdrive(old_drive, &number_of_drives); /* Restore old drive      */
    _dos_setdrive(drive, &number_of_drives);     /* Get new drive          */
    if (ok == 1 )
    {
	old_verify = disable_verify();
	printf("\nPlease enter the sampling frequency (152 < 48001 Hz) : ");
	s_frequency = decin32();

	if (s_frequency < 153)
	    s_frequency = 153;
	else if (s_frequency > 48000)
	    s_frequency = 48000;

	printf("Recording sampling frequency     = %6lu Hz\n", s_frequency);
	load_value = get_load_value(1.0 * s_frequency);
	printf("Status                           = %6u \n", status);
	printf("Counter load value               = %6u \n", load_value);
	max_time = disk_space / (4.0 * s_frequency);
				       /* 4 bytes / 16-bit stereo sample   */
	printf("Please enter the recording time ( < %8.1f s.) : ", max_time);
	rec_time = fabs(floatin());

	if (rec_time >= max_time)
	    rec_time = max_time - 0.5;
	no_of_blocks = (rec_time*4*s_frequency)/49152;
				       /* No. of 49152 byte data blocks    */
	printf("Number of 49152 byte data blocks = %u\n", no_of_blocks);

	printf("Please enter the Record-to-Disk filename : ");
	filenamin();                   /* Get filename */
	file_handle = open(fname, O_RDWR|O_BINARY|O_CREAT, S_IREAD|S_IWRITE);
	if (file_handle == -1 )
	{
	    printf("Error Opening File\n");
	    quit;
	} else
	{
	    printf("\nInitializing file, please wait .....\n\n");
	    junk();                              /* initialise file        */
	    lseek(file_handle, 0L, SEEK_SET);    /* Reset file-pointer     */

	    printf("\nPress any key to begin RECORDING.\n\n");
	    while (!kbhit())                     /* loop                   */
		;
	    getch();                             /* flush i/p buffer       */
	    printf("Recording to Hard Disk .... ");

	    /*record_to_disk();*/                    /* External 286 Procs     */

	    printf("Done\n");
	}
	close(file_handle);
    }
    restore_verify(old_verify);
    printf("Press any key to continue");
    while (!kbhit())                             /* loop for key press     */
	;
}  /* End of Record-to-Disk */


void playback()                        /* PLAYBACK from Hard Disk          */
{                                      /* Start READing Low-Level calls    */
    extern playback_from_disk(void);
    unsigned long   filelen;
    ok=1;
    _dos_setdrive(old_drive, &number_of_drives);      /* Restore old drive */
    _dos_setdrive(drive, &number_of_drives);     /* Get new drive          */
    if (ok == 1 )
    {
	printf("Please enter the Playback-from-Disk filename : ");
	filenamin();                   /* Get filename */
	file_handle = open(fname, O_RDWR|O_BINARY, S_IREAD|S_IWRITE);
	if (file_handle == -1 )
	{
	    printf("Error Opening File\n");
	} else
	{
	    lseek(file_handle, 0L, SEEK_END);        /* move f-p to file end   */
	    filelen = tell(file_handle);             /* file length in bytes   */
	    printf("File Size                     = %ld bytes\n", filelen);
	    printf("\nPlease enter the Playback sampling frequency (152 < 48001 Hz) : ");
	    s_frequency = decin32();

	    if (s_frequency < 153)
		s_frequency = 153;
	    else if (s_frequency > 48000)
		s_frequency = 48000;

	    printf("Playback sampling frequency   = %6lu Hz\n", s_frequency);
	    load_value = get_load_value(s_frequency);
	    printf("Counter load value            = %6u \n", load_value);

	    no_of_blocks = filelen / 49152L;     /* get no_of_blocks       */
						 /* ! truncates file size  */
	    play_time = (49152.0 * no_of_blocks) / (4.0 * s_frequency);

	    printf("No. of 49152 byte data blocks = %6u\n", no_of_blocks);
	    printf("Playback time                 = %8.1f s\n\n", play_time);

	    lseek(file_handle, 0L, SEEK_SET);    /* Reset file-pointer     */
	    printf("\nPress any key to begin PLAYBACK.\n\n");
	    while (!kbhit())                     /* loop for key press     */
		;
	    getch();                             /* flush i/p buffer       */
	    printf("Playing from Hard Disk .... ");

	    /*playback_from_disk();*/                /* External 286 Procs     */

	    printf("Done\n");
	}
	close(file_handle);
    }
    printf("Press any key to continue");
    while (!kbhit()) /* loop */
	;
}        /* End of Playback-from-Disk */


void test_disk() /* this function checks the Hard Disk for the maximum
		    sampling frequency, by writing and reading to a test
		    file "TEST.FIL". The file is full of 16-bit sequential
		    integers generated by a DSP32C program. The numbers are
		    generated by a counter incremented on each interrupt */

{   /* START RECORDING Low-Level Calls */
    extern record_to_disk(void), junk(void), test_disk_check(void);
    char        old_verify;
    float       test_size;

    status = 0;
    printf("\nPlease enter the size of the test file in Mbytes : ");
    test_size = floatin();
    no_of_blocks = (1000000 * fabs(test_size))/49152;
printf("\n%f",disk_space);
printf("\n%d",no_of_blocks*49152);
				       /* No. of 49152 byte blocks         */
    if ((no_of_blocks * 49152) > disk_space)
    {                                  /* i.e. no room !                   */
	printf("\nNot enough disk space to run Hard Disk test !\n\n");
	printf("Press any key to return to main menu.\n");
	while (!kbhit())               /* loop for key press               */
	    ;
    } else
    {
	old_verify = disable_verify();
	printf("\nPlease enter the sampling frequency (152 < 150000 Hz) : ");
	s_frequency = decin32();

	if (s_frequency < 153)
	    s_frequency = 153;
	else if (s_frequency > 150000)
	    s_frequency = 150000;

	printf("Recording sampling frequency     = %6lu Hz\n", s_frequency);
	load_value = get_load_value(s_frequency);
	printf("Counter load value               = %6u \n", load_value);
	printf("Number of 49152 byte data blocks = %6u\n", no_of_blocks);

	_dos_setdrive(old_drive, &number_of_drives);  /* Restore old drive */
	ok = 1;
	_dos_setdrive(drive, &number_of_drives);      /* Get new drive     */
	if (ok == 1)
	{
	    file_handle = open("TEST.FIL", O_RDWR|O_BINARY|O_CREAT|O_TRUNC, S_IREAD|S_IWRITE);
	    if (file_handle == -1 )
	    {
		printf("Error Opening Test File\n");
		quit;
	    } else
	    {
		printf("\nInitializing \"TEST.FIL\", please wait ....\n\n");
		junk();                          /* Initialising file      */
		lseek(file_handle, 0L, SEEK_SET); /* Reset file-pointer    */

		printf("Recording test pattern to \"TEST.FIL\" from DSP32C .... ");

		record_to_disk();                /* External 286 Procs     */

		printf("Done\n");
		printf("Checking \"TEST.FIL\" for sequential integers .... ");
		lseek(file_handle, 0L, SEEK_SET); /* Reset file-pointer    */
		no_of_errors = 0;
		test_disk_check();               /* Check TEST.FIL for errors */
		printf("Done.\nNumber of Errors = %u\n", no_of_errors);

		close(file_handle);
		printf("Deleting \"TEST.FIL\" ....\n");
		remove("TEST.FIL");              /* Delete Test File       */
	    }
	}       /* end of ok == 1 */
	restore_verify(old_verify);
	printf("\nPress any key to return to main menu.");
	while (!kbhit()) /* loop */
	    ;
    } /* of disk space test */
}  /* End of Test_Disk */


void remove_file()                     /* Delete File */
{
    int  test;

    printf("\nPlease enter the filename to delete : ");
    filenamin();                   /* Get filename */

    strupr(fname);                               /* Convert to upper case  */
    printf("Deleting : %s  ....\n\n", fname);
    test = remove(fname);
    if (test == 0)
	printf("%s successfully deleted.", fname);
    else
	printf("Unable to delete %s !", fname);
    printf("\n\nPress any key to return to main menu.");
    while (!kbhit())  /* loop */
	;
}


void quit()                  /* This function returns the User to DOS */
{
    _dos_setdrive(old_drive, &number_of_drives); /* Restore old drive */
    _setvideomode(_DEFAULTMODE);
    exit(0);
}


short disable_verify()
{
    char    old_veri;

    union REGS regs;
    regs.h.ah = 0x54;
    int86(0x21, &regs, &regs);
    old_veri  = regs.h.al;
    regs.h.ah = 0x2E;                  /* set Verify to off */
    regs.h.al = 0;
    int86(0x21, &regs, &regs);
    return(old_veri);
}


short restore_verify(old_val)
    char    old_val;
{
    union REGS regs;
    regs.h.ah = 0x2E;
    regs.h.al = old_val;
    int86(0x21, &regs, &regs);
    return(old_val);
}


short get_load_value(freq)   /* Calc. Counter load value, for s_frequency  */
    double freq;             /* Sample Timing fÅr den DSP FE70 fÅr 25KHz   */
{
    unsigned load_value;
    load_value = (65536 - (10E+6 / freq));
    return(load_value);      /* Werte zwischen FFBF und F9C0 fÅr DSP */
}


float floatin()     /* 32-bit Float input with some error checking */
{
    float  val;
    double num;
    short  i;
    char   c, dec[21];

    dec[0] ='\0';
    c = 0;
    i = 0;
    val = 0;
    num = 0;

    do
    {
	c = getch();
	if ((isdigit(c) != 0) && (i <= 19)) /* Test for Float and 20 digits */
	{
	    cprintf("%c", c);
	    dec[i] = c;
	    i++;
	}
	if (((c == 101) || (c == 69)) && (i <= 19)) /* Test for "E" or "e" */
	{
	    cprintf("%c", c);
	    dec[i] = c;
	    i++;
	}
	if ((c == 8) && (i != 0))           /* Backspace test */
	{
	    cprintf("%c %c", c, c);
	    i--;
	}
	if ((i == 0 ) && ((c == 43) || (c == 45)))
	{                              /* "+" and "-" test on 1st char */
	    cprintf("%c", c);
	    dec[i] = c;
	    i++;
	}
    }   while (c != 13);
    printf("\n");
    dec[i] = '\0';
    num = atof(dec);
    val = num;
    return(val);
}


long  decin32()     /* 32-bit decimal input with some error checking */
{
    long   val;
    short  i, j;
    char   c, dec[20];

    dec[0] ='\0';
    c = 0;
    i = 0;
    j = 0;
    val = 0;

    do
    {
	c = getch();
	if ((isdigit(c) != 0) && (i <= 9)) /* Test for Dec. and 10 digits */
	{
	    cprintf("%c", c);
	    dec[j] = c;
	    i++;
	    j++;
	}
	if ((c == 8) && (j != 0))           /* Backspace test */
	{
	    cprintf("%c %c", c, c);
	    i--;
	    j--;
	    if (j == 0)   i = 0;
	}
	if ((j == 0 ) && ((c == 43) || (c == 45)))
	{                              /* "+" and "-" test on 1st char */
	    cprintf("%c", c);
	    dec[j] = c;
	    j++;
	}
    }   while (c != 13);
    printf("\n");
    dec[j] = '\0';
    val = atol(dec);
    return(val);
}


void filenamin()     /* Filename string input with error checking */
{
    short  i;
    char    c, buff[40];

    buff[0] ='\0';
    c = 0;
    i = 0;

    do
    {
	c = getch();
	if ((c != 8) && (i < 36))                     /* Test for 35 chars */
	{
	    c = toupper(c);
	    cprintf("%c", c);
	    buff[i] = c;
	    i++;
	}
	if ((c == 8) && (i != 0))           /* Backspace test */
	{
	    cprintf("%c %c", c, c);
	    i--;
	}
    }   while (c != 13);
    printf("\n");
    buff[i] = '\32';
    sscanf(buff, "%s", fname);
}


