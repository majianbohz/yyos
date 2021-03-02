
//#include "../include/MFile.h"

//  Bm      ->  Bitmap
//  Loc     ->  Location
//  dir     ->  directories


#define     MEXT2_OK                     0
#define     MEXT2_INIT                  -1
#define     MEXT2_INFO                  -2
#define     MEXT2_IDLEBITMAP            -3
#define     MEXT2_IDLEINODEBLOCK        -4


// 块组描述符表
typedef struct{
    unsigned int      blockBmLoc;
    unsigned int      inodeBmLoc;
    unsigned int      inodeTable;
    unsigned short      freeBlocks;
    unsigned short      freeInodes;
    unsigned short      directories;
} sGroupDescriptor;

// inode 表
typedef struct{
    unsigned short      st_mode;
    unsigned short      user;
    unsigned int      size;
    unsigned int      atime;
    unsigned int      ctime;
    unsigned int      mtime;
    unsigned int      dtime;
    unsigned short      group;
    unsigned short      links;
    unsigned int      ocupiedBlocks;
    unsigned int      flags;
    unsigned int      osInfo;
    unsigned int      blocks[15];
} sInode;

// 数据块
typedef struct{
    unsigned int      inodeIndex;
    unsigned short      recordLen;
    unsigned char       fileNameLen;
    unsigned char       fileType;
    unsigned char       fileName[256];   // malloc 根据 fileNameLen    
}sDataBlock;

// super block
typedef struct
{
    unsigned int      inodes;
    unsigned int      blocks;
    unsigned int      reservedBlocks;
    unsigned int      freeBlocks;
    unsigned int      freeInodes;
    unsigned int      firstBlock;
    unsigned int      blockSize;      // 1024<<blockSize = 实际块大小（Bytes）
    unsigned int      log;            // 给 0 就成
    unsigned int      blocksPerGroup;
    unsigned int      framsPerGroup;
    unsigned int      inodesPerGroup;
    unsigned int      lastMountTime;
    unsigned int      lastWriteTime;
    unsigned short      mountNum;
    unsigned short      maxMountNum;
    unsigned short      magic;
    unsigned short      fsState;
    unsigned short      errBehav;
    unsigned short      minorRev;
    unsigned int      lastCheck;
    unsigned int      checkIntvl;
    unsigned int      osType;
    unsigned int      majorRev;
    unsigned short      reserverdBlocksUid;
    unsigned short      reserverdBlocksGid;
    unsigned int      firstInode;
    unsigned short      inodeSize;
    unsigned short      blockGroupIndex;
    unsigned char       compatibleFeatures[12];
    unsigned char       uuid[16]; 
    unsigned char       volmName[16];
    // 余下的填充零
} sSuperBlock;

