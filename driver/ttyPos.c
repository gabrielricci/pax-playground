#include "ttyPos.h"

#define DRV_VERSION	"303"
#define VERSION_DATE    "2014.10.13_01"
#define MAX_RETRY_S	5
#define DRV_NAME	"ttyPos"

static struct tty_pos *pdx_table[POS_TTY_MINORS];

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
static struct tty_port	pos_port[POS_TTY_MINORS];
#endif
#endif

static unsigned char ResetPipePort(struct tty_pos *dev)
{
	struct tty_pos *pdx = dev;
	int retval;

	retval = usb_clear_halt(pdx->udev, usb_sndbulkpipe(pdx->udev,
	    pdx->bulk_out_epAddr));
	if (retval) {
		ERR("%s - ERROR CLEAR %X HALT = %d",
		    __func__, pdx->bulk_out_epAddr, retval);
		goto reset_port;
	}

	retval = usb_clear_halt(pdx->udev, usb_rcvbulkpipe(pdx->udev,
	    pdx->bulk_in_epAddr));
	if (retval) {
		ERR("%s - ERROR CLEAR %X HALT = %d",
		    __func__, pdx->bulk_in_epAddr, retval);
		goto reset_port;
	}

	return 0;

 reset_port:
	retval = usb_reset_device(pdx->udev);
	if (retval) {
		ERR("%s - ERROR RESETTING DEVICE: %d", __func__, retval);
	}

	return retval;
}

static int VerifyChecksum(ST_BULK_IO *p_bio)
{
	unsigned char a, b;
	int i, dn;

	dn = p_bio->Len + 4;
	a = 0;

	for (i = 2; i < dn; i++) {
		a ^= ((unsigned char *)p_bio)[i];
	}

	a ^= p_bio->SeqNo & 0x0f;
	a ^= p_bio->ReqType & 0x0f;
	b = (p_bio->SeqNo & 0xf0) + (p_bio->ReqType >> 4);
	if (a != b)
		return 1;

	/* clear checksum field */
	p_bio->SeqNo &= 0x0f;
	p_bio->ReqType &= 0x0f;
	return 0;
}

static void SetChecksum(ST_BULK_IO *p_bio)
{
	unsigned char a;
	int i, dn;

	dn = p_bio->Len + 4;
	a = 0;

	for (i = 2; i < dn; i++) {
		a ^= ((unsigned char *)p_bio)[i];
	}

	a ^= p_bio->SeqNo & 0x0f;
	a ^= p_bio->ReqType & 0x0f;

	/* fill high 4 bits of checksum into high 4 bits of ID field */
	p_bio->SeqNo = (p_bio->SeqNo & 0x0f) | (a & 0xf0);

	/* fill low 4 bits of checksum into high 4 bits of REQ_TYPE field */
	p_bio->ReqType |= (a << 4);
}

static void UrbCallBack(struct urb *urb)
{
	struct tty_pos *pdx;

	pdx = (struct tty_pos *)urb->context;

	atomic_set(&pdx->urb_done, 1);
	wake_up(&pdx->urb_wait);
}

static int SendAndWaitUrb(struct tty_pos *dev)
{
	struct tty_pos *pdx = dev;
	int retval;

	atomic_set(&pdx->urb_done, 0);

	/* send the data out the bulk port */
	retval = usb_submit_urb(pdx->urb, /* GFP_KERNEL */ GFP_ATOMIC);
	if (retval) {
		ERR("%s - FAILED SUBMITTING WRITE URB: %d", __func__, retval);
		retval = 1;
		goto exit;
	}
	retval = wait_event_timeout(pdx->urb_wait,
	    (atomic_read(&pdx->urb_done) == 1), pdx->timeout_jiffies);
	if (retval == 0) {
		/* INFO("URB TIMEOUT\n"); */

		if (atomic_read(&pdx->urb_done) == 0) {	/* urb is not done */
#if 0
			usb_unlink_urb(pdx->urb);
			printk(KERN_ALERT "usb_done: %d; %d\n",
			    (atomic_read(&pdx->urb_done) == 1),
			    /* pdx->urb_done, */ usb_unlink_urb(pdx->urb));
#endif
			usb_kill_urb(pdx->urb);
#if 0
			wait_event(pdx->urb_wait,
			    (atomic_read(&pdx->urb_done) == 1));
#endif
		}
#if 0
		INFO("urb->status: %d, %d\n", pdx->urb->status,
		    pdx->urb->actual_length);
#endif
		retval = 2;
		goto exit;
	}
	else if (retval < 0) {
		ERR("%s - WAIT FAILED: %d", __func__, retval);
		retval = 3;
		goto exit;
	}
	else {
		retval = 0;
#if 0
		printk(KERN_INFO "Use jiffies: %d", pdx->timeout_jiffies -
		    retval);
#endif
	}

	if (pdx->urb->status) {
		/* if (pdx->urb->status != -EREMOTEIO) */
		{
			ERR("%s - nonzero status received: %d", __func__,
			    pdx->urb->status);
            ERR("urb status:%d\n",pdx->urb->status);    
			retval = 4;
			goto exit;
		}
	}
 exit:
	return retval;
}

