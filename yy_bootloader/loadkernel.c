#include "ext2.h"
extern void read_ata_sector(int sectorNum, int sectorNo, void* buf);
extern void* pRawKernelBuffer;

const int partitionStartSectorNo = 63;

sGroupDescriptor* getBlockGroupDescriptor(int blockNo, void* buf) {
    int sectorNum = 2;
    int sectorNo = blockNo*2 + partitionStartSectorNo;
    read_ata_sector(sectorNum, sectorNo, buf);
    return (sGroupDescriptor*)buf;
}

sInode* getRootInode(int blockNo, void* buf) {
    int sectorNo = blockNo*2 + partitionStartSectorNo;
    int sectorNum = 2;
    read_ata_sector(sectorNum, sectorNo, buf);
    return (sInode*)(buf + sizeof(sInode));
}

sInode* getKernelInodeFromRootDir(sInode* pRootInode) {
    return 0;
}

void loadKernelFileRaw(char * rawKernelBuffer, sInode* pRootInode) {
    int fileBlockNum = pRootInode->size/1024;
    for (int i=0; i<fileBlockNum; i++) {
        int sectorNo = pRootInode->blocks[i]*2 + partitionStartSectorNo;
        char sectorNum = 2;
        read_ata_sector(sectorNum, sectorNo, rawKernelBuffer + i*1024);    
    } 
}

void loadKernelFromExt2() {
    char buf[1024]; 
    sGroupDescriptor*  pGD = getBlockGroupDescriptor(2, buf);
    //gd.inodeTable; // inode table start block num
    sInode* pRootInode = getRootInode(pGD->inodeTable, buf);
    //rootInode.blocks[0];
    //rootInode.size;  
    sInode* pKernelInode = getKernelInodeFromRootDir(pRootInode);
    
    loadKernelFileRaw(pRawKernelBuffer, pKernelInode);
}
