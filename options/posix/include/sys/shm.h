#ifndef _SYS_SHM_H
#define _SYS_SHM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <abi-bits/pid_t.h>
#include <abi-bits/shm.h>
#include <bits/size_t.h>
#include <time.h>

#include <sys/ipc.h>

#define SHM_R 0400
#define SHM_W 0200

#define SHM_RDONLY 010000
#define SHM_RND 020000
#define SHM_REMAP 040000
#define SHM_EXEC 0100000

#define SHM_LOCK 11
#define SHM_UNLOCK 12
#define SHM_STAT 13
#define SHM_INFO 14
#define SHM_STAT_ANY 15
#define SHM_DEST 01000
#define SHM_LOCKED 02000
#define SHM_HUGETLB 04000
#define SHM_NORESERVE 010000

#define SHM_HUGE_SHIFT 26
#define SHM_HUGE_MASK 0x3f
#define SHM_HUGE_64KB (16 << 26)
#define SHM_HUGE_512KB (19 << 26)
#define SHM_HUGE_1MB (20 << 26)
#define SHM_HUGE_2MB (21 << 26)
#define SHM_HUGE_8MB (23 << 26)
#define SHM_HUGE_16MB (24 << 26)
#define SHM_HUGE_32MB (25 << 26)
#define SHM_HUGE_256MB (28 << 26)
#define SHM_HUGE_512MB (29 << 26)
#define SHM_HUGE_1GB (30 << 26)
#define SHM_HUGE_2GB (31 << 26)
#define SHM_HUGE_16GB (34U << 26)

typedef unsigned long shmatt_t;

struct shmid_ds {
	struct ipc_perm shm_perm;
	size_t shm_segsz;
	time_t shm_atime;
	time_t shm_dtime;
	time_t shm_ctime;
	pid_t shm_cpid;
	pid_t shm_lpid;
	unsigned long shm_nattch;
};

struct shminfo {
	unsigned long shmmax;
	unsigned long shmmin;
	unsigned long shmmni;
	unsigned long shmseg;
	unsigned long shmall;
	unsigned long __unused[4];
};

#ifndef __MLIBC_ABI_ONLY

void *shmat(int __shmid, const void *__shmaddr, int __shmflg);
int shmctl(int __shmid, int __cmd, struct shmid_ds *__buf);
int shmdt(const void *__shmaddr);
int shmget(key_t __key, size_t __size, int __shmflg);

#endif /* !__MLIBC_ABI_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _SYS_SHM_H */
