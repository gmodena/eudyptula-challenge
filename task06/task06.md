
For this task, I had to implement a misc char device driver,
register under /dev/eudtypytla. The driver has been implemented as a
kernel module.

## Char device driver 

A char device driver is a piece of functionality to transfer application (`user-space`) data to and from `kernel-space`. Data is read/written from a stream on a character-by-character base.

A char device driver can be accessed via a file attached to the `/dev` filesystem tree. Device drivers	
are represented by the kernel as a pair of `<major>:<minor>` numbers. Some major numbers
are reserved for a family of device types. For example, major number 13 is reserved for input devices.
A list of statically assigned major numbers can be found at `Documentation/admin-guide/devices.txt`.
A `<minor>` number is assigned to distinguish individual logical or physical devices. 

Let's look for example at `/dev/input/mouse0`, which on my system is the interface to a mouse pointer.
```
gmodena@debian:~$ ls -lh /dev/input/mouse0
crw-rw---- 1 root input 13, 32 May 19 22:39 /dev/input/mouse0
```
From the output of `ls -l` we can see that this file is a character device driver (first column of `crw`),
its major number is `13`, its minor is `32`.

Both major and minor numbers can be dynamically allocated at the module registration time.

## Interface
Linux exposes a `misc` interface (`<linux/miscdevice.h>`) to simplify (re) registration and I/O operation on char device drivers. 

- `static struct miscdevice` defines a misc device driver 
- `misc_register()` resigsters a misc device 
- `static const struct file_operations` defines callback functions for I/O operations on  the devicde

### Buffers & moving data from/to kernel space
To move data from/to user space, the following come in handy.

- `unsigned long copy_to_user (void __user* to, const void* from, unsigned long n);` copies data from user space to kernel space.
- `unsigned long copy_from_user (void* to, const void __user* from, unsigned long n);` copies data from kernel space to userspace

Recent kernels also expose two handy wrappers to both functions: `simple_write_to_buffer()` and `simple_read_from_buffer`.