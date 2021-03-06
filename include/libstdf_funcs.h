/**
 * @file libstdf_funcs.h
 * @brief Publicly exported functions.
 */
/*
 * Copyright (C) 2004-2007 Mike Frysinger <vapier@gmail.com>
 * Copyright (C) 2017 Stefan Brandner <stefan.brandner@gmx.at>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_FUNCS_H
#define _LIBSTDF_FUNCS_H

extern stdf_file* stdf_open(char*);							/**< Open a file with default options */
extern stdf_file* stdf_open_ex(char*, uint32_t, ...);		/**< Open a file with custom options */
extern stdf_file* stdf_dopen(int);							/**< Open a file descriptor with default options */
extern stdf_file* stdf_dopen_ex(int, uint32_t, ...);		/**< Open a file descriptor with custom options */
extern int stdf_close(stdf_file*);							/**< Close an open file */

extern int stdf_set_setting(stdf_file*, uint32_t, ...);
extern void stdf_get_setting(stdf_file*, uint32_t, ...);

extern stdf_rec_unknown* stdf_read_record(stdf_file*);			/**< Read in the next record and parse it */
extern stdf_rec_unknown* stdf_read_record_raw(stdf_file*);		/**< Read in the next record raw */
extern stdf_rec_unknown* stdf_parse_raw_record(stdf_rec_unknown*);	/**< Parse a previously read record */
extern void stdf_free_record(stdf_rec_unknown*);					/**< Free a record */

extern ssize_t stdf_write_record(stdf_file*, void*);		        /**< Write out a record */
extern ssize_t stdf_write_record_raw(stdf_file*, stdf_rec_unknown*);	/**< Write out a raw record */

#define stdf_init_header(header, type) INIT_HEADER(header, type)

extern char* stdf_get_rec_name(stdf_rec_typ, stdf_rec_sub);			/**< Translate a record into the 3 letter spec name */
#define	stdf_get_rec_name_from_head(h) stdf_get_rec_name(h.REC_TYP, h.REC_SUB)
#define	stdf_get_rec_name_from_rec(r) stdf_get_rec_name(r->header.REC_TYP, r->header.REC_SUB)

extern int stdf_rec_to_idx(void*) stdf_attribute_const;		/**< Normalize the record namespace into array indexes */
extern int stdf_rec_to_idx_count(void) stdf_attribute_const;/**< Highest array reference the normalize will return */
extern int stdf_rec_name_to_idx(char*) stdf_attribute_const;/**< Normalize the record names into array indexes */

extern char* stdf_get_Vn_name(int) stdf_attribute_const;			/**< Translate a Vn type into the 2 letter spec name */
extern void stdf_get_Vn_name_r(int, char*) stdf_attribute_const;	/**< Translate a Vn type into the 2 letter spec name (reentrant version) */

#endif /* _LIBSTDF_FUNCS_H */
