/**
 * @file libstdf_funcs.h
 * @brief Publicly exported functions.
 *
 */
/*
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_FUNCS_H
#define _LIBSTDF_FUNCS_H

extern stdf_file* stdf_open(char*);							/**< Open a file with default options */
extern stdf_file* stdf_open_ex(char*, uint32_t);			/**< Open a file with custom options */
extern int stdf_close(stdf_file*);							/**< Close an open file */

extern rec_unknown* stdf_read_record(stdf_file*);			/**< Read in the next record and parse it */
extern rec_unknown* stdf_read_record_raw(stdf_file*);		/**< Read in the next record raw */
extern rec_unknown* stdf_parse_raw_record(rec_unknown*);	/**< Parse a previously read record */
extern void stdf_free_record(rec_unknown*);					/**< Free a record  */

extern char* stdf_get_rec_name(rec_typ, rec_sub);			/**< Translate a record into the 3 letter spec name */
#define	stdf_get_rec_name_from_head(h) stdf_get_rec_name(h.REC_TYP, h.REC_SUB)
#define	stdf_get_rec_name_from_rec(r) stdf_get_rec_name(r->header.REC_TYP, r->h.REC_SUB)

extern char* stdf_get_Vn_name(int);							/**< Translate a Vn type into the 2 letter spec name */

#endif /* _LIBSTDF_FUNCS_H */
