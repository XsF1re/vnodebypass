#import "libdimentio.h"

#define PROC_P_LIST_LE_PREV_OFF (0x8)
#define PROC_P_LIST_LH_FIRST_OFF (0x0)

static kaddr_t kernproc;

//read kernel
uint32_t kernel_read32(uint64_t where) {
    uint32_t out;
    kread_buf(where, &out, sizeof(uint32_t));
    return out;
}

uint64_t kernel_read64(uint64_t where) {
    uint64_t out;
    kread_buf(where, &out, sizeof(uint64_t));
    return out;
}

//write kernel
void kernel_write32(uint64_t where, uint32_t what) {
    uint32_t _what = what;
    kwrite_buf(where, &_what, sizeof(uint32_t));
}

void kernel_write64(uint64_t where, uint64_t what) {
    uint64_t _what = what;
    kwrite_buf(where, &_what, sizeof(uint64_t));
}

kern_return_t
find_task(pid_t pid, kaddr_t *task) {
    pid_t cur_pid;
    kaddr_t proc;

    if(kread_addr(kernproc + PROC_P_LIST_LH_FIRST_OFF, &proc) == KERN_SUCCESS) {
        while(proc != 0 && kread_buf(proc + proc_p_pid_off, &cur_pid, sizeof(cur_pid)) == KERN_SUCCESS) {
            if(cur_pid == pid) {
                return kread_addr(proc + proc_task_off, task);
            }
            if(pid == 0 || kread_addr(proc + PROC_P_LIST_LE_PREV_OFF, &proc) != KERN_SUCCESS) {
                break;
            }
        }
    }
    return KERN_FAILURE;
}

kern_return_t
kread_addr(kaddr_t addr, kaddr_t *val) {
    return kread_buf(addr, val, sizeof(*val));
}
