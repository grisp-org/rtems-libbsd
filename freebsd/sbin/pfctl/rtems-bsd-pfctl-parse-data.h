/* generated by userspace-header-gen.py */
/*
 * NOTE: MANUALLY CHANGED.
 * YACC needs a special treatment for some variables. They are commented here.
 */
#include <rtems/linkersets.h>
#include "rtems-bsd-pfctl-data.h"
/* parse.c */
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct pfctl *pf);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int debug);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int rulestate);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static u_int16_t returnicmpdefault);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static u_int16_t returnicmp6default);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int blockpolicy);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int require_order);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int default_statelock);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct files files);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct file *file);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct symhead symhead);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct node_queue *queues);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct filter_opts filter_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct antispoof_opts antispoof_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct scrub_opts scrub_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct queue_opts queue_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct table_opts table_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct pool_opts pool_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct codel_opts codel_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct node_hfsc_opts hfsc_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct node_fairq_opts fairq_opts);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct node_state_opt *keep_state_defaults);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static struct loadanchorshead loadanchorshead);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static char *parsebuf);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int parseindex);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static int pushback_index);
/* RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static YYSTACKDATA yystack); */
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_pfctl, static char pushback_buffer[]);