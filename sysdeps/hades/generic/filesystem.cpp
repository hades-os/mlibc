#include <fcntl.h>
#include <errno.h>
#include <bits/ensure.h>
#include <bits/winsize.h>
#include <mlibc/all-sysdeps.hpp>
#include <hades/syscall.h>
#include <mlibc/debug.hpp>
#include <stdlib.h>

namespace mlibc {
    int sys_openat(int dirfd, const char *path, int flags, mode_t mode, int *fd) {
        auto res = syscall(SYS_openat, dirfd, path, flags, mode);
        if (int err = sc_error(res); err) {
            return err;
        }

        *fd = res;
        return 0;
    }

    int sys_open(const char *pathname, int flags, mode_t mode, int *fd) {
        return sys_openat(AT_FDCWD, pathname, flags, mode, fd);
    }

    int sys_read(int fd, void *buf, size_t count, ssize_t *bytes_read) {
        auto res = syscall(SYS_read, fd, buf, count);
        if (int err = sc_error(res); err) {
            return err;
        }

        *bytes_read = res;
        return 0;
    }

    int sys_write(int fd, const void *buf, size_t count, ssize_t *bytes_written) {
        auto res = syscall(SYS_write, fd, buf, count);
        if (int err = sc_error(res); err) {
            return err;
        }

        *bytes_written = res;
        return 0;
    }

    int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
        auto res = syscall(SYS_seek, fd, offset, whence);
        if (int err = sc_error(res); err) {
            return err;
        }

        *new_offset = res;
        return 0;
    }

    int sys_close(int fd) {
        auto res = syscall(SYS_close, fd);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_stat(fsfd_target fsfdt, int fd, const char *path, int flags, struct stat *statbuf) {
        switch (fsfdt) {
            case fsfd_target::path:
                fd = AT_FDCWD;
                break;
            case fsfd_target::fd:
                flags |= AT_EMPTY_PATH;
                break;
            case fsfd_target::fd_path:
                break;
            default:
                __ensure(!"Invalid FD target");
                __builtin_unreachable();
        }

        auto res = syscall(SYS_lstatat, fd, path, statbuf, flags);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_isatty(int fd) {
        struct winsize wsz;
        auto res = syscall(SYS_ioctl, fd, TIOCGWINSZ, &wsz);
        if (int err = sc_error(res); err) {
            errno = ENOTTY;
            return 0;
        }

        return 1;
    }
}