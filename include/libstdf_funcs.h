/* libstdf_funcs.h
 * Copyright (C) 2004 Mike Frysinger <vapier@gmail.com>
 * Released under the BSD license.  For more information,
 * please see: http://opensource.org/licenses/bsd-license.php
 */

#ifndef _LIBSTDF_FUNCS_H
#define _LIBSTDF_FUNCS_H

extern stdf_file* stdf_open(char*);
extern stdf_file* stdf_open_ex(char*, int);
extern int stdf_close(stdf_file*);

extern rec_unknown* stdf_read_record(stdf_file*);
extern void stdf_free_record(rec_unknown*);

extern char* stdf_get_rec_name(int,int);

#endif /* _LIBSTDF_FUNCS_H */
