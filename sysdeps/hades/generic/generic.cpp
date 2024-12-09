#include <fcntl.h>
#include <stdint.h>
#include <bits/ensure.h>
#include <mlibc/all-sysdeps.hpp>
#include <hades/syscall.h>
#include <mlibc/debug.hpp>
#include <stdint.h>
#include <stdlib.h>

namespace mlibc {
    void sys_libc_log(const char *message) {
        syscall(SYS_user_log, message);
    }

    void sys_libc_panic() {
        mlibc::infoLogger() << "PANIC!" << frg::endlog;
        __builtin_unreachable();
    }

    int sys_tcb_set(void *pointer) {
        syscall(SYS_set_fs_base, pointer);
        return 0;
    }

    void sys_exit(int status) {
        syscall(SYS_exit, status);
        __builtin_unreachable();
    }

    int sys_fork(pid_t *child) {
        uintptr_t res = syscall(SYS_fork);
        if (int err = sc_error(res); err) {
            return err;
        }

        *child = res;
        return 0;
    }

    int sys_execve(const char *path, char *const *argv, char *const *envp) {
        uintptr_t res = syscall(SYS_exec, path, argv, envp);
        if (int err = sc_error(res); err) {
            return err;
        }

        __builtin_unreachable();
    }

    int sys_futex_wait(int *pointer, int expected, const struct timespec *time) {
        int res = syscall(SYS_futex, pointer, 0, expected, time);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_futex_wake(int *pointer) {
        int res = syscall(SYS_futex, pointer, 1, 0, NULL);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }
    
    int sys_waitpid(pid_t pid, int *status, int flags, struct rusage *ru, pid_t *ret_pid) {
        (void) ru;

        uintptr_t res = syscall(SYS_waitpid, pid, status, flags);
        if (int err = sc_error(res); err) {
            return err;
        }

        *ret_pid = res;
        return 0;
    }

    int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window) {
        uintptr_t addr = syscall(SYS_mmap, (uintptr_t) hint, size, prot, flags, fd, offset);
        if (int err = sc_error(addr); err) {
            return err;
        }

        *window = (void *) addr;
        return 0;
    }

    int sys_vm_unmap(void *pointer, size_t size) {
        uintptr_t addr = syscall(SYS_munmap, (uintptr_t) pointer, size);
        if (int err = sc_error(addr); err) {
            return err;
        }

        return 0;
    }

    int sys_vm_protect(void *pointer, size_t size, int prot) {
	    mlibc::infoLogger() << "mlibc: sys_vm_protect(" << pointer << ", "
			    << size << ", " << prot << "); stub!\n"
			    << frg::endlog;
        return 0;
    }

    int sys_anon_allocate(size_t size, void **pointer) {
        return sys_vm_map(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0, pointer);
    }

    int sys_anon_free(void *pointer, size_t size) {
        return sys_vm_unmap(pointer, size);
    }

    pid_t sys_getpid() {
        return syscall(SYS_getpid);
    }

    pid_t sys_getppid() {
        return syscall(SYS_getppid);
    }

    uid_t sys_getuid() {
        return 0;
    }

    uid_t sys_geteuid() {
        return 0;
    }

    gid_t sys_getgid() {
        return 0;
    }

    int sys_setgid(gid_t gid) {
        return 0;
    }

    int sys_getsid(pid_t pid, pid_t *sid) {
        auto res = syscall(SYS_getsid, pid);
        if (int err = sc_error(res); err) {
            return err;
        }

        *sid = (pid_t) res;
        return 0;
    }

    gid_t sys_getegid() {
        mlibc::infoLogger() << "mlibc: " << __func__ << " is a stub!\n"
                    << frg::endlog;
        return 0;
    }

    int sys_getpgid(pid_t pid, pid_t *pgid) {
        auto res = syscall(SYS_getpgid, pid);
        if (int err = sc_error(res); err) {
            return err;
        }

        *pgid = (pid_t) res;
        return 0;
    }

    int sys_setpgid(pid_t pid, pid_t pgid) {
        auto res = syscall(SYS_setpgid, pid, pgid);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_setsid(pid_t *sid) {
        auto res = syscall(SYS_setsid);
        if (int err =sc_error(res); err) {
            return err;
        }

        *sid = (pid_t) res;
        return 0;
    }

    int sys_clock_get(int clock, time_t *secs, long *nanos) {
        auto res = syscall(SYS_clock_get, clock);
        *secs = res / 1000000000;
        *nanos = res % 1000000000;
        return 0;
    }
}