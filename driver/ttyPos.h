#ifndef _TTY_POS_H_
#define _TTY_POS_H_

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/kref.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/time.h>
#include <linux/sched.h>
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
#include <linux/err.h>
#include <linux/kthread.h>
#endif
#endif
#define USB_VENDOR_ID		0x1234
#define USB_PRODUCT_ID		0x0101

static struct usb_device_id pos_usb_table[] = {
	{ USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID) },
	{},
};

MODULE_DEVICE_TABLE(usb,pos_usb_table);

//#define POS_TTY_MAJOR		192	/* experimental range */
#define POS_TTY_MINORS		20	/* only have 20 devices */

#define THREAD_INIT		0x00
#define THREAD_STOPPED		0x01
#define THREAD_CREATED		0x82
#define THREAD_CLOSE		0x83

#define THREAD_IS_RUNNING(ThreadState)	(ThreadState & 0x80)

#define WRITE_COMMAND		0	/* write to usb device command */
#define READ_COMMAND		1	/* read from to usb device command */
#define STATUS_COMMAND		2	/* get device buffer status command */

#define POOL_SIZE		10241

typedef struct _POOL {
	unsigned int ReadPos;
	unsigned int WritePos;
	unsigned char Buffer[POOL_SIZE];
} __attribute__((packed)) POOL, *PPOOL;

typedef struct {
	unsigned char SeqNo;
	unsigned char ReqType;		/* 0:OUT, 1:IN, 2:STAT, 3:RESET */
	unsigned short Len;
	unsigned char Data[508];
} __attribute__((packed)) ST_BULK_IO;

typedef struct {
	unsigned int TxBufSize;
	unsigned int RxBufSize;
	unsigned int TxLeft;
	unsigned int RxLeft;
} __attribute__((packed)) ST_BIO_STATE;

struct tty_pos {
	struct tty_struct *tty;
	unsigned char devIndex;

	/* for tiocmget and tiocmset functions */
	int msr;		/* MSR shadow */
	int mcr;		/* MCR shadow */

	struct file *filp;

	struct usb_device *udev;
	struct usb_interface *interface;
	struct urb *urb;
	wait_queue_head_t urb_wait;
	atomic_t urb_done;
	int timeout_jiffies;
	atomic_t urb_busy;

	wait_queue_head_t write_wait;
	atomic_t write_flag;

	__u8 bulk_in_epAddr;
	__u8 bulk_out_epAddr;

	struct kref kref;
	struct mutex io_mutex;		/* synchronize I/O with disconnect */
	volatile u8 ThreadState;
	struct completion ThreadExit_completion;

	unsigned char SeqCount;
	ST_BULK_IO *BioPack;		/* for IO access */
	ST_BIO_STATE BioDevState;

	POOL TxPool;
};

#define to_pos_dev(d)		container_of(d, struct tty_pos, kref)

#define INIT_POOL_BUFFER(pool)	pool.ReadPos = pool.WritePos = 0

#define IS_POOL_EMPTY(pool)	(pool.ReadPos == pool.WritePos)

#define GET_USING_POOL(pool) \
	((pool.WritePos + POOL_SIZE - pool.ReadPos) % POOL_SIZE)

#define GET_SPACE_POOL(pool)	(POOL_SIZE - 1 - GET_USING_POOL(pool))

#if 0
#define ERR(stuff...)
#define INFO(stuff...)
#else
#define ERR(stuff...)		printk(KERN_ERR "ttyPos: " stuff)
#define INFO(stuff...)		printk(KERN_INFO "ttyPos: " stuff)
#endif

//--error code define of API call
#define USB_ERR_NOT_OPEN        (-3403)//通道未打开
#define USB_ERR_BUF             (-3404)//发送缓冲区错误
#define USB_ERR_NOT_FREE        (-3405)//无可用的端口
#define USB_ERR_NO_CONF         (-3411)//设备未完成枚举和配置过程
#define USB_ERR_DISCONN         (-3412)//设备已经与主机断开
#define USB_ERR_MEM_SYSTEM      (-3413)//系统内存出现异常
#define USB_ERR_BUSY            (-3414)//USB系统忙碌
#define USB_ERR_RC_SYSTEM       (-3415)//系统资源申请失败
#define USB_ERR_DEV_ABSENT      (-3416)//USB主机上设备不在位
#define USB_ERR_INVALID         (-3417)//USB通讯状态无效
#endif
