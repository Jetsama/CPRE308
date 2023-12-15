/* fat12ls.c 
* 
*  Displays the files in the root sector of an MSDOS floppy disk
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 32      /* size of the read buffer */
#define ROOTSIZE 256 /* max size of the root directory */
//define PRINT_HEX   // un-comment this to print the values in hex for debugging

struct BootSector
{
    unsigned char  sName[9];            // The name of the volume
    unsigned short iBytesSector;        // Bytes per Sector
    
    unsigned char  iSectorsCluster;     // Sectors per Cluster
    unsigned short iReservedSectors;    // Reserved sectors
    unsigned char  iNumberFATs;         // Number of FATs
    
    unsigned short iRootEntries;        // Number of Root Directory entries
    unsigned short iLogicalSectors;     // Number of logical sectors
    unsigned char  xMediumDescriptor;   // Medium descriptor
    
    unsigned short iSectorsFAT;         // Sectors per FAT
    unsigned short iSectorsTrack;       // Sectors per Track
    unsigned short iHeads;              // Number of heads
    
    unsigned short iHiddenSectors;      // Number of hidden sectors
};


// Prints the filename, time, date, attributes and size of each entry
//  Input: Calculated directory offset and number of directory entries
void parseDirectory(int iDirOff, int iEntries, unsigned char buffer[]);



// The characters are swapped (two, one) and returned as a short
// Input: Two initialized characters
unsigned short endianSwap(unsigned char one, unsigned char two);



// Fille out the the BootSector struct from the buffer data
//  Input: An initialized BootSector struct and a pointer to an array
//       of characters read from a BootSector
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[]);



//  Pre: String is initialized with at least 12 characters, buffer contains
//       the directory array, offset points to the location of the filename
// Post: fills and returns a string containing the filename in 8.3 format
char * toDOSName(char string[], unsigned char buffer[], int offset);



//  Pre: String is initialized with at least five characters, key contains
//       the byte containing the attribue from the directory buffer
// Post: fills the string with the attributes
char * parseAttributes(char string[], unsigned char key);



//  Pre: string is initialzied for at least 9 characters, usTime contains
//       the 16 bits used to store time
// Post: string contains the formatted time
char * parseTime(char string[], unsigned short usTime);



//  Pre: string is initialized for at least 13 characters, usDate contains
//       the 16 bits used to store the date
// Post: string contains the formatted date
char * parseDate(char string[], unsigned short usDate);




// reads the boot sector and root directory
int main(int argc, char * argv[])
{
    int pBootSector = 0;
    unsigned char buffer[SIZE];
    unsigned char rootBuffer[ROOTSIZE * 32];
    struct BootSector sector;
    int iRDOffset = 0;
    
    // Check for argument
    if (argc < 2) {
    	printf("Specify boot sector\n");
    	exit(1);
    }
    
    // Open the file and read the boot sector
    pBootSector = open(argv[1], O_RDONLY);
    read(pBootSector, buffer, SIZE);
    
    // Decode the boot Sector
    decodeBootSector(&sector, buffer);
    
    // Calculate the location of the root directory
    iRDOffset = (1 + (sector.iSectorsFAT * sector.iNumberFATs) )
                 * sector.iBytesSector;
                 
    printf("root dir offset: %d\n", iRDOffset);
	
    
    // Read the root directory into buffer
    lseek(pBootSector, iRDOffset, SEEK_SET);
    read(pBootSector, rootBuffer, ROOTSIZE);
    close(pBootSector);
    
    // Parse the root directory
    printf("max # root dir entries: %d\n", sector.iRootEntries);
    parseDirectory(iRDOffset, sector.iRootEntries, rootBuffer);
    
} // end main


// Converts two characters to an unsigned short with two, one
unsigned short endianSwap(unsigned char one, unsigned char two)
{
	unsigned short res = two;
	res *= 256;
	res += one;
    return res;
}


