/**
 * @file rec.h
 * @brief Record specific function prototypes.
 * @internal
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_REC_H
#define _LIBSTDF_REC_H

#include <libstdf.h>

extern stdf_rec_unknown* stdf_read_rec_unknown(stdf_file*) attribute_hidden;
extern stdf_rec_far* stdf_read_rec_far(stdf_file*) attribute_hidden;
extern stdf_rec_atr* stdf_read_rec_atr(stdf_file*) attribute_hidden;
extern stdf_rec_mir* stdf_read_rec_mir(stdf_file*) attribute_hidden;
extern stdf_rec_mrr* stdf_read_rec_mrr(stdf_file*) attribute_hidden;
extern stdf_rec_pcr* stdf_read_rec_pcr(stdf_file*) attribute_hidden;
extern stdf_rec_hbr* stdf_read_rec_hbr(stdf_file*) attribute_hidden;
extern stdf_rec_sbr* stdf_read_rec_sbr(stdf_file*) attribute_hidden;
extern stdf_rec_pmr* stdf_read_rec_pmr(stdf_file*) attribute_hidden;
extern stdf_rec_pgr* stdf_read_rec_pgr(stdf_file*) attribute_hidden;
extern stdf_rec_plr* stdf_read_rec_plr(stdf_file*) attribute_hidden;
extern stdf_rec_rdr* stdf_read_rec_rdr(stdf_file*) attribute_hidden;
extern stdf_rec_sdr* stdf_read_rec_sdr(stdf_file*) attribute_hidden;
extern stdf_rec_wir* stdf_read_rec_wir(stdf_file*) attribute_hidden;
extern stdf_rec_wrr* stdf_read_rec_wrr(stdf_file*) attribute_hidden;
extern stdf_rec_wcr* stdf_read_rec_wcr(stdf_file*) attribute_hidden;
extern stdf_rec_pir* stdf_read_rec_pir(stdf_file*) attribute_hidden;
extern stdf_rec_prr* stdf_read_rec_prr(stdf_file*) attribute_hidden;
#ifdef STDF_VER3
extern stdf_rec_pdr* stdf_read_rec_pdr(stdf_file*) attribute_hidden;
extern stdf_rec_fdr* stdf_read_rec_fdr(stdf_file*) attribute_hidden;
#endif
extern stdf_rec_tsr* stdf_read_rec_tsr(stdf_file*) attribute_hidden;
extern stdf_rec_ptr* stdf_read_rec_ptr(stdf_file*) attribute_hidden;
extern stdf_rec_mpr* stdf_read_rec_mpr(stdf_file*) attribute_hidden;
extern stdf_rec_ftr* stdf_read_rec_ftr(stdf_file*) attribute_hidden;
extern stdf_rec_bps* stdf_read_rec_bps(stdf_file*) attribute_hidden;
extern stdf_rec_eps* stdf_read_rec_eps(stdf_file*) attribute_hidden;
#ifdef STDF_VER3
extern stdf_rec_shb* stdf_read_rec_shb(stdf_file*) attribute_hidden;
extern stdf_rec_ssb* stdf_read_rec_ssb(stdf_file*) attribute_hidden;
extern stdf_rec_sts* stdf_read_rec_sts(stdf_file*) attribute_hidden;
extern stdf_rec_scr* stdf_read_rec_scr(stdf_file*) attribute_hidden;
#endif
extern stdf_rec_gdr* stdf_read_rec_gdr(stdf_file*) attribute_hidden;
extern stdf_rec_dtr* stdf_read_rec_dtr(stdf_file*) attribute_hidden;

extern ssize_t stdf_write_rec_far(stdf_file*, stdf_rec_far*) attribute_hidden;
extern ssize_t stdf_write_rec_atr(stdf_file*, stdf_rec_atr*) attribute_hidden;
extern ssize_t stdf_write_rec_mir(stdf_file*, stdf_rec_mir*) attribute_hidden;
extern ssize_t stdf_write_rec_mrr(stdf_file*, stdf_rec_mrr*) attribute_hidden;
extern ssize_t stdf_write_rec_pcr(stdf_file*, stdf_rec_pcr*) attribute_hidden;
extern ssize_t stdf_write_rec_hbr(stdf_file*, stdf_rec_hbr*) attribute_hidden;
extern ssize_t stdf_write_rec_sbr(stdf_file*, stdf_rec_sbr*) attribute_hidden;
extern ssize_t stdf_write_rec_pmr(stdf_file*, stdf_rec_pmr*) attribute_hidden;
extern ssize_t stdf_write_rec_pgr(stdf_file*, stdf_rec_pgr*) attribute_hidden;
extern ssize_t stdf_write_rec_plr(stdf_file*, stdf_rec_plr*) attribute_hidden;
extern ssize_t stdf_write_rec_rdr(stdf_file*, stdf_rec_rdr*) attribute_hidden;
extern ssize_t stdf_write_rec_sdr(stdf_file*, stdf_rec_sdr*) attribute_hidden;
extern ssize_t stdf_write_rec_wir(stdf_file*, stdf_rec_wir*) attribute_hidden;
extern ssize_t stdf_write_rec_wrr(stdf_file*, stdf_rec_wrr*) attribute_hidden;
extern ssize_t stdf_write_rec_wcr(stdf_file*, stdf_rec_wcr*) attribute_hidden;
extern ssize_t stdf_write_rec_pir(stdf_file*, stdf_rec_pir*) attribute_hidden;
extern ssize_t stdf_write_rec_prr(stdf_file*, stdf_rec_prr*) attribute_hidden;
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_pdr(stdf_file*, stdf_rec_pdr*) attribute_hidden;
extern ssize_t stdf_write_rec_fdr(stdf_file*, stdf_rec_fdr*) attribute_hidden;
#endif
extern ssize_t stdf_write_rec_tsr(stdf_file*, stdf_rec_tsr*) attribute_hidden;
extern ssize_t stdf_write_rec_ptr(stdf_file*, stdf_rec_ptr*) attribute_hidden;
extern ssize_t stdf_write_rec_mpr(stdf_file*, stdf_rec_mpr*) attribute_hidden;
extern ssize_t stdf_write_rec_ftr(stdf_file*, stdf_rec_ftr*) attribute_hidden;
extern ssize_t stdf_write_rec_bps(stdf_file*, stdf_rec_bps*) attribute_hidden;
extern ssize_t stdf_write_rec_eps(stdf_file*, stdf_rec_eps*) attribute_hidden;
#ifdef STDF_VER3
extern ssize_t stdf_write_rec_shb(stdf_file*, stdf_rec_shb*) attribute_hidden;
extern ssize_t stdf_write_rec_ssb(stdf_file*, stdf_rec_ssb*) attribute_hidden;
extern ssize_t stdf_write_rec_sts(stdf_file*, stdf_rec_sts*) attribute_hidden;
extern ssize_t stdf_write_rec_scr(stdf_file*, stdf_rec_scr*) attribute_hidden;
#endif
extern ssize_t stdf_write_rec_gdr(stdf_file*, stdf_rec_gdr*) attribute_hidden;
extern ssize_t stdf_write_rec_dtr(stdf_file*, stdf_rec_dtr*) attribute_hidden;

ssize_t _stdf_write_flush(stdf_file*, size_t) attribute_hidden;

#endif /* _LIBSTDF_REC_H */