/* error codes: 11~29 */
static int ProcessCommand(struct tty_pos *dev)
{
	struct tty_pos *pdx = dev;
	int retval;

	/* stage 1: send command pack */

	SetChecksum((ST_BULK_IO *)pdx->BioPack);

	if (pdx->urb == NULL)
		return 18;

	if (pdx->urb->status == -EINPROGRESS) {
#if 0
		ERR("URB IN PROGRESS\n");
#endif
		return 19;
	}

	usb_fill_bulk_urb(pdx->urb, pdx->udev,
	    usb_sndbulkpipe(pdx->udev, pdx->bulk_out_epAddr),
	    pdx->BioPack, pdx->BioPack->Len + 4, UrbCallBack, pdx);

	retval = SendAndWaitUrb((struct tty_pos *)pdx);

	if (retval != 0)
		return retval + 10;

	/* stage 2: receive answer pack */

	/* clear pack flags */
	pdx->BioPack->SeqNo = 0;
	pdx->BioPack->ReqType = 0;
	pdx->BioPack->Len = 0;
	if (pdx->urb == NULL)
		return 28;

	usb_fill_bulk_urb(pdx->urb, pdx->udev,
	    usb_rcvbulkpipe(pdx->udev, pdx->bulk_in_epAddr),
	    pdx->BioPack, sizeof(*pdx->BioPack), UrbCallBack, pdx);

	retval = SendAndWaitUrb((struct tty_pos *)pdx);
	if (retval != 0)
		return retval + 20;

	if (VerifyChecksum((ST_BULK_IO *)pdx->BioPack)) {
		unsigned int i;
		/* unsigned char x; */

		ERR("VERIFY CHECKSUM FAILED: %d\n", retval);
		ERR("%X; %X; %X\n", pdx->BioPack->SeqNo,
		    pdx->BioPack->ReqType, pdx->BioPack->Len);

		for (i = 0; i < 508; i++) {
			INFO("%X\n", pdx->BioPack->Data[i]);
		}
#if 0
		for (i = 0, x = pdx->BioPack.Data[0];
		    i < pdx->BioPack.Len; i++, x++) {
			if (pdx->BioPack.Data[i] != x) {
				printk(KERN_ALERT "%d: %X; %X\n", i - 1,
				    pdx->BioPack.Data[i - 1], x - 1);
				printk(KERN_ALERT "%d: %X; %X\n", i,
				    pdx->BioPack.Data[i], x);
				printk(KERN_ALERT "%d: %X; %X\n", i + 1,
				    pdx->BioPack.Data[i + 1], x + 1);
				printk(KERN_ALERT "%d: %X; %X\n", i + 2,
				    pdx->BioPack.Data[i + 2], x + 2);
				printk(KERN_ALERT "%d: %X; %X\n", i + 3,
				    pdx->BioPack.Data[i + 3], x + 3);
				break;
			}
		}
#endif
		return 29;
	}

	return 0;
}

static void SleepMs(unsigned int nMs, struct tty_pos *dev)
{
#if 0
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout(nMs * HZ / 1000);
#endif
	struct tty_pos *pdx = dev;

	unsigned int timeout = (nMs * HZ / 1000);
	if (timeout < 1) {
		timeout = 1;
	}

	wait_event_timeout(pdx->write_wait,
	    (atomic_read(&pdx->write_flag) == 1), timeout);
}

