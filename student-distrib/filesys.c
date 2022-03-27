#include "filesys.h"
#include "types.h"
#include "lib.h"

/*Checkpoint 3.2 directory functions*/
void initialize_filesys(uint32_t addr){
    bootBlock = (bootBlock_t *)addr;
}
int dir_open(const uint8_t* filename){

    return 0;
}

int dir_close(int32_t fd){
    
    return 0;
}

int dir_read(int32_t fd, void * buf, int32_t nbytes, int idx){
    int i, j, bytes_read;
    bytes_read = 0;
    // int length = (strlen(bootBlock->dentry_list[i].fileName) < nbytes) ? 
    //     strlen(bootBlock->dentry_list[i].fileName) : nbytes;
    //printf(" # of inodes lel: %d", bootBlock->numberOfInodes);
    //for (i = 0; i < 7; i++){
        int length = strlen(bootBlock->dentry_list[idx].fileName) + 1;
        if (length > FILE_NAME_LENGTH)
            length = FILE_NAME_LENGTH;
        for(j = 0; j < length; j++){
            // if (idx == 0){
            //     ((int8_t*)(buf))[bytes_read] = ' '; 
            //     ((int8_t*)(buf))[bytes_read] = bootBlock->dentry_list[idx].fileName[j+1];
            // }
            ((int8_t*)(buf))[bytes_read] = bootBlock->dentry_list[idx].fileName[j];
            bytes_read += 1;
//            printf("%c", ((int8_t*)(buf))[bytes_read]);
//            printf("%c \n");
        }
    //    printf("\n");
    //}
    //memcpy((uint8_t *)buf, bootBlock->dentry_list[i].fileName, length);
    return bytes_read;
}

int dir_write(int32_t fd, const void * buf, int32_t nbytes){
    
    return -1; //read-only file system, so return -1 automatically and uncondiionally
}

/*Checkpoint 3.2 file functions*/
int file_open(const uint8_t* filename, int fd){

    return 0;
}

int file_close(int32_t fd){
    return 0;

}

int file_read(int32_t fd, void * buf, int32_t nbytes){
    
    return 0;
}

int file_write(int32_t fd, const void * buf, int32_t nbytes){
    
    return -1; //read-only file system, so return -1 automatically and uncondiionally
}
int32_t read_dentry_name (const uint8_t* file_name, dentry_t * dentry){
    int i;
    if (file_name == NULL)
        return -1;
    //int dentry_fname_len = strlen(bootBlock->dentry_list[i].fileName);
    int arg_fname_len = strlen((int8_t *)file_name);
    if (arg_fname_len > FILE_NAME_LENGTH) //if it's too long...
        return -1;

    for(i = 0; i < bootBlock->numberOfDentries; i++){
        int dentry_fname_len = strlen(bootBlock->dentry_list[i].fileName);
        if (arg_fname_len == dentry_fname_len){
            if (strncmp(bootBlock->dentry_list[i].fileName, (int8_t *)file_name, dentry_fname_len) == 0){
                *dentry = bootBlock->dentry_list[i];
                //printf("inode #: %d \n", dentry->inode);
                return 0;
            }
        }
    }
    return -1;
}

int32_t read_dentry_index (uint32_t index, dentry_t* dentry){
    if (dentry == NULL)
        return -1; 
    printf(" # of inodes: %d \n", bootBlock->numberOfInodes);
    if (index < 0 || index >= bootBlock->numberOfInodes)
        return -1;
    
    if (index == bootBlock->dentry_list[index].inode) //this necessary? 
        *dentry = bootBlock->dentry_list[index];
    return 0;
}

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    int i, count;
    count = 0;
    if (buf == NULL)
        return -1;
    inode_initial_ptr = (inode_t *)bootBlock + 1;
    inode_t * inode_ptr = (inode_t *)(inode_initial_ptr + inode); //declare + initialize inode_ptr
    
    uint32_t data_block_idx = inode_ptr->data_block[offset/KB_4]; //calculate data block integer  
    uint32_t data_block_offset = offset % KB_4; //calculate offset WITHINT data block now

    data_block_initial_ptr = (uint8_t *)(bootBlock + 1 + bootBlock->numberOfInodes); 
    uint8_t * data_block_ptr = data_block_initial_ptr + data_block_idx; 

    for (i = 0; i < length; i++){
        ((uint8_t*)(buf))[i] = *(data_block_ptr + data_block_idx * KB_4 + data_block_offset + i); //how do I read by bytes once I have my data_block_ptr?
        count += 1;
    }
    return count; //return # of bytes read AKA # of bytes placed in buffer
}



