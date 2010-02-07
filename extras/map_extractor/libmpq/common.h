/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2010 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the AscentNG Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the AscentNG is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#define LIBMPQ_CONF_FL_INCREMENT	512			/* i hope we did not need more :) */
#define LIBMPQ_CONF_EXT			".conf"			/* listdb file seems to be valid with this extension */
#define LIBMPQ_CONF_HEADER		"LIBMPQ_VERSION"	/* listdb file must include this entry to be valid */
#define LIBMPQ_CONF_BUFSIZE		4096			/* maximum number of bytes a line in the file could contain */

#define LIBMPQ_CONF_TYPE_CHAR		1			/* value in config file is from type char */
#define LIBMPQ_CONF_TYPE_INT		2			/* value in config file is from type int */

#define LIBMPQ_CONF_EOPEN_DIR		-1			/* error on open directory */
#define LIBMPQ_CONF_EVALUE_NOT_FOUND	-2			/* value for the option was not found */

int libmpq_init_buffer(mpq_archive *mpq_a);
int libmpq_read_hashtable(mpq_archive *mpq_a);
int libmpq_read_blocktable(mpq_archive *mpq_a);
int libmpq_file_read_file(mpq_archive *mpq_a, mpq_file *mpq_f, unsigned int filepos, char *buffer, unsigned int toread);
int libmpq_read_listfile(mpq_archive *mpq_a, FILE *fp);

int libmpq_conf_get_value(FILE *fp, char *search_value, void *return_value, int type, int size);
char *libmpq_conf_delete_char(char *buf, char *chars);
int libmpq_conf_get_array(FILE *fp, char *search_value, char ***filelist, int *entries);
int libmpq_free_listfile(char **filelist);
int libmpq_read_listfile(mpq_archive *mpq_a, FILE *fp);
