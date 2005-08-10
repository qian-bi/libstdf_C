/**
 * @file libstdf_funcs.h
 * @brief Publicly exported functions.
 */
/*
 * Copyright (C) 2004-2005 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_FUNCS_H
#define _LIBSTDF_FUNCS_H

extern stdf_file* stdf_open(char*, ...);					/**< Open a file with default options */
extern stdf_file* stdf_dopen(int, ...);						/**< Open a file descriptor with default options */
extern int stdf_close(stdf_file*);							/**< Close an open file */

extern rec_unknown* stdf_read_record(stdf_file*);			/**< Read in the next record and parse it */
extern rec_unknown* stdf_read_record_raw(stdf_file*);		/**< Read in the next record raw */
extern rec_unknown* stdf_parse_raw_record(rec_unknown*);	/**< Parse a previously read record */
extern void stdf_free_record(rec_unknown*);					/**< Free a record  */

extern char* stdf_get_rec_name(rec_typ, rec_sub);			/**< Translate a record into the 3 letter spec name */
#define	stdf_get_rec_name_from_head(h) stdf_get_rec_name(h.REC_TYP, h.REC_SUB)
#define	stdf_get_rec_name_from_rec(r) stdf_get_rec_name(r->header.REC_TYP, r->header.REC_SUB)

extern int stdf_rec_to_idx(rec_unknown*);					/**< Normalize the record namespace into array indexes */
extern int stdf_rec_to_idx_count();							/**< Highest array reference the normalize will return */
extern int stdf_rec_name_to_idx(char*);						/**< Normalize the record names into array indexes */

extern char* stdf_get_Vn_name(int);							/**< Translate a Vn type into the 2 letter spec name */
extern void stdf_get_Vn_name_r(int, char*);					/**< Translate a Vn type into the 2 letter spec name (reentrant version) */

#endif /* _LIBSTDF_FUNCS_H */
