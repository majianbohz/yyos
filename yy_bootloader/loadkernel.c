#include "ext2.h"
extern void read_ata_sector(int sectorNum, int sectorNo, void* buf);
extern void* pRawKernelBuffer;

const int partitionStartSectorNo = 63;

int compareString(unsigned char* str1, unsigned char* str2) {
     int true = 1;
     int false = 0;     
     for (int i=0; true; i++) {
         if ( str1[i] != str2[i]) 
             return false;

	 if ( 0 == str1[i])
             return true;
     }
     return false;
}

void readExt2Block(int blockNo, void* buf) {
    int sectorNum = 2;
    int sectorNo = blockNo*2 + partitionStartSectorNo;
    read_ata_sector(sectorNum, sectorNo, buf);
    return;
}

sInode* getInodeByIndex(int inodeTableStartBlockNo, int inodeIndex, void* buf) {
    int blockNo = inodeTableStartBlockNo + (inodeIndex-1)/8;

    int sectorNo = blockNo*2 + partitionStartSectorNo;
    int sectorNum = 2;
    read_ata_sector(sectorNum, sectorNo, buf);
    return (sInode*)(buf + sizeof(sInode)*((inodeIndex-1)%8));
}

unsigned int getKernelInodeIndexFromRootDir(sInode* pRootInode) {
    int rootDirContentSize = pRootInode->size;
    char buf[1024];
    int sectorNum = 2;
    int sectorNo = pRootInode->blocks[0]*2 + partitionStartSectorNo;
    read_ata_sector(sectorNum, sectorNo, buf);
    char* buf2 = buf;
    sDirectoryEntry* pDirEntry = (sDirectoryEntry*)buf2;
    while (0 == compareString(pDirEntry->fileName, "kernel")) {
	buf2 += pDirEntry->entryLen;
        pDirEntry = (sDirectoryEntry*)buf2;
        if ((buf2 - buf) > 1000)
	    return 0;	
    }
    return pDirEntry->inodeIndex;
}

void loadKernelFileRaw(void * rawKernelBuffer, sInode* pKernelInode) {
    char indirectBuf[1024];	
    if (0 == pKernelInode->size)
        return;

    int fileBlockNum = pKernelInode->size/1024;
    if ((pKernelInode->size%1024) > 0)
        fileBlockNum++;

    if (fileBlockNum > 12)
        readExt2Block(pKernelInode->blocks[12], indirectBuf);

    for (int i=0; i<fileBlockNum; i++) {
       
       if ( i< 12 ) {    
         readExt2Block(pKernelInode->blocks[i], rawKernelBuffer + i*1024);
       }
       else if (i < (12 + 256)) {
	 int blockNo = *(int *)(indirectBuf + (i - 12)*4);
         readExt2Block(blockNo, rawKernelBuffer + i*1024); 
       }

    } 
}

void loadKernelFromExt2() {
    char buf[1024]; 
    int inodeTableStartBlockNo = 0;
    readExt2Block(2, buf);
    sGroupDescriptor*  pGD = (sGroupDescriptor*)buf;
    inodeTableStartBlockNo = pGD->inodeTable; // inode table start block num
    sInode* pRootInode = getInodeByIndex(inodeTableStartBlockNo, 2, buf); // root dir inode index 2 !!!
    //rootInode.blocks[0];
    //rootInode.size;  
    unsigned int kernelInodeIndex = getKernelInodeIndexFromRootDir(pRootInode);
    sInode* pKernelInode = getInodeByIndex(inodeTableStartBlockNo, kernelInodeIndex, buf);

    loadKernelFileRaw(pRawKernelBuffer, pKernelInode);
}

