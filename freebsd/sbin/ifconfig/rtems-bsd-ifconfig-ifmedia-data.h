/* generated by userspace-header-gen.py */
#include <rtems/linkersets.h>
#include "rtems-bsd-ifconfig-data.h"
/* ifmedia.c */
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmediareq *ifmedia_getstate_ifmr);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static int did_it);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_type_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ethernet_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ethernet_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ethernet_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_tokenring_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_tokenring_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_tokenring_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_fddi_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_fddi_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_fddi_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ieee80211_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ieee80211_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_ieee80211_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_atm_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_atm_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_atm_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_shared_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_subtype_shared_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_shared_option_descriptions[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_description ifm_shared_option_aliases[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct ifmedia_type_to_subtype ifmedia_types_to_subtypes[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct cmd media_cmds[]);
RTEMS_LINKER_RWSET_CONTENT(bsd_prog_ifconfig, static struct afswtch af_media);