static int ThreadCallBack(void *data)
{
	struct tty_pos *pdx = data;
	unsigned char loops;
	int retval;
	unsigned int i, rlen, wlen;
	struct tty_struct *tty;

	retval = ResetPipePort(pdx);
	if (retval != 0) {
		retval = 1;
		goto exit;
	}

	tty = pdx->tty;

	while (pdx->ThreadState == THREAD_CREATED) {

		/* get device buffer status */
		for (loops = 0; (loops < MAX_RETRY_S) &&
		    (pdx->ThreadState == THREAD_CREATED); loops++) {
			/* building command pack */
			pdx->SeqCount = (pdx->SeqCount + 1) & 0x0f;
			pdx->BioPack->SeqNo = pdx->SeqCount;
			pdx->BioPack->ReqType = STATUS_COMMAND;
			pdx->BioPack->Len = 0;

			retval = ProcessCommand((struct tty_pos *)pdx);
			if (retval != 0) {
				retval += 100;
				goto loop_s_tail;
			}

			if (pdx->urb->actual_length != 20) {
				retval = 130;
				goto loop_s_tail;
			}

			if (pdx->BioPack->SeqNo != pdx->SeqCount) {
				retval = 131;
				goto loop_s_tail;
			}

			if (pdx->BioPack->ReqType != STATUS_COMMAND) {
				retval = 132;
				goto loop_s_tail;
			}

			memcpy(&pdx->BioDevState, pdx->BioPack->Data,
			    sizeof(pdx->BioDevState));

			if ((!pdx->BioDevState.TxLeft) &&
			    IS_POOL_EMPTY(pdx->TxPool)) {
				SleepMs(10, (struct tty_pos *)pdx);
			}

 loop_s_tail:
			if (retval == 0)
				break;
			ERR("STATUS RETRY, loop: %d, err: %d, seq: %02X\n",
			    loops, retval, pdx->SeqCount);
			ResetPipePort(pdx);
		}

		if (retval != 0)
			goto exit;

 r_process:	/* read from usb device */

		for (loops = 0; (loops < MAX_RETRY_S) &&
		    (pdx->ThreadState == THREAD_CREATED); loops++) {
			if (!pdx->BioDevState.TxLeft)
				goto w_process;

			rlen = pdx->BioDevState.TxLeft;

			if (rlen > sizeof(pdx->BioPack->Data)) {
				rlen = sizeof(pdx->BioPack->Data);
			}
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
            rlen = tty_buffer_request_room(&pos_port[pdx->devIndex],rlen);
#else
			rlen = tty_buffer_request_room(tty, rlen);
#endif
#endif
			if (!loops) {
				pdx->SeqCount = (pdx->SeqCount + 1) & 0x0f;
			}

			pdx->BioPack->SeqNo = pdx->SeqCount;
			pdx->BioPack->ReqType = READ_COMMAND;
			pdx->BioPack->Len = 2;
			/* in dlen required */
			pdx->BioPack->Data[0] = (unsigned short)rlen & 0xff;
			pdx->BioPack->Data[1] = (unsigned short)rlen >> 8;

			retval = ProcessCommand((struct tty_pos *)pdx);
			if (retval != 0) {
				retval += 200;
				goto loop_r_tail;
			}

			if (pdx->BioPack->SeqNo != pdx->SeqCount) {
				retval = 231;
				goto loop_r_tail;
			}

			if (pdx->BioPack->ReqType != READ_COMMAND) {
				if ((pdx->BioPack->ReqType == STATUS_COMMAND) &&
				    (pdx->BioPack->Len >=
				    sizeof(pdx->BioDevState))) {
					memcpy(&pdx->BioDevState,
					    pdx->BioPack->Data,
					    sizeof(pdx->BioDevState));
					goto w_process;	/* no data to fetch */
				}

				retval = 232;
				ERR("  %02X, ERROR req_type: %02X.\n",
				    pdx->SeqCount, pdx->BioPack->ReqType);

				goto loop_r_tail;
			}

			if (pdx->urb->actual_length <
			    (int)pdx->BioPack->Len + 4) {
				retval = 233;
				goto loop_r_tail;
			}

			if (pdx->BioPack->Len > rlen) {
				ERR("MORE DATA FETCHED THAN DECLARED, NEED: "
				    "%d, RN: %d\n", rlen, pdx->BioPack->Len);
				retval = 234;
				goto exit;
			}

			rlen = pdx->BioPack->Len;
#if 0
			for (j = 0; j < rlen - 1; j++) {
				if ((pdx->BioPack.Data[j] + 1 !=
				    pdx->BioPack.Data[j + 1])
				    && (pdx->BioPack.Data[j + 1] != 0)) {
					inerr = 235;
					goto exit;
				}
			}
#endif
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
            tty_insert_flip_string(&pos_port[pdx->devIndex], pdx->BioPack->Data, rlen);
			tty_flip_buffer_push(&pos_port[pdx->devIndex]);
#else            
			tty_insert_flip_string(tty, pdx->BioPack->Data, rlen);
			tty_flip_buffer_push(tty);
#endif
#endif
			pdx->BioDevState.TxLeft -= rlen;
#if 0
			printk(KERN_ALERT "%02X, RN: %d\n", pdx->SeqCount,
			    rlen);
#endif

			if (pdx->BioDevState.TxLeft)
				goto r_process;
 loop_r_tail:
			if (retval == 0)
				break;

			ERR("RX RETRY, loop: %d, err: %d, SEQ: %02X\n",
			    loops, retval, pdx->SeqCount);

			ResetPipePort(pdx);
		}

		if (retval)
			goto exit;

 w_process:	/* write to usb device */

		wlen = GET_USING_POOL(pdx->TxPool);

		if (wlen > sizeof(pdx->BioPack->Data)) {
			wlen = sizeof(pdx->BioPack->Data);
		}

		if (wlen > pdx->BioDevState.RxLeft) {
			wlen = pdx->BioDevState.RxLeft;
		}

		for (loops = 0; (loops < MAX_RETRY_S) &&
		    (pdx->ThreadState == THREAD_CREATED); loops++) {
			if (wlen == 0)
				break;

			if (!loops) {
				pdx->SeqCount = (pdx->SeqCount + 1) & 0x0f;
			}

			pdx->BioPack->SeqNo = pdx->SeqCount;
			pdx->BioPack->ReqType = WRITE_COMMAND;
			pdx->BioPack->Len = (unsigned short)wlen;

			for (i = 0; i < wlen; i++) {
				pdx->BioPack->Data[i] =
				    pdx->TxPool.Buffer[(pdx->TxPool.ReadPos +
				    i) % POOL_SIZE];
			}

			retval = ProcessCommand((struct tty_pos *)pdx);
			if (retval != 0) {
				retval += 300;
				goto loop_w_tail;
			}

			if (pdx->urb->actual_length != 20) {
				retval = 330;
				goto loop_w_tail;
			}

			if (pdx->BioPack->SeqNo != pdx->SeqCount) {
				retval = 331;

				ERR("***** Mismatched with SEQ: %02X, "
				    "wlen: %d, loop: %d\n", pdx->SeqCount,
				    wlen, loops);

				ERR(" SEQ: %02X, type: %02X, dn: %d\n",
				    pdx->BioPack->SeqNo, pdx->BioPack->ReqType,
				    pdx->BioPack->Len);

				for (i = 0; i < pdx->BioPack->Len; i++) {
					INFO("%02X ", pdx->BioPack->Data[i]);
				}

				if (((pdx->BioPack->SeqNo + 1) % 16) ==
				    pdx->SeqCount)
					goto loop_w_tail;

				goto exit;
			}	/* mismatched seq_no */

			if (pdx->BioPack->ReqType != STATUS_COMMAND) {
				retval = 332;

				ERR(" SEQ: %02X, type: %02X, dn: %d\n",
				    pdx->BioPack->SeqNo, pdx->BioPack->ReqType,
				    pdx->BioPack->Len);

				for (i = 0; i < sizeof(pdx->BioPack->Data);
				    i++) {
					INFO("%02X ", pdx->BioPack->Data[i]);
				}

				INFO("\n");

				goto exit;
			}	/* mismatched req_type */
#if 0
			printk(KERN_ALERT "%02X, WN: %d\n", pdx->SeqCount,
			    wlen);
#endif
			pdx->TxPool.ReadPos = (pdx->TxPool.ReadPos + wlen) %
			    POOL_SIZE;

			memcpy(&pdx->BioDevState, pdx->BioPack->Data,
			    sizeof(pdx->BioDevState));

			if (!IS_POOL_EMPTY(pdx->TxPool))
				goto w_process;

			atomic_set(&pdx->write_flag, 0);
 loop_w_tail:
			if (retval == 0)
				break;

			ERR("TX RETRY, loop: %d, err: %d, SEQ: %02X\n",
			    loops, retval, pdx->SeqCount);

			ResetPipePort(pdx);
		}

		if (retval)
			goto exit;
	}

 exit:
#if 0    
	INFO("ThreadCallBack Exit\n");
#endif
	if (retval != 0) {
		ERR("%02X, ERR: %d\n", pdx->SeqCount, retval);

		ResetPipePort(pdx);
	}

	pdx->ThreadState = THREAD_STOPPED;
	complete(&pdx->ThreadExit_completion);
	do_exit(0);
}

