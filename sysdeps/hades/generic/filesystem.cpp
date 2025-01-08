#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <bits/ensure.h>
#include <bits/winsize.h>
#include <mlibc/all-sysdeps.hpp>
#include <hades/syscall.h>
#include <mlibc/debug.hpp>
#include <stdlib.h>

#include <sys/ioctls.h>

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

    int sys_faccessat(int dirfd, const char *pathname, int mode, int flags) {
        auto res = syscall(SYS_accessat, dirfd, pathname, flags, mode);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_access(const char *path, int mode) {
        return sys_faccessat(AT_FDCWD, path, mode, 0);
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

/*
    int fd_number = r->rdi;
    size_t req = r->rsi;
    void *arg = (void *) r->rdx;

*/

    int sys_ioctl(int fd, unsigned long request, void *arg, int *result) {
        auto res = syscall(SYS_ioctl, fd, request, arg);
        if (int err = sc_error(res); err) {
            return err;
        }

        if (res) {
            *result = res;
        }
        
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

    int sys_dup(int fd, int flags, int *newfd) {
        auto res = syscall(SYS_dup2, fd, -1);
        if (int err = sc_error(res); err) {
            return err;
        }

        *newfd = res;
        return 0;
    }

    int sys_dup2(int fd, int flags, int newfd){
        auto res = syscall(SYS_dup2, fd, newfd);
        if (int err = sc_error(res); err) {
            return err;
        }

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

        auto res = syscall(SYS_statat, fd, path, statbuf, flags);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_isatty(int fd) {
        struct termios attr;
        if (sys_tcgetattr(fd, &attr)) {
            return ENOTTY;
        }

        return 0;
    }

    int sys_tcgetattr(int fd, struct termios *attr) {
        int res;
        if (int err = sys_ioctl(fd, TCGETS, (void *) attr, &res); err) {
            return err;
        }

        return 0;
    }

    int sys_tcsetattr(int fd, int how, const struct termios *attr) {
        int req;

        switch (how) {
            case TCSANOW: req = TCSETS; break;
            case TCSADRAIN: req = TCSETSW; break;
            case TCSAFLUSH: req = TCSETSF; break;
            default: return EINVAL;
        }

        if (int err = sys_ioctl(fd, req, (void *)attr, NULL); err) {
            return err;
        }

        return 0;        
    }

    int sys_fcntl(int fd, int request, va_list args, int *result) {
        auto res = syscall(SYS_fcntl, fd, request, va_arg(args, uint64_t));
        if (int err = sc_error(res); err) {
            return err;
        }

        *result = res;
        return 0;
    }
    
    int sys_mkdirat(int dirfd, const char *path, mode_t mode) {
        auto res = syscall(SYS_mkdirat, dirfd, path, mode);
        if (int err = sc_error(res); err) {
            return err;
        }

        return 0;
    }

    int sys_mkdir(const char *path, mode_t mode) {
        return sys_mkdirat(AT_FDCWD, path, mode);
    }

    int sys_open_dir(const char *path, int *fd) {
        return sys_open(path, O_DIRECTORY, 0, fd);
    }

    int sys_read_entries(int fd, void *buffer, size_t max_size, size_t *bytes_read) {
        auto res = syscall(SYS_readdir, fd, buffer);

        if (res == -1) {
            *bytes_read = 0;
            return 0;
        } else if (res == 1) {
            return res;
        }

        *bytes_read = sizeof(dirent);
        return 0;
    }
}