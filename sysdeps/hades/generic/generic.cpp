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

    int sys_sleep(time_t *secs, long *nanos) {
        return syscall(SYS_sleep, secs, nanos);
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
        uintptr_t addr = syscall(SYS_mprotect, (uintptr_t) pointer, size, prot);
        if (int err = sc_error(addr); err) {
            return err;
        }

        return 0;
    }

    int sys_anon_allocate(size_t size, void **pointer) {
        return sys_vm_map(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0, pointer);
    }

    int sys_anon_free(void *pointer, size_t size) {
        return sys_vm_unmap(pointer, size);
    }

    int sys_getcwd(char *buffer, size_t size) {
        auto res = syscall(SYS_getcwd, buffer, size);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_chdir(const char *path) {
        auto res = syscall(SYS_chdir, path);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    pid_t sys_getpid() {
        return syscall(SYS_getpid);
    }

    pid_t sys_getppid() {
        return syscall(SYS_getppid);
    }

    uid_t sys_getuid() {
        return syscall(SYS_getuid);
    }

    int sys_setuid(uid_t uid) {
        auto res = syscall(SYS_setuid, uid);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    uid_t sys_geteuid() {
        return syscall(SYS_geteuid);
    }

    int sys_seteuid(uid_t euid) {
        auto res = syscall(SYS_seteuid, euid);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    gid_t sys_getgid() {
        return syscall(SYS_getgid);
    }

    int sys_setgid(gid_t gid) {
        auto res = syscall(SYS_setgid, gid);
         if (int err = sc_error(res); err) {
            return err;
        }

        return 0;       
    }

    gid_t sys_getegid() {
        return syscall(SYS_getegid);
    }

    int sys_setegid(gid_t egid) {
        auto res = syscall(SYS_setegid, egid);
        if (int err = sc_error(res); err) {
            return err;
        }

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

    int sys_gethostname(char *buffer, size_t bufsize) {
        auto res = syscall(SYS_gethostname, buffer, bufsize);
         if (int err = sc_error(res); err) {
            return err;
        }

        return 0;       
    }

    int sys_sethostname(const char *buffer, size_t bufsize) {
       auto res = syscall(SYS_sethostname, buffer, bufsize);
         if (int err = sc_error(res); err) {
            return err;
        }

        return 0;       

    }
}