static void pos_delete(struct kref *kref)
{
	struct tty_pos *pdx = to_pos_dev(kref);

	if (pdx == NULL)
		return;

    if(pdx->devIndex >= POS_TTY_MINORS)
		return;

	if (pdx->tty) {
		pdx->tty->driver_data = NULL;
	}

	pdx_table[pdx->devIndex] = NULL;
	usb_free_urb(pdx->urb);
	usb_put_dev(pdx->udev);
	kfree(pdx->BioPack);
	kfree(pdx);
}

static int pos_open(struct tty_struct *tty, struct file *filp)
{
	struct tty_pos *pdx;
	
    if(tty==NULL)return USB_ERR_MEM_SYSTEM;
    if(tty->index >= POS_TTY_MINORS || tty->index<0)return USB_ERR_MEM_SYSTEM;
    
//    INFO("%s index:%d\n",__func__,tty->index);
	pdx = pdx_table[tty->index];
	if (pdx == NULL)
		return USB_ERR_DEV_ABSENT;

	if (THREAD_IS_RUNNING(pdx->ThreadState)) {
		return USB_ERR_BUSY;
	}

	tty->driver_data = pdx;
	pdx->tty = tty;

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    tty_port_tty_set(&pos_port[pdx->devIndex], tty);
#endif
#endif

	kref_get(&pdx->kref);

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    reinit_completion(&pdx->ThreadExit_completion);
#else
	INIT_COMPLETION(pdx->ThreadExit_completion);
#endif
#endif
	pdx->ThreadState = THREAD_CREATED;
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
{
    struct task_struct *thread;
    thread = kthread_run(ThreadCallBack,(struct tty_pos *)pdx,"ThreadCallBack");
    if (IS_ERR(thread))
    {
		ERR("FAILED TO CREATE KERNEL THREAD!\n");
		pdx->ThreadState = THREAD_INIT;
		kref_put(&pdx->kref, pos_delete);
		return USB_ERR_RC_SYSTEM;
	}
}
#else
{
    pid_t pid;
	pid = kernel_thread(ThreadCallBack, (struct tty_pos *)pdx,
	    CLONE_FS | CLONE_FILES);
	if (pid < 0) {
		ERR("FAILED TO CREATE KERNEL THREAD!\n");
		pdx->ThreadState = THREAD_INIT;
		kref_put(&pdx->kref, pos_delete);
		return USB_ERR_RC_SYSTEM;
	}
}
#endif
#endif

	pdx->filp = filp;
	return 0;
}

