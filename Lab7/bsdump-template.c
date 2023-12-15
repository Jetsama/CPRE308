/* bsdump.c 
 * 
 * Reads the boot sector of an MSDOS floppy disk
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 32  /* size of the read buffer */
//define PRINT_HEX // un-comment this to print the values in hex for debugging

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

// Converts two characters to an unsigned short with (two, one)
// Input: Two unsigned characters
unsigned short endianSwap(unsigned char one, unsigned char two);


// Fills out the BootSector Struct using the buffer array
// Input: An initialized BootSector struct and a pointer to an array
//      of characters read from a BootSector
void decodeBootSector(struct BootSector * pBootS, unsigned char buffer[]);


// Displays the BootSector information to the console
// Input: A filled BootSector struct
void printBootSector(struct BootSector * pBootS);



// entry point:
int main(int argc, char * argv[])
{
	int pBootSector = 0;
	unsigned char buffer[SIZE];
	struct BootSector sector;
    
	// Check for argument
	if (argc < 2) {
		printf("Please specify the boot sector file!\n");
		exit(1);
	}
    
	// Open the file and read the boot sector
	pBootSector = open(argv[1], O_RDONLY);
	read(pBootSector, buffer, SIZE);
    close(pBootSector);
    
	// Decode the boot Sector
	decodeBootSector(&sector, buffer);
    
	// Print Boot Sector information
	printBootSector(&sector);
    
    return 0;
} 


// Converts two characters to an unsigned short with (two, one)
unsigned short endianSwap(unsigned char one, unsigned char two)
{
	unsigned short res = two;
	res *= 256;
	res += one;
    return res;
}


// TODO: Fills out the BootSector Struct from the buffer
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


// Displays the BootSector information to the console
void printBootSector(struct BootSector * pBootS)
{
#ifndef PRINT_HEX
	printf("                    Name:   %s\n", pBootS->sName);
	printf("            Bytes/Sector:   %i\n", pBootS->iBytesSector);
	printf("         Sectors/Cluster:   %i\n", pBootS->iSectorsCluster);
	printf("        Reserved Sectors:   %i\n", pBootS->iReservedSectors);
	printf("          Number of FATs:   %i\n", pBootS->iNumberFATs);
	printf("  Root Directory entries:   %i\n", pBootS->iRootEntries);
	printf("         Logical sectors:   %i\n", pBootS->iLogicalSectors);
	printf("       Medium descriptor:   0x%04x\n", pBootS->xMediumDescriptor);
	printf("             Sectors/FAT:   %i\n", pBootS->iSectorsFAT);
	printf("           Sectors/Track:   %i\n", pBootS->iSectorsTrack);
	printf("         Number of heads:   %i\n", pBootS->iHeads);
	printf("Number of Hidden Sectors:   %i\n", pBootS->iHiddenSectors);
#else
	printf("                    Name:   %s\n",     pBootS->sName);
	printf("            Bytes/Sector:   0x%04x\n", pBootS->iBytesSector);
	printf("         Sectors/Cluster:   0x%02x\n", pBootS->iSectorsCluster);
	printf("        Reserved Sectors:   0x%04x\n", pBootS->iReservedSectors);
	printf("          Number of FATs:   0x%02x\n", pBootS->iNumberFATs);
	printf("  Root Directory entries:   0x%04x\n", pBootS->iRootEntries);
	printf("         Logical sectors:   0x%04x\n", pBootS->iLogicalSectors);
	printf("       Medium descriptor:   0x%02x\n", pBootS->xMediumDescriptor);
	printf("             Sectors/FAT:   0x%04x\n", pBootS->iSectorsFAT);
	printf("           Sectors/Track:   0x%04x\n", pBootS->iSectorsTrack);
	printf("         Number of heads:   0x%04x\n", pBootS->iHeads);
	printf("Number of Hidden Sectors:   0x%04x\n", pBootS->iHiddenSectors);
#endif
	return;
}
