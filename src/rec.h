/**
 * @internal
 * @file rec.h
 * @brief Record specific function prototypes.
 *
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
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

#endif /* _LIBSTDF_REC_H */