static void pos_close(struct tty_struct *tty, struct file *filp)
{
	struct tty_pos *pdx = tty->driver_data;

	if (pdx == NULL)
		return;
    if(pdx_table[pdx->devIndex]==NULL)
        return;

	if (pdx->ThreadState == THREAD_CREATED) {
		pdx->ThreadState = THREAD_CLOSE;
		wait_for_completion(&pdx->ThreadExit_completion);
	}

	pdx->ThreadState = THREAD_INIT;

	kref_put(&pdx->kref, pos_delete);
    //tty_ldisc_flush(tty);
}

static int pos_write(struct tty_struct *tty, const unsigned char *buf,
    int count)
{
	struct tty_pos *pdx = tty->driver_data;
	unsigned int wn, i;
	int retval;

	if (!pdx)
		return USB_ERR_NOT_OPEN;

	if (!THREAD_IS_RUNNING(pdx->ThreadState)) {
		retval = USB_ERR_INVALID;
		goto exit;
	}

	if (count == 0) {
		retval = 0;
		goto exit;
	}

	wn = GET_SPACE_POOL(pdx->TxPool);
	if (wn >= count) {
		wn = count;
	}
	else
		return USB_ERR_BUF;

	for (i = 0; i < wn; i++) {
		pdx->TxPool.Buffer[(pdx->TxPool.WritePos + i) % POOL_SIZE] =
		    buf[i];
	}

	pdx->TxPool.WritePos = (pdx->TxPool.WritePos + wn) % POOL_SIZE;
	retval = wn;

	atomic_set(&pdx->write_flag, 1);
	wake_up(&pdx->write_wait);
 exit:
	return retval;
}

