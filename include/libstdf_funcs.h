/* libstdf_funcs.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 *
 * $Header$
 */

#ifndef _LIBSTDF_FUNCS_H
#define _LIBSTDF_FUNCS_H

extern stdf_file* stdf_open(char*);
extern stdf_file* stdf_open_ex(char*, int);
extern int stdf_close(stdf_file*);

extern rec_unknown* stdf_read_record(stdf_file*);
extern void stdf_free_record(stdf_file*, rec_unknown*);

extern char* stdf_get_rec_name(int, int);
#define	stdf_get_rec_name_from_head(h) stdf_get_rec_name(h.REC_TYP, h.REC_SUB)
#define	stdf_get_rec_name_from_rec(r) stdf_get_rec_name(r->header.REC_TYP, r->h.REC_SUB)

extern char* stdf_get_Vn_name(int);

#endif /* _LIBSTDF_FUNCS_H */
