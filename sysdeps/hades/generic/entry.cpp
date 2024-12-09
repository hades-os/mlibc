
#include <stdint.h>
#include <stdlib.h>
#include <bits/ensure.h>
#include <bits/posix/posix_signal.h>
#include <hades/syscall.h>
#include <mlibc/all-sysdeps.hpp>
#include <mlibc/elf/startup.h>

// defined by the POSIX library
void __mlibc_initLocale();

extern "C" uintptr_t *__dlapi_entrystack();
extern "C" void __dlapi_enter(uintptr_t *);

extern char **environ;
static mlibc::exec_stack_data __mlibc_stack_data;

struct LibraryGuard {
	LibraryGuard();
};

static LibraryGuard guard;

LibraryGuard::LibraryGuard() {
	__mlibc_initLocale();

	// Parse the exec() stack.
	mlibc::parse_exec_stack(__dlapi_entrystack(), &__mlibc_stack_data);
	mlibc::set_startup_data(__mlibc_stack_data.argc, __mlibc_stack_data.argv,
			__mlibc_stack_data.envp);
}

namespace mlibc {
    void sys_sigenter(void *sigenter) {
        syscall(SYS_sigenter, sigenter);
    }
    
    [[noreturn]] int sys_sigreturn(ucontext_t *) {
        syscall(SYS_sigreturn);
        __builtin_unreachable();
    }
}

static void __mlibc_sigentry(void *handler_rip, bool is_sigaction, int sig, siginfo_t *info, ucontext_t *ctx) {
    if ((uintptr_t) handler_rip == (uintptr_t) SIG_DFL) {
        mlibc::sys_exit(128 + sig);
    } else if ((uintptr_t) handler_rip == (uintptr_t) SIG_IGN) {

    } else {
        if (is_sigaction) {
            ((void (*)(int, siginfo_t *, void *)) handler_rip)(sig, info, ctx);
        } else {
            ((void (*)(int)) handler_rip)(sig);            
        }
    }

    mlibc::sys_sigreturn(ctx);
    __builtin_unreachable();
}

extern "C" void __mlibc_entry(uintptr_t *entry_stack, int (*main_fn)(int argc, char *argv[], char *env[])) {
    __dlapi_enter(entry_stack);

    mlibc::sys_sigenter((void *) __mlibc_sigentry);
	auto result = main_fn(__mlibc_stack_data.argc, __mlibc_stack_data.argv, environ);
	exit(result);
}

