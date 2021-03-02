#include "ext2.h"
extern read_ata_sector;

const int partitionStartSectorNo = 63

void loadKenelFromExt2() {
  char buf[1024]; 
  sGroupDescriptor  gd = getBlockGroupDescriptor(buf);
  gd.inodeTable; // inode table start block num
  sInode rootInode = getRootInode(gd.inodeTable);
  rootInode.blocks[0];
  rootInode.size;
  
}

sGroupDescriptor getBlockGroupDescriptor(char * buf) {
    char sectorNum = 2;
    read_ata_sector(2, buf);
}

sInode getRootInode(int blockNo, char * buf) {
    int sectorNo = blockNo*2 + partitionStartSectorNo;
    char sectorNum = 2;
    read_ata_sector(sectorNum, sectorNo, buf);
    return (sInode*)(buf + sizeof(sInode));
}

void loadKenelFileRaw(char * rawBuffer, unsigned int blocks[15]) {
      
}


