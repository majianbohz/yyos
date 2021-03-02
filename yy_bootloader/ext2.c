MExt2.c

//#include "../include/MExt2.h"

static char m_fileName[255] = {0};

static sSuperBlock          s_sprBlk;
static sGroupDescriptor     s_grpDescrpt;
static int sGetSuperBlockInfo(sSuperBlock* pSuperBlock);
static int sGetGroupDescriptorInfo(sGroupDescriptor* pGrpDescrpt);

int InitMExt2(char* cpFileName)
{
    if(0 > MFileOpen(cpFileName)){
        printf("InitMExt2 error, MFileOpen failed\n");
        return MEXT2_INIT;
    }
    sGetSuperBlockInfo(&s_sprBlk);
    sGetGroupDescriptorInfo(&s_grpDescrpt);

    return MEXT2_OK;
}

// 未加错误判断
// 超级块，占一个块
static int sGetSuperBlockInfo(sSuperBlock* pSuperBlock)
{
    pSuperBlock->inodes = MFileRead4Byte(0x400);
    pSuperBlock->blocks = MFileRead4Byte(0x400 + 4);
    pSuperBlock->reservedBlocks = MFileRead4Byte(0x400 + 4*2);
    pSuperBlock->freeBlocks = MFileRead4Byte(0x400 + 4*3);
    pSuperBlock->freeInodes = MFileRead4Byte(0x400 + 4*4);
    pSuperBlock->firstBlock = MFileRead4Byte(0x400 + 4*5);
    pSuperBlock->blockSize  = 1024<<MFileRead4Byte(0x400 + 4*6);
    pSuperBlock->blocksPerGroup = MFileRead4Byte(0x420);
    pSuperBlock->framsPerGroup = MFileRead4Byte(0x420 + 4);
    pSuperBlock->inodesPerGroup = MFileRead4Byte(0x420 + 4*2);
    pSuperBlock->inodeSize = MFileRead2Byte(0x458);
    pSuperBlock->blockGroupIndex = MFileRead2Byte(0x458 + 2);

    return MEXT2_OK;
}

int GetSuperBlockInfo(sSuperBlock* pSuperBlock)
{
    *pSuperBlock = s_sprBlk;

    return MEXT2_OK;
}


// 未完美错误判断
// 块组
static int sGetGroupDescriptorInfo(sGroupDescriptor* pGrpDescrpt)
{
    uint32  start = 0;

    // 首先获取起点，即超级块结束的位置
    start = 0x400 + 1024;

    pGrpDescrpt->blockBmLoc = MFileRead4Byte(start);
    pGrpDescrpt->inodeBmLoc = MFileRead4Byte(start + 4);
    pGrpDescrpt->inodeTable = MFileRead4Byte(start + 4*2);
    pGrpDescrpt->freeBlocks = MFileRead2Byte(start + 4*3);
    pGrpDescrpt->freeInodes = MFileRead2Byte(start + 4*3 + 2);
    pGrpDescrpt->directories = MFileRead4Byte(start + 4*3 + 2*2);

    return MEXT2_OK;
}

int GetGroupDescriptorInfo(sGroupDescriptor* pGrpDescrpt)
{
    *pGrpDescrpt = s_grpDescrpt;

    return MEXT2_OK;
}


// 获取 inode
int sGetInodeInfo(off_t addr, sInode* pSInode)
{
    int i = 0;

    pSInode->st_mode = MFileRead2Byte(addr);
    pSInode->user    = MFileRead2Byte(addr + 2*1);
    pSInode->size    = MFileRead4Byte(addr + 2*2);
    pSInode->group   = MFileRead2Byte(addr + 24);
    pSInode->links   = MFileRead2Byte(addr + 24 + 2);
    pSInode->ocupiedBlocks = MFileRead4Byte(addr + 24 + 2*2 );
    for(i=0; i<15; i++){
        pSInode->blocks[i] = MFileRead4Byte(addr + 40 + 4*i);
    }

    return MEXT2_OK;
}