static int pos_write_room(struct tty_struct *tty)
{
	struct tty_pos *pdx = tty->driver_data;
	int room = -EINVAL;

	if (!pdx)
		return -ENODEV;

	room = GET_SPACE_POOL(pdx->TxPool);
	return room;
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
static int pos_ioctl(struct tty_struct *tty, struct file *filp,
    unsigned int cmd, unsigned long arg)
#else
static int pos_ioctl(struct tty_struct *tty, unsigned int cmd,
    unsigned long arg)
#endif    
{
	struct tty_pos *pdx = tty->driver_data;

#if 0
	INFO("pos_ioctl\n");
#endif
	if (!pdx)
		return -ENODEV;

	switch (cmd) {
	case TIOCGSERIAL:
	case TIOCMIWAIT:
	case TIOCGICOUNT:
	default:
		break;
	}
    #if 0
    INFO("%s-cmd: 0x%X\n", __func__, cmd);
    #endif
	return -ENOIOCTLCMD;
}

#define RELEVANT_IFLAG(iflag) \
	((iflag) & (IGNBRK | BRKINT | IGNPAR | PARMRK | INPCK))

static void pos_set_termios(struct tty_struct *tty,
    struct ktermios *old_termios)
{
	unsigned int cflag;

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    cflag = tty->termios.c_cflag;
#else
	cflag = tty->termios->c_cflag;
#endif
#endif
	/* check that they really want us to change something */
	if (old_termios) {
		if ((cflag == old_termios->c_cflag) &&
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
            (RELEVANT_IFLAG(tty->termios.c_iflag) ==
#else            
		    (RELEVANT_IFLAG(tty->termios->c_iflag) ==
#endif
#endif
		    RELEVANT_IFLAG(old_termios->c_iflag))) {
#if 0
			INFO(" - nothing to change...\n");
#endif
			return;
		}
	}
#if 0
	/* get the byte size */
	switch (cflag & CSIZE) {
	case CS5:
		INFO(" - data bits = 5\n");
		break;
	case CS6:
		INFO(" - data bits = 6\n");
		break;
	case CS7:
		INFO(" - data bits = 7\n");
		break;
	default:
	case CS8:
		INFO(" - data bits = 8\n");
		break;
	}

	/* determine the parity */
	if (cflag & PARENB) {
		if (cflag & PARODD) {
			INFO(" - parity = odd\n");
		}
		else {
			INFO(" - parity = even\n");
		}
	}
	else {
		INFO(" - parity = none\n");
	}

	/* figure out the stop bits requested */
	if (cflag & CSTOPB) {
		INFO(" - stop bits = 2\n");
	}
	else {
		INFO(" - stop bits = 1\n");
	}

	/* figure out the hardware flow control settings */
	if (cflag & CRTSCTS) {
		INFO(" - RTS/CTS is enabled\n");
	}
	else {
		INFO(" - RTS/CTS is disabled\n");
	}

	/* determine software flow control.
	 * if we are implementing XON/XOFF, set the start and
	 * stop character in the device */
	if (I_IXOFF(tty) || I_IXON(tty)) {
		unsigned char stop_char = STOP_CHAR(tty);
		unsigned char start_char = START_CHAR(tty);

		/* if we are implementing INBOUND XON/XOFF */
		if (I_IXOFF(tty)) {
			INFO(" - INBOUND XON/XOFF is enabled, "
			    "XON = %2x, XOFF = %2x", start_char, stop_char);
		}
		else {
			INFO(" - INBOUND XON/XOFF is disabled");
		}

		/* if we are implementing OUTBOUND XON/XOFF */
		if (I_IXON(tty)) {
			INFO(" - OUTBOUND XON/XOFF is enabled, "
			    "XON = %2x, XOFF = %2x", start_char, stop_char);
		}
		else {
			INFO(" - OUTBOUND XON/XOFF is disabled");
		}
	}

	/* get the baud rate wanted */
	INFO(" - baud rate = %d\n", tty_get_baud_rate(tty));
#endif
}

static void pos_throttle(struct tty_struct *tty)
{
	/* INFO("pos_throttle\n"); */
}

static void pos_unthrottle(struct tty_struct *tty)
{
	/* INFO("pos_unthrottle\n"); */
}

static void pos_flush_buffer(struct tty_struct *tty)
{
	struct tty_pos *pdx = tty->driver_data;

#if 0
	INFO("pos_flush_buffer\n");
#endif

	if (!pdx)
		return;

	INIT_POOL_BUFFER(pdx->TxPool);
}

static int pos_chars_in_buffer(struct tty_struct *tty)
{
	int in_buf_len;
	struct tty_pos *pdx = tty->driver_data;

	if (!pdx)
		return -ENODEV;

	in_buf_len = GET_USING_POOL(pdx->TxPool);
#if 0
	printk(KERN_ALERT "pos_chars_in_buffer: %d\n", in_buf_len);
#endif

	return in_buf_len;
}

/* Our fake UART values */
#define MCR_DTR		0x01
#define MCR_RTS		0x02
#define MCR_LOOP	0x04
#define MSR_CTS		0x08
#define MSR_CD		0x10
#define MSR_RI		0x20
#define MSR_DSR		0x40

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
static int pos_tiocmget(struct tty_struct *tty, struct file *filp)
#else
static int pos_tiocmget(struct tty_struct *tty)
#endif
{
	struct tty_pos *pdx = tty->driver_data;
	unsigned int msr, mcr, result;

#if 0
	INFO("pos_tiocmget\n");
#endif
	if (!pdx)
		return -ENODEV;

	msr = pdx->msr;
	mcr = pdx->mcr;

	result = ((mcr & MCR_DTR) ? TIOCM_DTR : 0) |	/* DTR is set */
	    ((mcr & MCR_RTS) ? TIOCM_RTS : 0) |		/* RTS is set */
	    ((mcr & MCR_LOOP) ? TIOCM_LOOP : 0) |	/* LOOP is set */
	    ((msr & MSR_CTS) ? TIOCM_CTS : 0) |		/* CTS is set */
	    ((msr & MSR_CD) ? TIOCM_CAR : 0) |	/* Carrier detect is set */
	    ((msr & MSR_RI) ? TIOCM_RI : 0) |	/* Ring Indicator is set */
	    ((msr & MSR_DSR) ? TIOCM_DSR : 0);		/* DSR is set */

	return result;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,0,0)
static int pos_tiocmset(struct tty_struct *tty, struct file *filp,
    unsigned int set, unsigned int clear)
#else
static int pos_tiocmset(struct tty_struct *tty, unsigned int set, 
    unsigned int clear)
#endif    
{
	struct tty_pos *pdx = tty->driver_data;
	unsigned int mcr;

	if (!pdx)
		return -ENODEV;

	mcr = pdx->mcr;

	if (set & TIOCM_RTS) {
		mcr |= MCR_RTS;
	}
	if (set & TIOCM_DTR) {
		mcr |= MCR_DTR;		/* mcr |= MCR_RTS; */
	}

	if (clear & TIOCM_RTS) {
		mcr &= ~MCR_RTS;
	}
	if (clear & TIOCM_DTR) {
		mcr &= ~MCR_DTR;	/* mcr &= ~MCR_RTS; */
	}

	/* set the new MCR value in the device */
	pdx->mcr = mcr;
	return 0;
}

static const struct tty_operations pos_ops = {
	.open = pos_open,
	.close = pos_close,
	.write = pos_write,
	.write_room = pos_write_room,
	.ioctl = pos_ioctl,
	.set_termios = pos_set_termios,
	.throttle = pos_throttle,
	.unthrottle = pos_unthrottle,
	.flush_buffer = pos_flush_buffer,
	.chars_in_buffer = pos_chars_in_buffer,
	.tiocmget = pos_tiocmget,
	.tiocmset = pos_tiocmset,
};

struct tty_driver *pos_tty_driver;

static int pos_usb_probe(struct usb_interface *interface,
    const struct usb_device_id *id)
{
	struct tty_pos *pdx;
	struct usb_host_interface *iface_desc;
	struct usb_endpoint_descriptor *endpoint;

	int i, retval = -ENOMEM;

    for(i=0;i<POS_TTY_MINORS;i++)
    {
		if (pdx_table[i] == NULL)
			break;
	}

    if(i==POS_TTY_MINORS)
		return -ENOMEM;

	pdx = kzalloc(sizeof(*pdx), /* GFP_KERNEL */ GFP_ATOMIC);
	if (!pdx) {
		ERR("OUT OF MEMORY pdx\n");
		return -ENOMEM;
	}

	pdx->BioPack = kzalloc(sizeof(*pdx->BioPack), GFP_ATOMIC);
	if (!pdx) {
		ERR("OUT OF MEMORY BioPack\n");
		return -ENOMEM;
	}

	printk(KERN_ALERT "ttyPos probe:%s %s,index:%d\n",DRV_VERSION,VERSION_DATE,i);

	pdx->devIndex = i;
	pdx_table[pdx->devIndex] = pdx;

	INIT_POOL_BUFFER(pdx->TxPool);

	pdx->timeout_jiffies = 400 * HZ / 1000;		/* 400ms */
	kref_init(&pdx->kref);
	init_waitqueue_head(&pdx->urb_wait);
	atomic_set(&pdx->urb_done, 0);

	init_waitqueue_head(&pdx->write_wait);
	atomic_set(&pdx->write_flag, 0);

	init_completion(&pdx->ThreadExit_completion);

	pdx->udev = usb_get_dev(interface_to_usbdev(interface));
	pdx->interface = interface;

	pdx->urb = usb_alloc_urb(0, /* GFP_KERNEL */ GFP_ATOMIC);
	if (!pdx->urb) {
		retval = -ENOMEM;
		ERR("FAILED ALLOC URB!\n");
		goto error;
	}

	iface_desc = interface->cur_altsetting;

	for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
		endpoint = &iface_desc->endpoint[i].desc;

		if (endpoint->bEndpointAddress & 0x80) {
			pdx->bulk_in_epAddr = endpoint->bEndpointAddress;
		}
		else {
			pdx->bulk_out_epAddr = endpoint->bEndpointAddress;
		}
	}

	if (!(pdx->bulk_in_epAddr && pdx->bulk_out_epAddr)) {
		ERR("COULD NOT FIND BOTH BULK-IN AND BULK-OUT ENDPOINT\n");
	}

	usb_set_intfdata(interface, pdx);
#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    tty_port_register_device(&pos_port[pdx->devIndex], pos_tty_driver,
			pdx->devIndex, NULL);
#else
    tty_register_device(pos_tty_driver, pdx->devIndex, NULL);
#endif
#endif

    dev_info(&interface->dev,
                "USB POS device now attached to PosUsb-%d",
                interface->minor);
	return 0;

 error:
	if (pdx) {
		kref_put(&pdx->kref, pos_delete);
	}

	ERR("--pos_probe error\n");
	return retval;
}

