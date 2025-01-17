#include <bits/ensure.h>
#include <mlibc/all-sysdeps.hpp>
#include <hades/syscall.h>
#include <sys/types.h>

extern "C" void __mlibc_signal_restore();
namespace mlibc {
    int sys_sigaction(int sig, const struct sigaction *__restrict action, struct sigaction *__restrict old_action) {
        struct sigaction kernel_act;
        struct sigaction kernel_old;

        if (action) {
            kernel_act.sa_handler = action->sa_handler;
            kernel_act.sa_mask = action->sa_mask;
            kernel_act.sa_flags = action->sa_flags;
            kernel_act.sa_restorer = __mlibc_signal_restore;
        }

        auto res = syscall(SYS_sigaction, sig, &kernel_act, &kernel_old);
        if (int err = sc_error(res); err) {
            return err;
        }

        if (old_action) {
            old_action->sa_handler = kernel_old.sa_handler;
            old_action->sa_mask = kernel_old.sa_mask;
            old_action->sa_flags = kernel_old.sa_flags;
            old_action->sa_restorer = kernel_old.sa_restorer;
        }

        return 0;
    }

    int sys_sigpending(sigset_t *set) {
        auto res = syscall(SYS_sigpending, set);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_sigprocmask(int how, const sigset_t *__restrict set, sigset_t *__restrict retrieve) {
        auto res = syscall(SYS_sigprocmask, how, set, retrieve);
        return sc_error(res);
    }

    int sys_sigsuspend(const sigset_t *set) {
        auto res = syscall(SYS_sigsuspend, set);
        return sc_error(res);
    }

    int sys_kill(int pid, int sig) {
        if (sig == 0) return 0;

        auto res = syscall(SYS_kill, pid, sig);
        return sc_error(res);
    }
}