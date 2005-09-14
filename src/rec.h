/**
 * @file rec.h
 * @brief Record specific function prototypes.
 * @internal
 */
/*
 * Copyright (C) 2004-2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_REC_H
#define _LIBSTDF_REC_H

#include <libstdf.h>

extern rec_unknown* stdf_read_rec_unknown(stdf_file*);
extern rec_far* stdf_read_rec_far(stdf_file*);
extern rec_atr* stdf_read_rec_atr(stdf_file*);
extern rec_mir* stdf_read_rec_mir(stdf_file*);
extern rec_mrr* stdf_read_rec_mrr(stdf_file*);
extern rec_pcr* stdf_read_rec_pcr(stdf_file*);
extern rec_hbr* stdf_read_rec_hbr(stdf_file*);
extern rec_sbr* stdf_read_rec_sbr(stdf_file*);
extern rec_pmr* stdf_read_rec_pmr(stdf_file*);
extern rec_pgr* stdf_read_rec_pgr(stdf_file*);
extern rec_plr* stdf_read_rec_plr(stdf_file*);
extern rec_rdr* stdf_read_rec_rdr(stdf_file*);
extern rec_sdr* stdf_read_rec_sdr(stdf_file*);
extern rec_wir* stdf_read_rec_wir(stdf_file*);
extern rec_wrr* stdf_read_rec_wrr(stdf_file*);
extern rec_wcr* stdf_read_rec_wcr(stdf_file*);
extern rec_pir* stdf_read_rec_pir(stdf_file*);
extern rec_prr* stdf_read_rec_prr(stdf_file*);
#ifdef STDF_VER3
extern rec_pdr* stdf_read_rec_pdr(stdf_file*);
extern rec_fdr* stdf_read_rec_fdr(stdf_file*);
#endif
extern rec_tsr* stdf_read_rec_tsr(stdf_file*);
extern rec_ptr* stdf_read_rec_ptr(stdf_file*);
extern rec_mpr* stdf_read_rec_mpr(stdf_file*);
extern rec_ftr* stdf_read_rec_ftr(stdf_file*);
extern rec_bps* stdf_read_rec_bps(stdf_file*);
extern rec_eps* stdf_read_rec_eps(stdf_file*);
#ifdef STDF_VER3
extern rec_shb* stdf_read_rec_shb(stdf_file*);
extern rec_ssb* stdf_read_rec_ssb(stdf_file*);
extern rec_sts* stdf_read_rec_sts(stdf_file*);
extern rec_scr* stdf_read_rec_scr(stdf_file*);
#endif
extern rec_gdr* stdf_read_rec_gdr(stdf_file*);
extern rec_dtr* stdf_read_rec_dtr(stdf_file*);

extern ssize_t stdf_write_rec_far(stdf_file*, rec_far*, uchar*);
extern ssize_t stdf_write_rec_atr(stdf_file*, rec_atr*, uchar*);
extern ssize_t stdf_write_rec_mir(stdf_file*, rec_mir*, uchar*);
extern ssize_t stdf_write_rec_mrr(stdf_file*, rec_mrr*, uchar*);
extern ssize_t stdf_write_rec_pcr(stdf_file*, rec_pcr*, uchar*);
extern ssize_t stdf_write_rec_hbr(stdf_file*, rec_hbr*, uchar*);
extern ssize_t stdf_write_rec_sbr(stdf_file*, rec_sbr*, uchar*);
extern ssize_t stdf_write_rec_pmr(stdf_file*, rec_pmr*, uchar*);
extern ssize_t stdf_write_rec_pgr(stdf_file*, rec_pgr*, uchar*);
extern ssize_t stdf_write_rec_plr(stdf_file*, rec_plr*, uchar*);
extern ssize_t stdf_write_rec_rdr(stdf_file*, rec_rdr*, uchar*);
extern ssize_t stdf_write_rec_sdr(stdf_file*, rec_sdr*, uchar*);
extern ssize_t stdf_write_rec_wir(stdf_file*, rec_wir*, uchar*);
extern ssize_t stdf_write_rec_wrr(stdf_file*, rec_wrr*, uchar*);
extern ssize_t stdf_write_rec_wcr(stdf_file*, rec_wcr*, uchar*);
extern ssize_t stdf_write_rec_pir(stdf_file*, rec_pir*, uchar*);
extern ssize_t stdf_write_rec_prr(stdf_file*, rec_prr*, uchar*);
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_pdr(stdf_file*, rec_pdr*, uchar*);
extern ssize_t stdf_write_rec_fdr(stdf_file*, rec_fdr*, uchar*);
#endif
extern ssize_t stdf_write_rec_tsr(stdf_file*, rec_tsr*, uchar*);
extern ssize_t stdf_write_rec_ptr(stdf_file*, rec_ptr*, uchar*);
extern ssize_t stdf_write_rec_mpr(stdf_file*, rec_mpr*, uchar*);
extern ssize_t stdf_write_rec_ftr(stdf_file*, rec_ftr*, uchar*);
extern ssize_t stdf_write_rec_bps(stdf_file*, rec_bps*, uchar*);
extern ssize_t stdf_write_rec_eps(stdf_file*, rec_eps*, uchar*);
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_shb(stdf_file*, rec_shb*, uchar*);
extern ssize_t stdf_write_rec_ssb(stdf_file*, rec_ssb*, uchar*);
extern ssize_t stdf_write_rec_sts(stdf_file*, rec_sts*, uchar*);
extern ssize_t stdf_write_rec_scr(stdf_file*, rec_scr*, uchar*);
#endif
extern ssize_t stdf_write_rec_gdr(stdf_file*, rec_gdr*, uchar*);
extern ssize_t stdf_write_rec_dtr(stdf_file*, rec_dtr*, uchar*);

#endif /* _LIBSTDF_REC_H */