static void pos_usb_disconnect(struct usb_interface *interface)
{
	struct tty_pos *pdx;

#if 0
	INFO("++pos_disconnect\n");
#endif
	pdx = usb_get_intfdata(interface);
	if (pdx == NULL)
		return;

	tty_unregister_device(pos_tty_driver, pdx->devIndex);

	if (pdx->ThreadState == THREAD_CREATED) {
		pdx->ThreadState = THREAD_CLOSE;
#if 0        
		INFO("Wait thread exit\n");
#endif        
		wait_for_completion(&pdx->ThreadExit_completion);
#if 0        
		INFO("Wait thread exit success!\n");
#endif    
	}
	pdx->ThreadState = THREAD_INIT;

	usb_set_intfdata(interface, NULL);

#if 0
	usb_deregister_dev(interface, &pos_class);
#endif

	pdx->interface = NULL;

	kref_put(&pdx->kref, pos_delete);
#if 0
	INFO("--pos_disconnect\n");
#endif
}

#if 1
static int pos_usb_suspend(struct usb_interface *intf, pm_message_t message)
{
//	printk(KERN_ALERT "pos_suspend\n");

	return 0;
}

static int pos_usb_resume(struct usb_interface *intf)
{
//	printk(KERN_ALERT "pos_resume\n");

	return 0;
}
#endif

