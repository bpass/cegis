/*
 * Copyright (C) 2000 - 2004, Amnon Barak (amnon@MOSIX.org)
 *
 * THIS  SOFTWARE  IS  PROVIDED IN ITS  "AS IS"  CONDITION, WITH NO WARRANTY
 * WHATSOEVER. NO LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING
 * FROM THE USE OF THIS SOFTWARE WILL BE ACCEPTED.
 */
/*
 * Author(s): Amnon Shiloh.
 */
#ifndef _LINUX_MOSIX_H
#define _LINUX_MOSIX_H

#ifdef CONFIG_MOSIX

#ifdef __KERNEL__

#include <linux/sched.h>
#include <linux/user.h>
#include <linux/elf.h>
#include <mos/defs.h>
#include <mos/request.h>

/* operations on DEPUTY's data-base of REMOTE files: */

extern int mosix_register_a_file(struct file *, int);
extern void mosix_undo_last_file_registration(struct file *, int);
extern int mosix_rebuild_file_list(void);
extern void mosix_update_remote_files(void);

struct vmalist
{
	unsigned int vmstart;
	unsigned int vmend;
	unsigned short vmflags;
	short	maydump;
};

struct vmamaps
{
	unsigned long vmstart, vmend, vmflags, vmpgoff;
	struct file *fp;
};

extern void init_mosix(void);
extern void wake_up_mosix(struct task_struct *);
extern int mosix_wakeable(struct task_struct *);
extern int mosix_go_home(int);
extern int mosix_go_home_for_reason(int, int);
extern int mosix_send_back_home(struct task_struct *);
extern int mosix_need_while_asleep(void);
extern void mosix_run_while_asleep(void);
extern void mosix_pre_usermode_functions(void);
extern int stay_me_and_my_clones(uint32_t);
extern void unstay_mm(struct mm_struct *);
extern void mosix_inform_remote_of_nice(void);
extern void mosix_max_map_count_changed(void);
extern void mosix_snap_load(int);
extern void mosix_clear_statistics(void);
extern int mosix_fork_init_fields(struct task_struct *, long);
extern void mosix_fork_free_fields(struct task_struct *);
extern void mosix_exit(void);
extern void mosix_very_exit(void);
extern void mosix_obtain_registers(unsigned long);
extern void mosix_bring_monkey_users_back(struct inode *);
extern void mosix_no_longer_monkey(struct inode *);
extern void mosix_check_for_freedom_to_move(void);
extern int mosix_pre_clone(void);
extern void mosix_post_clone(void);
extern void mosix_remote_syscall_trace(void);
extern u64 mosix_remote_tsc(void);
extern int mosix_forkmigrate(void);
extern int mosix_deputy_fork(struct task_struct *, int, unsigned long);
extern void mosix_exit_mm(struct task_struct *);
extern void mosix_exec_mmap(struct mm_struct *);
extern void mosix_deputy_rusage(int);
extern int mosix_deputy_personality(unsigned long);
extern void mosix_deputy_count_args(char **, char**, int *, int *);
extern int mosix_deputy_bring_strings(struct linux_binprm *, char *, char **, char **);
extern int mosix_deputy_setup_args(int, unsigned long *);
extern int mosix_deputy_exec_mmap(char *);
extern int mosix_deputy_dump_thread(struct user *);
extern void mosix_deputy_init_aout_mm(struct exec *);
extern unsigned long mosix_deputy_elf_setup(char *, int, int, struct elfhdr *, unsigned long, unsigned long, unsigned long, int, int, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, struct elf_tables_extras *);
extern void mosix_deputy_fix_elf_aout_interp(unsigned, unsigned, unsigned);
extern int mosix_deputy_list_vmas(struct vmalist **, unsigned long *, unsigned long *);
extern long mosix_deputy_brk(unsigned long, unsigned long);

extern void mosix_notify_urgent(struct socket *);
extern void mosix_notify_receive(struct socket *);

extern void mosix_proc_init(void);
extern int mosix_proc_get_remote_array(char *, int, int);
extern int mosix_proc_get_node_array(char *, int, int);

extern void mosix_decay_exec(void);
extern void mosix_add_to_whereto(struct task_struct *, int);
extern void mosix_do_add_to_whereto(struct task_struct *, int);

