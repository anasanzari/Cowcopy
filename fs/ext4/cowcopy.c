#include <linux/compiler.h>
#include <linux/sched.h>


#include <linux/string.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/fsnotify.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/namei.h>
#include <linux/backing-dev.h>
#include <linux/capability.h>
#include <linux/securebits.h>
#include <linux/security.h>
#include <linux/mount.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/personality.h>
#include <linux/pagemap.h>
#include <linux/syscalls.h>
#include <linux/rcupdate.h>
#include <linux/audit.h>
#include <linux/falloc.h>
#include <linux/fs_struct.h>
#include <linux/ima.h>
#include <linux/dnotify.h>
#include <linux/stat.h>
#include <linux/unistd.h>



#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mbcache.h>
#include <linux/quotaops.h>
#include <linux/rwsem.h>
#include <linux/jbd2.h>
#include "ext4.h"

#include "xattr.h"

#include "ext4_jbd2.h"
#include "acl.h"


asmlinkage int sys_ext4_cowcopy(const char __user *src, const char __user *dest) {

  struct path pt; //path for src
  struct path destpt; //path for dest
  struct dentry *temp_dentry;
  int error;
  char ext4_string[] = "ext4";
  uint i = -1;

  printk("Before user_path_at call \n");

  error = user_path_at(AT_FDCWD, src, 0, &pt);
  if (error) { printk("File Not Found OR SoMEHTiNG \n"); return error; }

  // Check if file is a directory or not
  if(!S_ISREG(pt.dentry->d_inode->i_mode)){ printk("is A Directory \n"); return (-EPERM); }

  // Check if src is in a ext4 file system
  if(strcmp(ext4_string, pt.dentry->d_sb->s_type->name)){ printk("src is not in ext4"); return (-EOPNOTSUPP); }

  // Check if file already exists
  temp_dentry = user_path_create(AT_FDCWD, dest, &destpt, 0);
  if(temp_dentry == ERR_PTR(-EEXIST)){ printk("file already exists\n"); return (-EEXIST); }

  // Check if src and dest are in the same device
  if(pt.mnt->mnt_root != destpt.mnt->mnt_root) { printk("SOMESHITWEDONTUNDERSTAND"); return (-EXDEV); }

  // Check if file is being written to
  if (pt.dentry->d_inode->i_writecount.counter > 0) { printk("file is being written to "); return -EPERM; }

  // Make a hard link

  error = vfs_link(pt.dentry, destpt.dentry->d_inode, temp_dentry, NULL); //(to be linked, new parent dir, new dentry)
  mnt_drop_write(destpt.mnt);
  dput(temp_dentry);
  mutex_unlock(&destpt.dentry->d_inode->i_mutex);
  path_put(&destpt);
  path_put(&pt);
  if (error){ printk("vfs link error"); return error; }

  //check to make sure we are keeping count of cowcopy attr correctly
  error = ext4_xattr_get(temp_dentry->d_inode, 7, "cow_moo", &i, sizeof(int));
  error < 1 ? i = 1 : i++;

  // Set the correct count
  error =  ext4_xattr_set(temp_dentry->d_inode, 7, "cow_moo", &i, sizeof(int), 0);
  if(error){ printk("set count error"); return error; }

  return 0;
}
