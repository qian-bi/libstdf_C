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

extern ssize_t stdf_write_rec_far(stdf_file*, rec_far*);
extern ssize_t stdf_write_rec_atr(stdf_file*, rec_atr*);
extern ssize_t stdf_write_rec_mir(stdf_file*, rec_mir*);
extern ssize_t stdf_write_rec_mrr(stdf_file*, rec_mrr*);
extern ssize_t stdf_write_rec_pcr(stdf_file*, rec_pcr*);
extern ssize_t stdf_write_rec_hbr(stdf_file*, rec_hbr*);
extern ssize_t stdf_write_rec_sbr(stdf_file*, rec_sbr*);
extern ssize_t stdf_write_rec_pmr(stdf_file*, rec_pmr*);
extern ssize_t stdf_write_rec_pgr(stdf_file*, rec_pgr*);
extern ssize_t stdf_write_rec_plr(stdf_file*, rec_plr*);
extern ssize_t stdf_write_rec_rdr(stdf_file*, rec_rdr*);
extern ssize_t stdf_write_rec_sdr(stdf_file*, rec_sdr*);
extern ssize_t stdf_write_rec_wir(stdf_file*, rec_wir*);
extern ssize_t stdf_write_rec_wrr(stdf_file*, rec_wrr*);
extern ssize_t stdf_write_rec_wcr(stdf_file*, rec_wcr*);
extern ssize_t stdf_write_rec_pir(stdf_file*, rec_pir*);
extern ssize_t stdf_write_rec_prr(stdf_file*, rec_prr*);
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_pdr(stdf_file*, rec_pdr*);
extern ssize_t stdf_write_rec_fdr(stdf_file*, rec_fdr*);
#endif
extern ssize_t stdf_write_rec_tsr(stdf_file*, rec_tsr*);
extern ssize_t stdf_write_rec_ptr(stdf_file*, rec_ptr*);
extern ssize_t stdf_write_rec_mpr(stdf_file*, rec_mpr*);
extern ssize_t stdf_write_rec_ftr(stdf_file*, rec_ftr*);
extern ssize_t stdf_write_rec_bps(stdf_file*, rec_bps*);
extern ssize_t stdf_write_rec_eps(stdf_file*, rec_eps*);
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_shb(stdf_file*, rec_shb*);
extern ssize_t stdf_write_rec_ssb(stdf_file*, rec_ssb*);
extern ssize_t stdf_write_rec_sts(stdf_file*, rec_sts*);
extern ssize_t stdf_write_rec_scr(stdf_file*, rec_scr*);
#endif
extern ssize_t stdf_write_rec_gdr(stdf_file*, rec_gdr*);
extern ssize_t stdf_write_rec_dtr(stdf_file*, rec_dtr*);

ssize_t _stdf_write_flush(stdf_file*, size_t);

#endif /* _LIBSTDF_REC_H */