struct sockaddr;
extern int reserved_mosix_address(struct sockaddr *);
extern void comm_report_violation(char *, struct sockaddr *);

void init_guest_user_struct(void);
extern int get_free_guest_slots(void);
extern int count_guests(void);

/* argument of mosix_deputy_setup_args: */
#define	SETUP_ARGS_PLAIN	0	
#define	SETUP_ARGS_AS_AOUT	1
#define	SETUP_ARGS_AS_ELF	2
#define	SETUP_ARGS_NOTYET	3

extern int mosix_deputy_restore_sigcontext(struct sigcontext *, int *);
extern void mosix_deputy_setup_frame(unsigned long, struct k_sigaction *, siginfo_t, sigset_t *);
extern unsigned long mosix_deputy_mmap(struct file *, unsigned long, int, unsigned long, unsigned long, unsigned long, off_t, nopage_t);
extern int deputy_munmap(unsigned long, size_t);
extern int deputy_mprotect(unsigned long, size_t, unsigned long);
extern void mosix_deputy_rlimit(int, struct rlimit);
extern int mosix_deputy_dump_fpu(struct user_i387_struct *);
extern int mosix_sync_caps(kernel_cap_t);

#define	ALL_REGISTERS	0x7fff	/* as many as in struct pt_regs */
#define	BIT_OF_REGISTER(_which) \
	(1 << (((int)&(((struct pt_regs *)0)->_which)) / sizeof(int)))

/*
 * meaning of signals on REMOTE
 */

#define	FATAL_SIGSEGV		SIGINT
#define	REMOTE_FILE_RELEASED	SIGQUIT

/* other signals that can occur on REMOTE:
 * SIGKILL, SIGSEGV, SIGPROF, SIGVTALRM, SIGFPE, SIGBUS, SIGIOT, SIGILL,
 * SIGXCPU, SIGXFSZ, SIGPROF, SIGTRQAP, SIGPWR (a tricky one).
 * SIGTERM requires the process to migrate back.
 */

#define	MOSIX_PRIORITY		(100)
#define	MOSIX_ASLEEP_PRIORITY	(MOSIX_PRIORITY * 3 / 10)
#define	MOSIX_DEPUTY_PRIORITY	(MOSIX_PRIORITY * 1 / 10)
#define	MOSIX_RESPOND_PRIORITY	(MOSIX_PRIORITY * 6 / 10)

#define	PROC_MOSIX_USE_START	1024	/* coordinate with proc/fs/base.c */

#if defined(CONFIG_MOSIX_DIAG) && defined(CONFIG_SMP)
#define KERNEL_LOCKED	do{if(current->lock_depth == -1)panic("not locked %d of " __FILE__, __LINE__);}while(0)
#define MOSIX_LOCKED	do{if(current->mosix.lock_depth == -1)panic("not locked %d of " __FILE__, __LINE__);}while(0)
#else
#define	KERNEL_LOCKED	do {} while(0)
#define	MOSIX_LOCKED	do {} while(0)
#endif /* CONFIG_MOSIX_DIAG */

#define	mos_to_contact(m) ((struct socket *)((m)->contact))
#define	mos_to_waitp(m)	((wait_queue_head_t *)(&(m)->wait_dist))
#define mos_to_regs(m)	((struct pt_regs *)((m)->altregs))

struct proc_dir_entry;
extern struct proc_dir_entry *proc_mosix;
typedef ssize_t (proc_mosix_pid_writer)(struct file *, struct task_struct *, const char *, size_t);
extern proc_mosix_pid_writer proc_mosix_pid_set_migrate;
extern proc_mosix_pid_writer proc_mosix_pid_set_goto;
extern proc_mosix_pid_writer proc_mosix_pid_set_lock;
extern proc_mosix_pid_writer proc_mosix_pid_set_sigmig;
extern proc_mosix_pid_writer proc_mosix_pid_set_disclosure;
#ifdef CONFIG_MOSIX_FS
extern proc_mosix_pid_writer proc_mosix_pid_set_selected;
#endif /* CONFIG_MOSIX_FS */

#ifdef CONFIG_MOSIX_UDB
extern void udbinit(void);
extern int udb_booting;
extern int nmi_debugger;
#endif /* CONFIG_MOSIX_UDB */

#endif /*__KERNEL__*/

#endif /* CONFIG_MOSIX */

#endif
