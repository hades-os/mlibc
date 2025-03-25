#ifndef HADES_SYSCALL_H
#define HADES_SYSCALL_H

#define SYS_openat 0
#define SYS_pipe 1
#define SYS_close 2
#define SYS_pread 3
#define SYS_read 4
#define SYS_pwrite 5
#define SYS_write 6
#define SYS_mkdirat 7
#define SYS_linkat 8
#define SYS_unlinkat 9
#define SYS_renameat 10
#define SYS_lseek 11
#define SYS_dup2 12
#define SYS_fcntl 13
#define SYS_statat 14
#define SYS_ioctl 15
#define SYS_accessat 16
#define SYS_poll 17
#define SYS_ppoll 18
#define SYS_mmap 19
#define SYS_munmap 20
#define SYS_mprotect 21
#define SYS_set_fs_base 22
#define SYS_set_gs_base 23
#define SYS_get_fs_base 24
#define SYS_get_gs_base 25
#define SYS_exit 26
#define SYS_fork 27
#define SYS_exec 28
#define SYS_futex 29
#define SYS_waitpid 30
#define SYS_readdir 31
#define SYS_getcwd 32
#define SYS_chdir 33
#define SYS_getpid 37
#define SYS_gettid 38
#define SYS_getppid 39
#define SYS_setpgid 40
#define SYS_getpgid 41
#define SYS_setsid 42
#define SYS_getsid 43
#define SYS_kill 44
#define SYS_pause 45
#define SYS_sigsuspend 46
#define SYS_sigreturn 47
#define SYS_sigenter 48
#define SYS_sigaction 49
#define SYS_sigpending 50
#define SYS_sigprocmask 51
#define SYS_sleep 52
#define SYS_clock_gettime 53
#define SYS_clock_get 54
#define SYS_user_log 55
#define SYS_getuid 56
#define SYS_setuid 57
#define SYS_geteuid 58
#define SYS_seteuid 59
#define SYS_getgid 60
#define SYS_setgid 61
#define SYS_getegid 62
#define SYS_setegid 63
#define SYS_sethostname 64
#define SYS_gethostname 65

extern "C" {
    using sc_word_t = long;
    
    static sc_word_t syscall0(int sc) {
        sc_word_t ret;
        asm volatile("syscall" : "=a"(ret) : "a"(sc) : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall1(int sc, sc_word_t arg1) {
        sc_word_t ret;
        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1)
                    : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall2(int sc, sc_word_t arg1, sc_word_t arg2) {
        sc_word_t ret;
        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1), "S"(arg2)
                    : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall3(int sc, sc_word_t arg1, sc_word_t arg2,
                            sc_word_t arg3) {
        sc_word_t ret;
        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1), "S"(arg2), "d"(arg3)
                    : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall4(int sc, sc_word_t arg1, sc_word_t arg2,
                            sc_word_t arg3, sc_word_t arg4) {
        sc_word_t ret;

        register sc_word_t arg4_reg asm("r10") = arg4;

        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1), "S"(arg2), "d"(arg3), "r"(arg4_reg)
                    : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall5(int sc, sc_word_t arg1, sc_word_t arg2,
                            sc_word_t arg3, sc_word_t arg4, sc_word_t arg5) {
        sc_word_t ret;

        register sc_word_t arg4_reg asm("r10") = arg4;
        register sc_word_t arg5_reg asm("r8") = arg5;

        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1), "S"(arg2), "d"(arg3), "r"(arg4_reg),
                    "r"(arg5_reg)
                    : "rcx", "r11", "memory");
        return ret;
    }

    static sc_word_t syscall6(int sc, sc_word_t arg1, sc_word_t arg2,
                            sc_word_t arg3, sc_word_t arg4, sc_word_t arg5,
                            sc_word_t arg6) {
        sc_word_t ret;

        register sc_word_t arg4_reg asm("r10") = arg4;
        register sc_word_t arg5_reg asm("r8") = arg5;
        register sc_word_t arg6_reg asm("r9") = arg6;

        asm volatile("syscall"
                    : "=a"(ret)
                    : "a"(sc), "D"(arg1), "S"(arg2), "d"(arg3), "r"(arg4_reg),
                    "r"(arg5_reg), "r"(arg6_reg)
                    : "rcx", "r11", "memory");
        return ret;
    }
}

__attribute__((__always_inline__)) inline sc_word_t sc_cast(long x) { return x; }
__attribute__((__always_inline__)) inline sc_word_t sc_cast(const void *x) {
    return reinterpret_cast<sc_word_t>(x);
}

__attribute__((__always_inline__)) static inline long _syscall(int call) {
    return syscall0(call);
}

__attribute__((__always_inline__)) static inline long _syscall(int call,
                                                           sc_word_t arg0) {
    return syscall1(call, arg0);
}

__attribute__((__always_inline__)) static inline long
_syscall(int call, sc_word_t arg0, sc_word_t arg1) {
    return syscall2(call, arg0, arg1);
}

__attribute__((__always_inline__)) static inline long
_syscall(int call, sc_word_t arg0, sc_word_t arg1, sc_word_t arg2) {
    return syscall3(call, arg0, arg1, arg2);
}

__attribute__((__always_inline__)) static inline long
_syscall(int call, sc_word_t arg0, sc_word_t arg1, sc_word_t arg2,
         sc_word_t arg3) {
    return syscall4(call, arg0, arg1, arg2, arg3);
}

__attribute__((__always_inline__)) static inline long
_syscall(int call, sc_word_t arg0, sc_word_t arg1, sc_word_t arg2,
         sc_word_t arg3, sc_word_t arg4) {
    return syscall5(call, arg0, arg1, arg2, arg3, arg4);
}

__attribute__((__always_inline__)) static inline long
_syscall(int call, sc_word_t arg0, sc_word_t arg1, sc_word_t arg2,
         sc_word_t arg3, sc_word_t arg4, sc_word_t arg5) {
    return syscall6(call, arg0, arg1, arg2, arg3, arg4, arg5);
}

template <typename... T>
__attribute__((__always_inline__)) static inline long syscall(sc_word_t call,
                                                          T... args) {
    return _syscall(call, sc_cast(args)...);
}

inline int sc_error(long ret) {
    if (ret < 0)
        return -ret;
    return 0;
}

#endif