// TODO Fills out the BootSector Struct from the buffer
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[])
{
	
	// TODO: Pull the name and put it in the struct pBootS (remember to null-terminate)
	int buffer_counter = 3; //0x00 3 Binary offset of boot loader hex
	char buffer_char = buffer[buffer_counter];
	for(int i = 0; i <8; i++){ //0x03 8 Volume Label (ASCII, null padded) ASCII

		pBootS->sName[i] = buffer[buffer_counter];
		buffer_counter++;
	}
	pBootS->sName[8] = '\n';

	// TODO: Read bytes/sector and convert to big endian
    pBootS->iBytesSector = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]); //0x0B 2 Bytes per logical sector decimal
	buffer_counter +=2;
	

	// TODO: Read sectors/cluster, Reserved sectors and Number of Fats
	pBootS->iSectorsCluster = buffer[buffer_counter]; //0x0D 1 Logical sectors per cluster (block) decimal
	buffer_counter+=1;

	pBootS->iReservedSectors = endianSwap(buffer[buffer_counter],buffer[buffer_counter+1]);//0x0E 2 Number of reserved sectors decimal
	buffer_counter+=2;

	pBootS->iNumberFATs = buffer[buffer_counter];//0x10 1 Number of FATs (generally 2) decimal
	buffer_counter+=1;

	// TODO: Read root entries, logicical sectors and medium descriptor
    pBootS->iRootEntries = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]); //0x0E 2 Number of reserved sectors decimal
	buffer_counter+=2;
	
	pBootS->iLogicalSectors = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]);//0x13 2 Number of logical sectors decima
	buffer_counter+=2;
	pBootS->xMediumDescriptor = buffer[21]; //0x15 1 Media descriptor hex

	buffer_counter+=1;
	
	// TODO: Read and covert sectors/fat, sectors/track, and number of heads
    pBootS->iSectorsFAT = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]);//0x1A 2 Number of heads decimal
	buffer_counter+=2;
	pBootS->iSectorsTrack = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]);
	buffer_counter+=2;

	pBootS->iHeads = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]);
	buffer_counter+=2;

	// TODO: Read hidden sectors
	pBootS->iHiddenSectors = endianSwap(buffer[buffer_counter],buffer[buffer_counter +1]); //0x1C 2 Number of hidden sectors decimal
}


// TODO: iterates through the directory to display filename, time, date,
// attributes and size of each directory entry to the console
void parseDirectory(int iDirOff, int iEntries, unsigned char buffer[])
{
    int i = 0;
    char string[13];
    
    // Display table header with labels
    printf("Filename\tAttrib\tTime\t\tDate\t\tSize\n");
    
    // loop through directory entries to print information for each
	// Don't forget to NULL terminate the strings
    for(i = 0; i < (iEntries); i = i + 1)   {

    	if (buffer[i*32]==0x00)
    		break;
    		
    	if (buffer[i*32]==0xe5)
    		continue;
        int offset = i*32;
		//Display filename
        //0x00 8 Filename
        //0x08 3 Extension
		toDOSName(string, buffer, offset /*TODO: replace 0 with correct value */);
		printf("%-12s\t", string);
		
		//Display Attributes
        //0x0b 1 Attributes
		parseAttributes(string, buffer[offset + 11]/*TODO: replace 0 with correct value */);
		printf("%s\t", string);
		
		//Display Time
        //0x16 2 Last Access Time
		parseTime(string,  endianSwap(buffer[offset + 22], buffer[offset + 23])/*TODO: replace 0 with correct value */);
		printf("%s\t", string);
		
		//Display Date
        //0x18 2 Last Access Date
		parseDate(string, endianSwap(buffer[offset + 24], buffer[offset + 25])/*TODO: replace 0 with correct value */);
		printf("%s\t", string);
		
		//Display Size
        //0x1c 4 Size
		printf("%d\n", endianSwap(buffer[offset + 28], buffer[offset + 29]) /* TODO: replace 0 with actual size */);
    }
    
    // Display key
    printf("(R)ead Only (H)idden (S)ystem (A)rchive\n");
}


// Parses the attributes bits of a file
char * parseAttributes(char *string, unsigned char key)
{
 	int i=0;
 	if (key&1)
 		string[i++] = 'R';
 	if ((key>>1)&1)
 		string[i++] = 'H';
 	if ((key>>2)&1)
 		string[i++] = 'S';
 	if ((key>>5)&1)
 		string[i++] = 'A';
 	
 	string[i] = '\0';
    return string;
} // end parseAttributes()


// Decodes the bits assigned to the time of each file
char * parseTime(char *time_string, unsigned short time)
{
	unsigned short hour = (time>>11)&0x1f;
	unsigned short minute = (time>>5)&0x3f;
	unsigned short second = (time&0x1f)*2;
	
    sprintf(time_string, "%02d:%02d:%02d", hour, minute, second);
			
    return time_string;
    
    
} // end parseTime()


// Decodes the bits assigned to the date of each file
char * parseDate(char *date_string, unsigned short date)
{
	unsigned short year = (date>>9) + 1980;
	unsigned short month = (date>>5)&0xf;
	unsigned short day = date&0x1f;
	
    sprintf(date_string, "%04d/%02d/%02d", year, month, day);

    return date_string;
    
} // end parseDate()


// Formats a filename string as DOS (adds the dot to 8-dot-3)
char * toDOSName(char *string, unsigned char buffer[], int offset)
{
	int index = 0;
	int i;
	for (i=0; i<8; i++)
		string[index++] = buffer[offset+i];
	while (string[--index]==' ');
	
	string[++index] = '.';
	
    for (i=8; i<11; i++)
    	string[++index] = buffer[offset+i];
    string[++index] = '\0';
    return string;
} // end toDosNameRead-Only Bit