#ifdef OLD_USB_DRIVER
static void pos_usb_pre_reset(struct usb_interface *intf)
{
	/* struct tty_pos *pdx = usb_get_intfdata(intf); */
#if 0
	INFO("pos_pre_reset\n");
#endif
}
#else
static int pos_usb_pre_reset(struct usb_interface *intf)
{
	/* struct tty_pos *pdx = usb_get_intfdata(intf); */
#if 0
	INFO("pos_pre_reset\n");
#endif
	return 0;
}
#endif

#ifdef OLD_USB_DRIVER
static void pos_usb_post_reset(struct usb_interface *intf)
{
#if 0
	INFO("pos_post_reset\n");
#endif
}
#else
static int pos_usb_post_reset(struct usb_interface *intf)
{
#if 0
	INFO("pos_post_reset\n");
#endif
	return 0;
}
#endif

static struct usb_driver pos_usb_driver = {
    .name   =       "PosUsb",
	.probe = pos_usb_probe,
	.disconnect = pos_usb_disconnect,
#if 1
	.suspend = pos_usb_suspend,
	.resume = pos_usb_resume,
	.supports_autosuspend = 1,
#endif
	.pre_reset = pos_usb_pre_reset,
	.post_reset = pos_usb_post_reset,
	.id_table = pos_usb_table,
};

/* Compatible with TTY_DRIVER_DYNAMIC_DEV and TTY_DRIVER_NO_DEVFS */
#define TTY_USB_DEV     0x0008 

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))

static int pos_port_activate(struct tty_port *port, struct tty_struct *tty)
{
	return 0;
}

static void pos_port_shutdown(struct tty_port *port)
{

}

static int pos_carrier_raised(struct tty_port *port)
{
	return 0;
}

static void pos_dtr_rts(struct tty_port *port, int onoff)
{
}

static const struct tty_port_operations pos_port_ops = {
	.activate	= pos_port_activate,
	.shutdown	= pos_port_shutdown,
	.carrier_raised = pos_carrier_raised,
	.dtr_rts	= pos_dtr_rts,
};
#endif
#endif

static int __init pos_tty_init(void)
{
	int result,i;

	printk(KERN_ALERT "ttyPos:%s %s\n",DRV_VERSION,VERSION_DATE);

    for(i=0;i<POS_TTY_MINORS;i++)
		pdx_table[i] = NULL;

	pos_tty_driver = alloc_tty_driver(POS_TTY_MINORS);
	if (!pos_tty_driver)
		return -ENOMEM;

	pos_tty_driver->owner = THIS_MODULE;
	pos_tty_driver->driver_name = "usbpos";
	pos_tty_driver->name = 	"ttyPos";
	pos_tty_driver->major = 0;//POS_TTY_MAJOR;//The major number will be chosen dynamically
	pos_tty_driver->minor_start = 0;
	pos_tty_driver->type = TTY_DRIVER_TYPE_SERIAL;
	pos_tty_driver->subtype = SERIAL_TYPE_NORMAL;
	pos_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_USB_DEV;
	pos_tty_driver->init_termios = tty_std_termios;
	pos_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD
	    | HUPCL | CLOCAL;

	tty_set_operations(pos_tty_driver, &pos_ops);

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
	for (i = 0; i < POS_TTY_MINORS; i++) 
    {
		tty_port_init(&pos_port[i]);
		pos_port[i].ops = &pos_port_ops;
		pos_port[i].close_delay     = HZ / 2;	/* .5 seconds */
		pos_port[i].closing_wait    = 30 * HZ;/* 30 seconds */
	}
#endif
#endif

	result = tty_register_driver(pos_tty_driver);
	if (result) {
		ERR("%s - tty_register_driver failed\n", __func__);
		goto byebye1;
	}

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    for (i = 0; i < POS_TTY_MINORS; i++)
		tty_port_destroy(&pos_port[i]);
#endif
#endif

	result = usb_register(&pos_usb_driver);
	if (result) {
		ERR("%s - usb_register failed; err: %d\n",__func__, result);
        goto byebye2;
	}

    
	return 0;
byebye2:
	tty_unregister_driver(pos_tty_driver);
byebye1:
    put_tty_driver(pos_tty_driver);

	return result;
}

static void __exit pos_tty_exit(void)
{
    unsigned int i;
    
	usb_deregister(&pos_usb_driver);
	tty_unregister_driver(pos_tty_driver);
    put_tty_driver(pos_tty_driver);

#ifdef LINUX_VERSION_CODE
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
    for (i = 0; i < POS_TTY_MINORS; i++)
		tty_port_destroy(&pos_port[i]);
#endif
#endif

    INFO("pos_tty_exit\n");
}

module_init(pos_tty_init);
module_exit(pos_tty_exit);

MODULE_LICENSE("GPL");
MODULE_ALIAS_LDISC(N_SLIP);
