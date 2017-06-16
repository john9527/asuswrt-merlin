/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/* milli_httpd - pretty small HTTP server
** A combination of
** micro_httpd - really small HTTP server
** and
** mini_httpd - small HTTP server
**
** Copyright ?1999,2000 by Jef Poskanzer <jef@acme.com>.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
//#include <sys/stat.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <ctype.h>

typedef unsigned int __u32;   // 1225 ham

#include <httpd.h>
//2008.08 magic{
#include <bcmnvram.h>	//2008.08 magic
#include <arpa/inet.h>	//2008.08 magic

#include <error.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <shared.h>
#include <push_log.h>

#define ETCPHYRD	14
#define SIOCGETCPHYRD   0x89FE
//#include "etioctl.h"

#define SERVER_NAME "httpd"
#define SERVER_PORT 80
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

#ifdef RTCONFIG_HTTPS
#include <syslog.h>
#include <mssl.h>
#include <shutils.h>
#define SERVER_PORT_SSL	443
#endif
#include "bcmnvram_f.h"

#ifdef RTCONFIG_QTN
#include "web-qtn.h"
#endif

/* A multi-family sockaddr. */
typedef union {
    struct sockaddr sa;
    struct sockaddr_in sa_in;
    } usockaddr;

#include "queue.h"
#define MAX_CONN_ACCEPT 64
#define MAX_CONN_TIMEOUT 60
#define MAX_DISC_TIMEOUT 15

typedef struct conn_item {
	TAILQ_ENTRY(conn_item) entry;
	int fd;
	usockaddr usa;
} conn_item_t;

typedef struct conn_list {
	TAILQ_HEAD(, conn_item) head;
	int count;
} conn_list_t;

/* Globals. */
static FILE *conn_fp;
static char auth_userid[AUTH_MAX];
static char auth_passwd[AUTH_MAX];
static char auth_realm[AUTH_MAX];
char host_name[64];
char referer_host[64];
char user_agent[1024];

#ifdef TRANSLATE_ON_FLY
char Accept_Language[16];

struct language_table language_tables[] = {
	{"br", "BR"},
	{"pt-BR", "BR"},
	{"zh-cn", "CN"},
	{"zh-Hans-CN", "CN"},
	{"cs", "CZ"},
	{"cs-cz", "CZ"},
	{"da", "DA"},
	{"da-DK", "DA"},
	{"de", "DE"},
	{"de-at", "DE"},
	{"de-ch", "DE"},
	{"de-de", "DE"},
	{"de-li", "DE"},
	{"de-lu", "DE"},
	{"en", "EN"},
	{"en-us", "EN"},
	{"es", "ES"},
	{"es-ec", "ES"},
	{"es-py", "ES"},
	{"es-pa", "ES"},
	{"es-ni", "ES"},
	{"es-gt", "ES"},
	{"es-do", "ES"},
	{"es-es", "ES"},
	{"es-hn", "ES"},
	{"es-ve", "ES"},
	{"es-pr", "ES"},
	{"es-ar", "ES"},
	{"es-bo", "ES"},
	{"es-us", "ES"},
	{"es-co", "ES"},
	{"es-cr", "ES"},
	{"es-uy", "ES"},
	{"es-pe", "ES"},
	{"es-cl", "ES"},
	{"es-mx", "ES"},
	{"es-sv", "ES"},
	{"fi", "FI"},
	{"fi-FI", "FI"},
	{"fr", "FR"},
	{"fr-fr", "FR"},
	{"hu-hu", "HU"},
	{"hu", "HU"},
	{"it", "IT"},
	{"it-it", "IT"},
	{"it-ch", "IT"},
	{"ja", "JP"},
	{"ja-JP", "JP"},
	{"ms", "MS"},
	{"ms-MY", "MS"},
	{"ms-BN", "MS"},
	{"no", "NO"},
	{"nb", "NO"},
	{"nn", "NO"},
	{"nb-NO", "NO"},
	{"nn-NO", "NO"},
	{"pl-pl", "PL"},
	{"pl", "PL"},
	{"ru", "RU"},
	{"ru-ru", "RU"},
	{"ro", "RO"},
	{"ro-ro", "RO"},
	{"sv", "SV"},
	{"sv-FI", "SV"},
	{"sv-SE", "SV"},
	{"th", "TH"},
	{"th-TH", "TH"},
	{"th-TH-TH", "TH"},
	{"tr", "TR"},
	{"tr-TR", "TR"},
	{"zh", "TW"},
	{"zh-tw", "TW"},
	{"zh-Hant-TW", "TW"},
	{"zh-hk", "TW"},
	{"uk", "UK"},
	{NULL, NULL}
};

#endif //TRANSLATE_ON_FLY

/* Forwards. */
static int initialize_listen_socket( usockaddr* usaP );
static int auth_check( char* dirname, char* authorization, char* url);
static int referer_check(char* referer, int fromapp_flag);
static int check_noauth_referrer(char* referer, int fromapp_flag);
static char *get_referrer(char *referer);
static void __send_authenticate( char* realm );
static void send_authenticate( char* realm );
static void send_error( int status, char* title, char* extra_header, char* text );
//#ifdef RTCONFIG_CLOUDSYNC
static void send_page( int status, char* title, char* extra_header, char* text );
//#endif
static void send_headers( int status, char* title, char* extra_header, char* mime_type );
static int b64_decode( const char* str, unsigned char* space, int size );
static int match( const char* pattern, const char* string );
static int match_one( const char* pattern, int patternlen, const char* string );
static void handle_request(void);
int check_user_agent(char* user_agent);

/* added by Joey */
//2008.08 magic{
//int redirect = 1;
int redirect = 0;
int change_passwd = 0;
int reget_passwd = 0;
int x_Setting = 0;
int skip_auth = 0;
int isLogout = 0;
char url[128];
char cloud_file[128];
int http_port;

/* Added by Joey for handle one people at the same time */
unsigned int login_ip=0; // the logined ip
time_t login_timestamp=0; // the timestamp of the logined ip
time_t login_timestamp_tmp=0; // the timestamp of the current session.
time_t last_login_timestamp=0; // the timestamp of the current session.
unsigned int login_ip_tmp=0; // the ip of the current session.
unsigned int login_try=0;
unsigned int last_login_ip = 0;	// the last logined ip 2008.08 magic
/* limit login IP addr; 2012.03 Yau */
unsigned int access_ip[4];
unsigned int MAX_login;

// 2008.08 magic {
time_t request_timestamp = 0;
time_t turn_off_auth_timestamp = 0;
int temp_turn_off_auth = 0;	// for QISxxx.htm pages

/* Const vars */
const int int_1 = 1;

void http_login(unsigned int ip, char *url);
void http_login_timeout(unsigned int ip);
void http_logout(unsigned int ip);
int http_login_check(void);

#if 0
static int check_if_inviteCode(const char *dirpath){
	return 1;
}
#endif
void sethost(char *host)
{
	char *cp;

	if(!host) return;

	memset(host_name, 0, sizeof(host_name));
	strncpy(host_name, host, sizeof(host_name));

	for ( cp = host_name; *cp && *cp != '\r' && *cp != '\n'; cp++ );
	*cp = '\0';
}

char *gethost(void)
{
	if(strlen(host_name)) {
		return host_name;
	}
	else return(nvram_safe_get("lan_ipaddr"));
}

#include <sys/sysinfo.h>
long uptime(void)
{
	struct sysinfo info;
	sysinfo(&info);

	return info.uptime;
}

static int
initialize_listen_socket( usockaddr* usaP )
    {
    int listen_fd;

    memset( usaP, 0, sizeof(usockaddr) );
    usaP->sa.sa_family = AF_INET;
//    usaP->sa_in.sin_addr.s_addr = htonl( INADDR_ANY );
    usaP->sa_in.sin_addr.s_addr =  (inet_addr(nvram_safe_get("lan_ipaddr")) != INADDR_NONE) ? inet_addr(nvram_safe_get("lan_ipaddr")) : htonl( INADDR_ANY );
    usaP->sa_in.sin_port = htons( http_port );

    listen_fd = socket( usaP->sa.sa_family, SOCK_STREAM, 0 );
    if ( listen_fd < 0 )
	{
	perror( "socket" );
	return -1;
	}
    (void) fcntl( listen_fd, F_SETFD, FD_CLOEXEC );
    if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, &int_1, sizeof(int_1) ) < 0 )
	{
	close(listen_fd);	// 1104 chk
	perror( "setsockopt" );
	return -1;
	}
    if ( bind( listen_fd, &usaP->sa, sizeof(struct sockaddr_in) ) < 0 )
	{
	close(listen_fd);	// 1104 chk
	perror( "bind" );
	return -1;
	}
    if ( listen( listen_fd, 1024 ) < 0 )
	{
	close(listen_fd);	// 1104 chk
	perror( "listen" );
	return -1;
	}
    return listen_fd;
    }

static char
*get_referrer(char *referer)
{
	char *auth_referer=NULL;
	char *cp1=NULL, *cp2=NULL, *location_cp=NULL, *location_cp1=NULL;

	if(strstr(referer,"\r") != (char*) 0)
		location_cp1 = strtok(referer, "\r");
	else
		location_cp1 = referer;

	location_cp = strstr(location_cp1,"//");
	if(location_cp != (char*) 0){
		cp1 = &location_cp[2];
		if(strstr(cp1,"/") != (char*) 0){
			cp2 = strtok(cp1, "/");
			auth_referer = cp2;
		}else
			auth_referer = cp1;
	}else
		auth_referer = location_cp1;

	return auth_referer;
}

static int
check_noauth_referrer(char* referer, int fromapp_flag)
{
	char *auth_referer=NULL;

	if(fromapp_flag != 0)
		return 0;

	if(!referer || !strlen(host_name)){
		return NOREFERER;
	}else{
		auth_referer = get_referrer(referer);
	}

	if(!strcmp(host_name, auth_referer))
		return 0;
	else
		return REFERERFAIL;
}

static int
referer_check(char* referer, int fromapp_flag)
{
	char *auth_referer=NULL;
	const int d_len = strlen(DUT_DOMAIN_NAME);
	int port = 0;
	int referer_from_https = 0;
	int referer_host_check = 0;

	if(fromapp_flag != 0)
		return 0;
	if(!referer){
		return NOREFERER;
	}else{
		auth_referer = get_referrer(referer);
	}

	if(referer_host[0] == 0){
		return WEB_NOREFERER;
	}

	if(!strcmp(host_name, auth_referer)) referer_host_check = 1;

	if (*(auth_referer + d_len) == ':' && (port = atoi(auth_referer + d_len + 1)) > 0 && port < 65536)
		referer_from_https = 1;

	if (((strlen(auth_referer) == d_len) || (*(auth_referer + d_len) == ':' && atoi(auth_referer + d_len + 1) > 0))
	   && strncmp(DUT_DOMAIN_NAME, auth_referer, d_len)==0){
		if(referer_from_https)
			snprintf(auth_referer,sizeof(referer_host),"%s:%d",nvram_safe_get("lan_ipaddr"), port);
		else
			snprintf(auth_referer,sizeof(referer_host),"%s",nvram_safe_get("lan_ipaddr"));
	}

	/* form based referer info? */
	if(referer_host_check && (strlen(auth_referer) == strlen(referer_host)) && strncmp( auth_referer, referer_host, strlen(referer_host) ) == 0){
		//_dprintf("asus token referer_check: the right user and password\n");
		return 0;
	}else{
		//_dprintf("asus token referer_check: the wrong user and password\n");
		return REFERERFAIL;
	}
	return REFERERFAIL;
}

static int
auth_check( char* dirname, char* authorization ,char* url)
{
	char authinfo[500];
	char* authpass;
	int l;
	struct in_addr temp_ip_addr;
	char *temp_ip_str;
	time_t dt;

	// Get ip addr
	temp_ip_addr.s_addr = login_ip_tmp;
	temp_ip_str = inet_ntoa(temp_ip_addr);

	if(isLogout == 1){
		isLogout = 0;
		send_authenticate( dirname );
		// Send login msg to syslog
		//logmessage(HEAD_HTTP_LOGIN, "login user %s", temp_ip_str);
		return 0;
	}

	login_timestamp_tmp = uptime();
	dt = login_timestamp_tmp - last_login_timestamp;
	if(last_login_timestamp != 0 && dt > MAX_CONN_TIMEOUT){
		login_try = 0;
		last_login_timestamp = 0;
	}

	if (MAX_login <= DEFAULT_LOGIN_MAX_NUM)
		MAX_login = DEFAULT_LOGIN_MAX_NUM;
	if(login_try >= MAX_login){

		if(login_try%MAX_login == 0)
			logmessage(HEAD_HTTP_LOGIN, "Detect abnormal logins at %d times. The newest one was from %s.", login_try, temp_ip_str);

#ifdef LOGIN_LOCK
		send_authenticate( dirname );
		return 0;
#endif
	}

	//printf("[httpd] auth chk:%s, %s\n", dirname, url);	// tmp test
	/* Is this directory unprotected? */
	if ( !strlen(auth_passwd) )
		/* Yes, let the request go through. */
		return 1;

	/* Basic authorization info? */
	if ( !authorization || strncmp( authorization, "Basic ", 6 ) != 0)
	{
		__send_authenticate( dirname );
		return 0;
	}

	/* Decode it. */
	l = b64_decode( &(authorization[6]), (unsigned char*) authinfo, sizeof(authinfo) );
	authinfo[l] = '\0';
	/* Split into user and password. */
	authpass = strchr( authinfo, ':' );
	if ( authpass == (char*) 0 ) {
		/* No colon?  Bogus auth info. */
		send_authenticate( dirname );
		return 0;
	}
	*authpass++ = '\0';

	/* Is this the right user and password? */
	if ( strcmp( auth_userid, authinfo ) == 0 && strcmp( auth_passwd, authpass ) == 0)
	{
		//fprintf(stderr, "login check : %x %x\n", login_ip, last_login_ip);
		/* Is this is the first login after logout */
		//if (login_ip==0 && last_login_ip==login_ip_tmp)
		//{
		//	send_authenticate(dirname);
		//	last_login_ip=0;
		//	return 0;
		//}
		if ( login_ip == 0 || last_login_ip != 0 || login_ip != login_ip_tmp ) {
			// Send login msg to syslog
			logmessage(HEAD_HTTP_LOGIN, "login '%s' successful from %s:%d", authinfo, temp_ip_str, http_port);
		}
		login_try = 0;
                last_login_timestamp = 0;
		last_login_ip = 0;
		set_referer_host();
		return 1;
	}
	else
	{
		// Failed login msg to syslog
		logmessage(HEAD_HTTP_LOGIN, "login '%s' failed from %s:%d", authinfo, temp_ip_str, http_port);
	}

	send_authenticate( dirname );
	return 0;
}

static void
__send_authenticate( char* realm )
{
	char header[10000];

	(void) snprintf(header, sizeof(header), "WWW-Authenticate: Basic realm=\"%s\"", realm);
	send_error( 401, "Unauthorized", header, "Authorization required." );
}

static void
send_authenticate( char* realm )
{
	login_try++;
	last_login_timestamp = login_timestamp_tmp;

	__send_authenticate(realm);
}

static void
send_error( int status, char* title, char* extra_header, char* text )
{
	send_headers( status, title, extra_header, "text/html" );
	(void) fprintf( conn_fp, "<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\n<BODY BGCOLOR=\"#cc9999\"><H4>%d %s</H4>\n", status, title, status, title );
	(void) fprintf( conn_fp, "%s\n", text );
	(void) fprintf( conn_fp, "</BODY></HTML>\n" );
	(void) fflush( conn_fp );
}

//#ifdef RTCONFIG_CLOUDSYNC
static void
send_page( int status, char* title, char* extra_header, char* text ){
    send_headers( status, title, extra_header, "text/html" );
    (void) fprintf( conn_fp, "<HTML><HEAD>");
    (void) fprintf( conn_fp, "%s\n", text );
    (void) fprintf( conn_fp, "</HEAD></HTML>\n" );
    (void) fflush( conn_fp );
}
//#endif

static void
send_headers( int status, char* title, char* extra_header, char* mime_type )
    {
    time_t now;
    char timebuf[100];

    (void) fprintf( conn_fp, "%s %d %s\r\n", PROTOCOL, status, title );
    (void) fprintf( conn_fp, "Server: %s\r\n", SERVER_NAME );
    (void) fprintf( conn_fp, "x-frame-options: SAMEORIGIN\r\n");
    (void) fprintf( conn_fp, "x-xss-protection: 1; mode=block\r\n");
    now = time( (time_t*) 0 );
    (void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
    (void) fprintf( conn_fp, "Date: %s\r\n", timebuf );
    if ( extra_header != (char*) 0 )
	(void) fprintf( conn_fp, "%s\r\n", extra_header );
    if ( mime_type != (char*) 0 )
	(void) fprintf( conn_fp, "Content-Type: %s\r\n", mime_type );

    (void) fprintf( conn_fp, "Connection: close\r\n" );
    (void) fprintf( conn_fp, "\r\n" );
    }


/* Base-64 decoding.  This represents binary data as printable ASCII
** characters.  Three 8-bit binary bytes are turned into four 6-bit
** values, like so:
**
**   [11111111]  [22222222]  [33333333]
**
**   [111111] [112222] [222233] [333333]
**
** Then the 6-bit values are represented using the characters "A-Za-z0-9+/".
*/

static int b64_decode_table[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
    };

/* Do base-64 decoding on a string.  Ignore any non-base64 bytes.
** Return the actual number of bytes generated.  The decoded size will
** be at most 3/4 the size of the encoded, and may be smaller if there
** are padding characters (blanks, newlines).
*/
static int
b64_decode( const char* str, unsigned char* space, int size )
    {
    const char* cp;
    int space_idx, phase;
    int d, prev_d=0;
    unsigned char c;

    space_idx = 0;
    phase = 0;
    for ( cp = str; *cp != '\0'; ++cp )
	{
	d = b64_decode_table[(int)*cp];
	if ( d != -1 )
	    {
	    switch ( phase )
		{
		case 0:
		++phase;
		break;
		case 1:
		c = ( ( prev_d << 2 ) | ( ( d & 0x30 ) >> 4 ) );
		if ( space_idx < size )
		    space[space_idx++] = c;
		++phase;
		break;
		case 2:
		c = ( ( ( prev_d & 0xf ) << 4 ) | ( ( d & 0x3c ) >> 2 ) );
		if ( space_idx < size )
		    space[space_idx++] = c;
		++phase;
		break;
		case 3:
		c = ( ( ( prev_d & 0x03 ) << 6 ) | d );
		if ( space_idx < size )
		    space[space_idx++] = c;
		phase = 0;
		break;
		}
	    prev_d = d;
	    }
	}
    return space_idx;
    }


/* Simple shell-style filename matcher.  Only does ? * and **, and multiple
** patterns separated by |.  Returns 1 or 0.
*/
int
match( const char* pattern, const char* string )
    {
    const char* or;

    for (;;)
	{
	or = strchr( pattern, '|' );
	if ( or == (char*) 0 )
	    return match_one( pattern, strlen( pattern ), string );
	if ( match_one( pattern, or - pattern, string ) )
	    return 1;
	pattern = or + 1;
	}
    }


static int
match_one( const char* pattern, int patternlen, const char* string )
    {
    const char* p;

    for ( p = pattern; p - pattern < patternlen; ++p, ++string )
	{
	if ( *p == '?' && *string != '\0' )
	    continue;
	if ( *p == '*' )
	    {
	    int i, pl;
	    ++p;
	    if ( *p == '*' )
		{
		/* Double-wildcard matches anything. */
		++p;
		i = strlen( string );
		}
	    else
		/* Single-wildcard matches anything but slash. */
		i = strcspn( string, "/" );
	    pl = patternlen - ( p - pattern );
	    for ( ; i >= 0; --i )
		if ( match_one( p, pl, &(string[i]) ) )
		    return 1;
	    return 0;
	    }
	if ( *p != *string )
	    return 0;
	}
    if ( *string == '\0' )
	return 1;
    return 0;
}

int web_write(const char *buffer, int len, FILE *stream)
{
	int n = len;
	int r = 0;

	while (n > 0) {
		r = fwrite(buffer, 1, n, stream);
		if (( r == 0) && (errno != EINTR)) return -1;
		buffer += r;
		n -= r;
	}
	return r;
}

int check_user_agent(char* user_agent){

	int fromapp = 0;

	if(user_agent != NULL){
		char *cp1=NULL, *app_router=NULL, *app_platform=NULL, *app_framework=NULL, *app_verison=NULL;
		cp1 = strdup(user_agent);

		vstrsep(cp1, "-", &app_router, &app_platform, &app_framework, &app_verison);

		if(app_router != NULL && app_framework != NULL && strcmp( app_router, "asusrouter") == 0){
				fromapp=FROM_ASUSROUTER;
			if(strcmp( app_framework, "DUTUtil") == 0)
				fromapp=FROM_DUTUtil;
			else if(strcmp( app_framework, "ASSIA") == 0)
				fromapp=FROM_ASSIA;
			else if(strcmp( app_framework, "IFTTT") == 0)
				fromapp=FROM_IFTTT;
		}
		if(cp1) free(cp1);
	}
	return fromapp;
}

#if 0
void
do_file(char *path, FILE *stream)
{
	FILE *fp;
	char buf[1024];
	int nr;

	if (!(fp = fopen(path, "r")))
		return;
	while ((nr = fread(buf, 1, sizeof(buf), fp)) > 0) {
		web_write(buf, nr, stream);
	}
	fclose(fp);
}
#else
int do_fwrite(const char *buffer, int len, FILE *stream)
{
	int n = len;
	int r = 0;

	while (n > 0) {
		r = fwrite(buffer, 1, n, stream);
		if ((r == 0) && (errno != EINTR)) return -1;
		buffer += r;
		n -= r;
	}

	return r;
}

void do_file(char *path, FILE *stream)
{
	FILE *fp;
	char buf[1024];
	int nr;

	if ((fp = fopen(path, "r")) != NULL) {
		while ((nr = fread(buf, 1, sizeof(buf), fp)) > 0)
			do_fwrite(buf, nr, stream);
		fclose(fp);
	}
}

#endif

void set_referer_host(void)
{
	const int d_len = strlen(DUT_DOMAIN_NAME);
	int port = 0;
	int referer_includes_port = 0;

	memset(referer_host, 0, sizeof(referer_host));
	if (*(host_name + d_len) == ':' && (port = atoi(host_name + d_len + 1)) > 0 && port < 65536){
		referer_includes_port = 1;
	}
	if (((strlen(host_name) == d_len) || (*(host_name + d_len) == ':' && atoi(host_name + d_len + 1) > 0))
	   && strncmp(DUT_DOMAIN_NAME, host_name, d_len)==0){
		if(referer_includes_port)
			snprintf(referer_host,sizeof(referer_host),"%s:%d",nvram_safe_get("lan_ipaddr"), port);
		else
			snprintf(referer_host,sizeof(referer_host),"%s",nvram_safe_get("lan_ipaddr"));
	}
	else
		snprintf(referer_host,sizeof(host_name),"%s",host_name);
}

int is_firsttime(void);

time_t detect_timestamp, detect_timestamp_old, signal_timestamp;
char detect_timestampstr[32];


#define APPLYAPPSTR 	"applyapp.cgi"
#define GETAPPSTR 	"getapp"


static void
handle_request(void)
{
	char line[10000], *cur;
	char *method, *path, *protocol, *authorization, *boundary, *alang, *referer, *useragent;
	char *cp;
	char *file;
	int len;
	struct mime_handler *handler;
	struct except_mime_handler *exhandler;
	struct mime_referer *doreferer;
	int mime_exception, do_referer, login_state = -1;
	int fromapp=0;
	int referer_result = 1;
	int cl = 0, flags;

	/* Initialize the request variables. */
	authorization = boundary = referer = useragent = NULL;
	host_name[0] = 0;
	bzero( line, sizeof line );

	/* Parse the first line of the request. */
	if ( fgets( line, sizeof(line), conn_fp ) == (char*) 0 ) {
		send_error( 400, "Bad Request", (char*) 0, "No request found." );
		return;
	}

	method = path = line;
	strsep(&path, " ");
	//while (*path == ' ') path++;
	while (path && *path == ' ') path++;	// oleg patch
	protocol = path;
	strsep(&protocol, " ");
	//while (*protocol == ' ') protocol++;
	while (protocol && *protocol == ' ') protocol++;    // oleg pat
	cp = protocol;
	strsep(&cp, " ");
	if ( !method || !path || !protocol ) {
		send_error( 400, "Bad Request", (char*) 0, "Can't parse request." );
		return;
	}
	cur = protocol + strlen(protocol) + 1;

#ifdef TRANSLATE_ON_FLY
	memset(Accept_Language, 0, sizeof(Accept_Language));
#endif

	/* Parse the rest of the request headers. */
	while ( fgets( cur, line + sizeof(line) - cur, conn_fp ) != (char*) 0 )
	{
		if ( strcmp( cur, "\n" ) == 0 || strcmp( cur, "\r\n" ) == 0 ) {
			break;
		}
#ifdef TRANSLATE_ON_FLY
		else if ( strncasecmp( cur, "Accept-Language:",16) ==0) {
			char *p;
			struct language_table *pLang;
			char lang_buf[256];
			memset(lang_buf, 0, sizeof(lang_buf));
			alang = &cur[16];
			strncpy(lang_buf, alang, sizeof(lang_buf)-1);
			p = lang_buf;
			while (p != NULL)
			{
				p = strtok (p, "\r\n ,;");
				if (p == NULL)  break;
				//2008.11 magic{
				int i, len=strlen(p);

				for (i=0;i<len;++i)
					if (isupper(p[i])) {
						p[i]=tolower(p[i]);
					}

				//2008.11 magic}
				for (pLang = language_tables; pLang->Lang != NULL; ++pLang)
				{
					if (strcasecmp(p, pLang->Lang)==0)
					{
						snprintf(Accept_Language,sizeof(Accept_Language),"%s",pLang->Target_Lang);
						if (is_firsttime ())    {
							nvram_set("preferred_lang", Accept_Language);
						}
						break;
					}
				}

				if (Accept_Language[0] != 0) {
					break;
				}
				p+=strlen(p)+1;
			}

			if (Accept_Language[0] == 0)    {
				// If all language setting of user's browser are not supported, use English.
				//printf ("Auto detect language failed. Use English.\n");
				strcpy (Accept_Language, "EN");

				// 2008.10 magic {
				if (is_firsttime())
					nvram_set("preferred_lang", "EN");
				// 2008.10 magic }
			}
		}
#endif
		else if ( strncasecmp( cur, "Authorization:", 14 ) == 0 )
		{
			cp = &cur[14];
			cp += strspn( cp, " \t" );
			authorization = cp;
			cur = cp + strlen(cp) + 1;
		}
		else if ( strncasecmp( cur, "User-Agent:", 11 ) == 0 )
		{
			cp = &cur[11];
			cp += strspn( cp, " \t" );
			useragent = cp;
			cur = cp + strlen(cp) + 1;
			//_dprintf("httpd user-agent = %s\n", useragent);
		}
		else if ( strncasecmp( cur, "Referer:", 8 ) == 0 )
		{
			cp = &cur[8];
			cp += strspn( cp, " \t" );
			referer = cp;
			cur = cp + strlen(cp) + 1;
			_dprintf("httpd referer = %s\n", referer);
		}
		else if ( strncasecmp( cur, "Host:", 5 ) == 0 )
		{
			cp = &cur[5];
			cp += strspn( cp, " \t" );
			sethost(cp);
			cur = cp + strlen(cp) + 1;
			_dprintf("httpd host = %s\n", host_name);
		}
		else if (strncasecmp( cur, "Content-Length:", 15 ) == 0) {
			cp = &cur[15];
			cp += strspn( cp, " \t" );
			cl = strtoul( cp, NULL, 0 );
		}
		else if ((cp = strstr( cur, "boundary=" ))) {
			boundary = &cp[9];
			for ( cp = cp + 9; *cp && *cp != '\r' && *cp != '\n'; cp++ );
			*cp = '\0';
			cur = ++cp;
		}
	}

	if ( strcasecmp( method, "get" ) != 0 && strcasecmp(method, "post") != 0 && strcasecmp(method, "head") != 0 ) {
		send_error( 501, "Not Implemented", (char*) 0, "That method is not implemented." );
		return;
	}

	if ( path[0] != '/' ) {
		send_error( 400, "Bad Request", (char*) 0, "Bad filename." );
		return;
	}
	file = &(path[1]);
	len = strlen( file );
	if ( file[0] == '/' || strcmp( file, ".." ) == 0 || strncmp( file, "../", 3 ) == 0 || strstr( file, "/../" ) != (char*) 0 || strcmp( &(file[len-3]), "/.." ) == 0 ) {
		send_error( 400, "Bad Request", (char*) 0, "Illegal filename." );
		return;
	}

//2008.08 magic{
	if (file[0] == '\0' || file[len-1] == '/')
		file = "index.asp";

// 2007.11 James. {
	char *query;
	int file_len;

	memset(url, 0, 128);
	if ((query = index(file, '?')) != NULL) {
		file_len = strlen(file)-strlen(query);

		if(file_len > sizeof(url))
			file_len = sizeof(url);


		strncpy(url, file, file_len);
	}
	else
	{
		strncpy(url, file, sizeof(url)-1);
	}
// 2007.11 James. }

	if(strncmp(url, APPLYAPPSTR, strlen(APPLYAPPSTR))==0)  fromapp=1;
	else if(strncmp(url, GETAPPSTR, strlen(GETAPPSTR))==0)  {
		fromapp=1;
		strlcpy(url, url+strlen(GETAPPSTR), sizeof(url));
		file += strlen(GETAPPSTR);
	}
	//_dprintf("fromapp(url): %i\n", fromapp);

	memset(user_agent, 0, sizeof(user_agent));
	if(useragent != NULL)
		strncpy(user_agent, useragent, sizeof(user_agent)-1);
	else
		strcpy(user_agent, "");

	fromapp = check_user_agent(useragent);
	//_dprintf("fromapp(check_user_agent): %i\n", fromapp);

	_dprintf("httpd url: %s file: %s\n", url, file);

	http_login_timeout(login_ip_tmp);
	set_referer_host();
	mime_exception = 0;
	do_referer = 0;

	if(!fromapp) {

		login_state = http_login_check();

		// for each page, mime_exception is defined to do exception handler

		// check exception
		for (exhandler = &except_mime_handlers[0]; exhandler->pattern; exhandler++) {
			if(match(exhandler->pattern, url))
			{
				mime_exception = exhandler->flag;
				break;
			}
		}
		// check doreferer
		for (doreferer = &mime_referers[0]; doreferer->pattern; doreferer++) {
			if(match(doreferer->pattern, url))
			{
				do_referer = doreferer->flag;
				break;
			}
		}

		// check for login conditions
		if(login_state==1)
		{
			x_Setting = nvram_get_int("x_Setting");
			skip_auth = 0;
		}
		else if(login_state==3) { // few pages can be shown even someone else login
			if(!(mime_exception&MIME_EXCEPTION_NOAUTH_ALL)) {
				file = "Nologin.asp";
				memset(url, 0, sizeof(url));
				strcpy(url, file);
			}
		}
	}
	else { // Jerry5 fix AiCloud login issue. 20120815
		x_Setting = nvram_get_int("x_Setting");
		skip_auth = 0;
	}

	for (handler = &mime_handlers[0]; handler->pattern; handler++) {
		if (match(handler->pattern, url))
		{
			nvram_set("httpd_handle_request", url);
			//nvram_set_int("httpd_handle_request_fromapp", fromapp);

			if (handler->auth) {
				if (skip_auth) {
					//skip
				}
				else if ((mime_exception&MIME_EXCEPTION_NOAUTH_FIRST)&&!x_Setting) {
					skip_auth=1;
				}
				else if((mime_exception&MIME_EXCEPTION_NOAUTH_ALL)) {
					//noauth
				}
				else {
					if(do_referer&CHECK_REFERER){
						referer_result = referer_check(referer, fromapp);
						_dprintf("referer_result(check): %i, referer: %s fromapp: %i\n", referer_result, referer, fromapp);
						if(referer_result != 0){
							if(strcasecmp(method, "post") == 0){
								if (handler->input) {
									handler->input(file, conn_fp, cl, boundary);
								}
							}
							if(!fromapp) {
								http_logout(login_ip_tmp);
							}
							return;
						}
					}
					handler->auth(auth_userid, auth_passwd, auth_realm);
					if (!auth_check(auth_realm, authorization, url))
					{
						_dprintf("referer_result(auth): realm: %s url: %s\n", auth_realm, url);
						if(strcasecmp(method, "post") == 0){
							if (handler->input) {
								handler->input(file, conn_fp, cl, boundary);
							}
						}
						if(!fromapp) {
							http_logout(login_ip_tmp);
						}
						return;
					}
				}

				if(!fromapp) {
					if (	!strstr(url, "QIS_")
							&& !strstr(url, ".js")
							&& !strstr(url, ".css")
							&& !strstr(url, ".gif")
							&& !strstr(url, ".png"))
						http_login(login_ip_tmp, url);
				}
			}else{
				if(fromapp == 0 && (do_referer&CHECK_REFERER)){
					referer_result = check_noauth_referrer(referer, fromapp);
					_dprintf("referer_result(noauth): %i, referer: %s fromapp: %i\n", referer_result, referer, fromapp);
					if(referer_result != 0){
						if(strcasecmp(method, "post") == 0){
							if (handler->input) {
								handler->input(file, conn_fp, cl, boundary);
							}
						}
						if(!fromapp) {
							http_logout(login_ip_tmp);
						}
						return;
					}
				}
			}

			if (strcasecmp(method, "post") == 0 && !handler->input) {
				send_error(501, "Not Implemented", NULL, "That method is not implemented.");
				return;
			}
// 2007.11 James. for QISxxx.htm pages }

			if (handler->input) {
				handler->input(file, conn_fp, cl, boundary);
#if defined(linux)
				if ((flags = fcntl(fileno(conn_fp), F_GETFL)) != -1 &&
						fcntl(fileno(conn_fp), F_SETFL, flags | O_NONBLOCK) != -1) {
					/* Read up to two more characters */
					if (fgetc(conn_fp) != EOF)
						(void)fgetc(conn_fp);

					fcntl(fileno(conn_fp), F_SETFL, flags);
				}
#elif defined(vxworks)
				flags = 1;
				if (ioctl(fileno(conn_fp), FIONBIO, (int) &flags) != -1) {
					/* Read up to two more characters */
					if (fgetc(conn_fp) != EOF)
						(void)fgetc(conn_fp);
					flags = 0;
					ioctl(fileno(conn_fp), FIONBIO, (int) &flags);
				}
#endif
			}

			if(!strstr(file,".cgi") && !strstr(file,".log") && !strstr(file,".CFG") && !strstr(file,".TAR") && !(strstr(file, "syslog.txt")) && !check_if_file_exist(file)){
				send_error( 404, "Not Found", (char*) 0, "File not found." );
				return;
			}

			send_headers( 200, "Ok", handler->extra_header, handler->mime_type );
			if (strcasecmp(method, "head") != 0 && handler->output) {
				handler->output(file, conn_fp);
			}

			break;
		}
	}

	if (!handler->pattern){
//#ifdef RTCONFIG_CLOUDSYNC
		// Todo: verify invite code
		if(strlen(file) > 50){
			char inviteCode[256];
			memset(cloud_file, 0, sizeof(cloud_file));
			if(!check_xxs_blacklist(file, 0))
				strlcpy(cloud_file, file, sizeof(cloud_file));
			snprintf(inviteCode, sizeof(inviteCode), "<meta http-equiv=\"refresh\" content=\"0; url=cloud_sync.asp?flag=%s\">\r\n", cloud_file);
			send_page( 200, "OK", (char*) 0, inviteCode);
		}
		else
//#endif
		send_error( 404, "Not Found", (char*) 0, "File not found." );
	}

	if(!fromapp) {
		if(!strcmp(file, "Logout.asp")){
			struct in_addr temp_ip_addr;
		        char *temp_ip_str;

			temp_ip_addr.s_addr = login_ip_tmp;
			temp_ip_str = inet_ntoa(temp_ip_addr);

			isLogout = 1;
			http_logout(login_ip_tmp);
			// Send logout msg to syslog
			logmessage (HEAD_HTTP_LOGIN, "logout successful %s:%d", temp_ip_str, http_port);

		}
	}
	nvram_unset("httpd_handle_request");
	//nvram_unset("httpd_handle_request_fromapp");
}

//2008 magic{
void http_login_cache(usockaddr *u) {
	struct in_addr temp_ip_addr;
	char *temp_ip_str;

	login_ip_tmp = (unsigned int)(u->sa_in.sin_addr.s_addr);
	temp_ip_addr.s_addr = login_ip_tmp;
	temp_ip_str = inet_ntoa(temp_ip_addr);
}

void http_get_access_ip(void) {
	struct in_addr tmp_access_addr;
	char tmp_access_ip[32];
	char *nv, *nvp, *b;
	int i=0, p=0;

	for(; i<ARRAY_SIZE(access_ip); i++)
		access_ip[i]=0;

	nv = nvp = strdup(nvram_safe_get("http_clientlist"));

	if (nv) {
		while ((b = strsep(&nvp, "<")) != NULL) {
			if (strlen(b)==0) continue;
			sprintf(tmp_access_ip, "%s", b);
			inet_aton(tmp_access_ip, &tmp_access_addr);

			if (p >= ARRAY_SIZE(access_ip)) {
				_dprintf("%s: access_ip out of range (p %d addr %x)!\n",
					__func__, p, (unsigned int)tmp_access_addr.s_addr);
				break;
			}
			access_ip[p] = (unsigned int)tmp_access_addr.s_addr;
			p++;
		}
		free(nv);
	}
}

void http_login(unsigned int ip, char *url) {
	struct in_addr login_ip_addr;
	char *login_ip_str;
	char login_ipstr[32], login_timestampstr[32];
	char login_port_str[] = "65535XXX";

	unsigned int login_port = nvram_get_int("login_port");
	unsigned int http_lanport = nvram_get_int("http_lanport");
	unsigned int https_lanport = nvram_get_int("https_lanport");
	unsigned int webdav_https_port = nvram_get_int("webdav_https_port");

	if ((http_port != http_lanport
//	  && http_port != SERVER_PORT
#ifdef RTCONFIG_HTTPS
	  && http_port != https_lanport
//	  && http_port != SERVER_PORT_SSL
	  && http_port != webdav_https_port
#endif
	    ) || ip == 0x100007f || ip == 0x0000000) // local or asusrouter
		return;

	login_timestamp = uptime();
	memset(login_timestampstr, 0, 32);
	sprintf(login_timestampstr, "%lu", login_timestamp);
	nvram_set("login_timestamp", login_timestampstr);

	if(ip != login_ip || http_port != login_port) {
		_dprintf("httpd_login(%u:%i)\n", ip, http_port);

		login_ip = ip;
		last_login_ip = 0;

		login_ip_addr.s_addr = login_ip;
		login_ip_str = inet_ntoa(login_ip_addr);
		nvram_set("login_ip_str", login_ip_str);

		memset(login_ipstr, 0, 32);
		sprintf(login_ipstr, "%u", login_ip);
		nvram_set("login_ip", login_ipstr);

		sprintf(login_port_str, "%u", http_port);
		nvram_set("login_port", login_port_str);
	}

	set_referer_host();
}

int http_client_ip_check(void) {
	struct in_addr tmp_access_addr;
	char tmp_access_ip[32];
	int i = 0;

	if(nvram_match("http_client", "1")) {
		//always allow router
		sprintf(tmp_access_ip, "%s", nvram_get("lan_ipaddr"));
		inet_aton(tmp_access_ip, &tmp_access_addr);
		if(login_ip_tmp==(unsigned int)tmp_access_addr.s_addr);
			return 1;

		//user specified ip
		while(i<ARRAY_SIZE(access_ip)) {
			if(access_ip[i]!=0) {
				if(login_ip_tmp==access_ip[i])
					return 1;
			}
			i++;
		}
		return 0;
	}

	return 1;
}

// 0: can not login, 1: can login, 2: loginer, 3: not loginer
int http_login_check(void)
{
	unsigned int login_port = nvram_get_int("login_port");
	unsigned int http_lanport = nvram_get_int("http_lanport");
	unsigned int https_lanport = nvram_get_int("https_lanport");
	unsigned int webdav_https_port = nvram_get_int("webdav_https_port");

	if ((http_port != http_lanport
//	  && http_port != SERVER_PORT
#ifdef RTCONFIG_HTTPS
	  && http_port != https_lanport
//	  && http_port != SERVER_PORT_SSL
	  && http_port != webdav_https_port
#endif
	    ) || login_ip_tmp == 0x100007f || login_ip_tmp == 0x0000000) // local or asusrouter
		//return 1;
		return 0;	// 2008.01 James.

	if (login_ip == 0 && !login_port)
		return 1;
	else if (login_ip == login_ip_tmp && (login_port == http_port || !login_port))
		return 2;

	return 3;
}

void http_login_timeout(unsigned int ip)
{
	time_t now, login_ts;
	struct in_addr temp_ip_addr;
	char *temp_ip_str;
	unsigned int login_port = nvram_get_int("login_port");

	temp_ip_addr.s_addr = login_ip;
	temp_ip_str = inet_ntoa(temp_ip_addr);

//	time(&now);
	now = uptime();
	login_ts = atol(nvram_safe_get("login_timestamp"));

// 2007.10 James. for really logout. {
	//if (login_ip!=ip && (unsigned long)(now-login_timestamp) > 60) //one minitues
//	if (((login_ip != 0 && login_ip != ip) || (login_port != http_port || !login_port)) && ((unsigned long)(now-login_ts) > 60)) //one minitues
	if (((login_ip != 0 && login_ip != ip) || ((login_port != http_port) && login_port)) && ((unsigned long)(now-login_ts) > MAX_DISC_TIMEOUT))
// 2007.10 James }
	{
		http_logout(login_ip);
		if (login_ip)
			logmessage(HEAD_HTTP_LOGIN, "logout successful (ip %s disconnected)", temp_ip_str);
		if (login_port)
			logmessage(HEAD_HTTP_LOGIN, "logout successful (port %d disconnected)", login_port);
	}
}

void http_logout(unsigned int ip)
{
	unsigned int login_port = nvram_get_int("login_port");
	unsigned int http_lanport = nvram_get_int("http_lanport");
	unsigned int https_lanport = nvram_get_int("https_lanport");

	if ((ip == login_ip && (login_port == http_lanport || login_port == https_lanport || !login_port)) || ip == 0 ) {
		_dprintf("httpd_logout(%u:%i)\n", ip, http_port);
		last_login_ip = login_ip;
		login_ip = 0;
		login_timestamp = 0;

		nvram_set("login_ip", "");
		nvram_set("login_timestamp", "");
		nvram_set("login_port", "");
		memset(referer_host, 0, sizeof(referer_host));

// 2008.03 James. {
		if (change_passwd == 1) {
			change_passwd = 0;
			reget_passwd = 1;
		}
// 2008.03 James. }
	}
}
//2008 magic}
//

int is_auth(void)
{
	unsigned int http_lanport = nvram_get_int("http_lanport");
	unsigned int https_lanport = nvram_get_int("https_lanport");
	unsigned int webdav_https_port = nvram_get_int("webdav_https_port");

	if (http_port==http_lanport ||
//	    http_port==SERVER_PORT ||
#ifdef RTCONFIG_HTTPS
	    http_port==https_lanport ||
//	    http_port==SERVER_PORT_SSL ||
	    http_port==webdav_https_port ||
#endif
		strcmp(nvram_get_x("PrinterStatus", "usb_webhttpcheck_x"), "1")==0) return 1;
	else return 0;
}

int is_firsttime(void)
{
	if (strcmp(nvram_get("x_Setting"), "1")==0)
		return 0;
	else
		return 1;
}

/* str_replace
* @param {char*} source
* @param {char*} find
* @param {char*} rep
* */
char *config_model_name(char *source, char *find,  char *rep){
   int find_L=strlen(find);
   int rep_L=strlen(rep);
   int length=strlen(source)+1;
   int gap=0;

   char *result = (char*)malloc(sizeof(char) * length);
   strcpy(result, source);

   char *former=source;
   char *location= strstr(former, find);

	/* stop searching when there is no finding string */
   while(location!=NULL){
       gap+=(location - former);
       result[gap]='\0';

       length+=(rep_L-find_L);
       result = (char*)realloc(result, length * sizeof(char));
       strcat(result, rep);
       gap+=rep_L;

       former=location+find_L;
       strcat(result, former);

       location= strstr(former, find);
   }
   return result;
}

#ifdef TRANSLATE_ON_FLY
#ifdef RTCONFIG_AUTODICT
int
load_dictionary (char *lang, pkw_t pkw)
{
	char dfn[16];
	char dummy_buf[16];
	int dict_item_idx;
	char* tmp_ptr;
	int dict_item; // number of dict item, now get from text file
	char *q;
	FILE *dfp = NULL;
	int remain_dict;
	int dict_size = 0;
//	struct timeval tv1, tv2;
	const char *eng_dict = "EN.dict";
#ifndef RELOAD_DICT
	static char loaded_dict[12] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
#endif  // RELOAD_DICT
#ifdef RTCONFIG_DYN_DICT_NAME
	char *dyn_dict_buf;
	char *dyn_dict_buf_new;
#endif

//printf ("lang=%s\n", lang);

//	gettimeofday (&tv1, NULL);
	if (lang == NULL || (lang != NULL && strlen (lang) == 0))       {
		// if "lang" is invalid, use English as default
		snprintf (dfn, sizeof (dfn), eng_dict);
	} else {
		snprintf (dfn, sizeof (dfn), "%s.dict", lang);
	}

#ifndef RELOAD_DICT
//	printf ("loaded_dict (%s) v.s. dfn (%s)\n", loaded_dict, dfn);
	if (strcmp (dfn, loaded_dict) == 0)     {
		return 1;
	}
	release_dictionary (pkw);
#endif  // RELOAD_DICT

	do      {
//		 printf("Open (%s) dictionary file.\n", dfn);
//
// Now DICT files all use UTF-8, it is no longer a text file
// it need to use open as binary
//
		dfp = fopen (dfn, "rb");
		if (dfp != NULL)	{
#ifndef RELOAD_DICT
			snprintf (loaded_dict, sizeof (loaded_dict), "%s", dfn);
#endif  // RELOAD_DICT
			break;
		}

//		printf ("Open (%s) failure. errno %d (%s)\n", dfn, errno, strerror (errno));
		if (dfp == NULL && strcmp (dfn, eng_dict) == 0) {
			return 0;
		} else {
			// If we can't open specified language file, use English as default
			snprintf (dfn, sizeof (dfn), eng_dict);
		}
	} while (1);

	memset (pkw, 0, sizeof (kw_t));
	fseek (dfp, 0L, SEEK_END);
	dict_size = ftell (dfp) + 128;
	// skip BOM header length
	dict_size -= 3;
	printf ("dict_size %d\n", dict_size);

#ifdef RTCONFIG_DYN_DICT_NAME
	dyn_dict_buf = (char *) malloc(dict_size);
	fseek (dfp, 0L, SEEK_SET);
	// skip BOM
	fread (dummy_buf, 1, 3, dfp);
	// read to dict string buffer
	memset(dyn_dict_buf, 0, dict_size);
	fread (dyn_dict_buf, 1, dict_size, dfp);
	dyn_dict_buf_new = config_model_name(dyn_dict_buf, "ZVDYNMODELVZ", nvram_safe_get("productid"));

	free(dyn_dict_buf);

	dict_size = sizeof(char) * strlen(dyn_dict_buf_new);
	pkw->buf = (unsigned char *) (q = malloc (dict_size));
	strcpy(pkw->buf, dyn_dict_buf_new);
	free(dyn_dict_buf_new);
#else
	pkw->buf = (unsigned char *) (q = malloc (dict_size));

	fseek (dfp, 0L, SEEK_SET);
	// skip BOM
	fread (dummy_buf, 1, 3, dfp);
	// read to dict string buffer
	memset(pkw->buf, 0, dict_size);
	fread (pkw->buf, 1, dict_size, dfp);
#endif
	// calc how many dict item , dict_item
	remain_dict = dict_size;
	tmp_ptr = (char *) pkw->buf;
	dict_item = 0;
	while (remain_dict>0) {
		if (*tmp_ptr == 0x0a) {
			dict_item++;
			tmp_ptr++;
			remain_dict--;
		}
		else if (*tmp_ptr == 0) {
			break;
		}
		else {
			tmp_ptr++;
			remain_dict--;
		}
	}
	// allocate memory according dict_item
	pkw->idx = malloc (dict_item * sizeof(unsigned char*));

	printf ("dict_item %d\n", dict_item);

	// get all string start and put to pkw->idx
	remain_dict = dict_size;
	for (dict_item_idx = 0; dict_item_idx < dict_item; dict_item_idx++) {
		pkw->idx[dict_item_idx] = (unsigned char *) q;
		while (remain_dict>0) {
			if (*q == 0x0a) {
				*q=0;
				q++;
				remain_dict--;
				break;
			}
			if (*q == 0) {
				break;
			}
			q++;
			remain_dict--;
		}
	}

	pkw->len = dict_item;

	fclose (dfp);

	return 1;
}


void
release_dictionary (pkw_t pkw)
{
	if (pkw == NULL)	{
		return;
	}

	//pkw->len = pkw->tlen = 0;
	pkw->len = 0;

	if (pkw->idx != NULL)   {
		free (pkw->idx);
		pkw->idx = NULL;
	}

	if (pkw->buf != NULL)   {
		free (pkw->buf);
		pkw->buf = NULL;
	}
}

char*
search_desc (pkw_t pkw, char *name)
{
	int i;
	char *ret = NULL;
	int dict_idx;
	char name_buf[128];

/*
	printf("search_desc:");
	printf(name);
	printf("\n");
*/

	if (pkw == NULL || (pkw != NULL && pkw->len <= 0))      {
		return NULL;
	}

	// remove equal
	memset(name_buf,0,sizeof(name_buf));
	// minus one for reserver one char for string zero char
	for (i = 0; i<sizeof(name_buf)-1; i++)  {
		if (*name == 0 || *name == '=') {
			break;
		}
		name_buf[i]=*name++;
	}

/*
	for (i = 0; i < pkw->len; ++i)  {
		char *p;
		p = pkw->idx[i];
		if (strncmp (name, p, strlen (name)) == 0)      {
			ret = p + strlen (name);
			break;
		}
	}
*/

/*
	printf("search_desc:");
	printf(name_buf);
	printf("\n");
*/

	dict_idx = atoi(name_buf);
//	printf("%d , %d\n",dict_idx,pkw->len);
	if (dict_idx < pkw->len) {
		ret = (char *) pkw->idx[dict_idx];
	}
	else {
		ret = (char *) pkw->idx[0];
	}

/*
	printf("ret:");
	printf(ret);
	printf("\n");
*/

	return ret;
}
#else
int
load_dictionary (char *lang, pkw_t pkw)
{
	char dfn[16];
	char *p, *q;
	FILE *dfp = NULL;
	int dict_size = 0;
//	struct timeval tv1, tv2;
	const char *eng_dict = "EN.dict";
#ifndef RELOAD_DICT
	static char loaded_dict[12] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
#endif  // RELOAD_DICT

//	gettimeofday (&tv1, NULL);
	if (lang == NULL || (lang != NULL && strlen (lang) == 0))       {
		// if "lang" is invalid, use English as default
		snprintf (dfn, sizeof (dfn), eng_dict);
	} else {
		snprintf (dfn, sizeof (dfn), "%s.dict", lang);
	}

#ifndef RELOAD_DICT
//	printf ("loaded_dict (%s) v.s. dfn (%s)\n", loaded_dict, dfn);
	if (strcmp (dfn, loaded_dict) == 0)     {
		return 1;
	}
	release_dictionary (pkw);
#endif  // RELOAD_DICT

	do      {
//		 printf("Open (%s) dictionary file.\n", dfn);
		dfp = fopen (dfn, "r");
		if (dfp != NULL)	{
#ifndef RELOAD_DICT
			snprintf (loaded_dict, sizeof (loaded_dict), "%s", dfn);
#endif  // RELOAD_DICT
			break;
		}

//		printf ("Open (%s) failure. errno %d (%s)\n", dfn, errno, strerror (errno));
		if (dfp == NULL && strcmp (dfn, eng_dict) == 0) {
			return 0;
		} else {
			// If we can't open specified language file, use English as default
			snprintf (dfn, sizeof (dfn), eng_dict);
		}
	} while (1);

	memset (pkw, 0, sizeof (kw_t));
	fseek (dfp, 0L, SEEK_END);
	dict_size = ftell (dfp) + 128;
//	printf ("dict_size %d\n", dict_size);
	REALLOC_VECTOR (pkw->idx, pkw->len, pkw->tlen, sizeof (unsigned char*));
	pkw->buf = q = malloc (dict_size);

	fseek (dfp, 0L, SEEK_SET);
#if 0
	while (!feof (dfp))     {
		// if pkw->idx is not enough, add 32 item to pkw->idx
		REALLOC_VECTOR (pkw->idx, pkw->len, pkw->tlen, sizeof (unsigned char*));

		fscanf (dfp, "%[^\n]%*c", q);
		if ((p = strchr (q, '=')) != NULL)      {
			pkw->idx[pkw->len] = q;
			pkw->len++;
			q = p + strlen (p);
			*q = '\0';
			q++;
		}
	}
#else
	while ((fscanf(dfp, "%[^\n]", q)) != EOF) {
		fgetc(dfp);

		// if pkw->idx is not enough, add 32 item to pkw->idx
		REALLOC_VECTOR (pkw->idx, pkw->len, pkw->tlen, sizeof (unsigned char*));

		if ((p = strchr (q, '=')) != NULL) {
			pkw->idx[pkw->len] = q;
			pkw->len++;
			q = p + strlen (p);
			*q = '\0';
			q++;
		}
	}

#endif

	fclose (dfp);
//	gettimeofday (&tv2, NULL);
//	printf("Load %d keywords spent %ldms\n", pkw->len, ((tv2.tv_sec * 1000000 + tv2.tv_usec) - (tv1.tv_sec * 1000000 + tv1.tv_usec)) / 1000);

	return 1;
}


void
release_dictionary (pkw_t pkw)
{
	if (pkw == NULL)	{
		return;
	}

	pkw->len = pkw->tlen = 0;

	if (pkw->idx != NULL)   {
		free (pkw->idx);
		pkw->idx = NULL;
	}

	if (pkw->buf != NULL)   {
		free (pkw->buf);
		pkw->buf = NULL;
	}
}

char*
search_desc (pkw_t pkw, char *name)
{
	int i;
	char *p, *ret = NULL;

	if (pkw == NULL || (pkw != NULL && pkw->len <= 0))      {
		return NULL;
	}
	for (i = 0; i < pkw->len; ++i)  {
		p = pkw->idx[i];
		if (strncmp (name, p, strlen (name)) == 0)      {
			ret = p + strlen (name);
			break;
		}
	}

	return ret;
}
#endif
#endif //TRANSLATE_ON_FLY

void reapchild()	// 0527 add
{
	signal(SIGCHLD, reapchild);
	wait(NULL);
}

#ifdef RTCONFIG_QTN
extern char *wl_ether_etoa(const struct ether_addr *n);
#endif

int do_ssl = 0; 	// use Global for HTTPS upgrade judgment in web.c
int ssl_stream_fd; 	// use Global for HTTPS stream fd in web.c
int main(int argc, char **argv)
{
	usockaddr usa;
	int listen_fd;
	socklen_t sz = sizeof(usa);
	char pidfile[32];
	fd_set active_rfds;
	conn_list_t pool;
	int c;
	//int do_ssl = 0;

	do_ssl = 0; // default
	http_port = nvram_get_int("http_lanport");
	// usage : httpd -s -p [port]
	if(argc) {
		while ((c = getopt(argc, argv, "sp:")) != -1) {
			switch (c) {
			case 's':
				do_ssl = 1;
				break;
			case 'p':
				http_port = atoi(optarg);
				break;
			default:
				fprintf(stderr, "ERROR: unknown option %c\n", c);
				break;
			}
		}
	}

#ifdef RTCONFIG_QTN
	time_t start_time = uptime();
	int ret;
QTN_RESET:
	ret = rpc_qcsapi_init();
	if (ret < 0) {
		dbG("Qcsapi qcsapi init error, return: %d\n", ret);
	}
	else if (nvram_get_int("qtn_restore_defaults"))
	{
		nvram_unset("qtn_restore_defaults");
//		eval("qcsapi_sockrpc", "update_bootcfg_param", "ipaddr", "1.1.1.2");
		rpc_qcsapi_restore_default_config(0);
		dbG("Restaring Qcsapi init...\n");
		sleep(15);
		goto QTN_RESET;
	}

	qcsapi_init();

	dbG("Qcsapi qcsapi init takes %ld seconds\n", uptime() - start_time);

	qcsapi_mac_addr wl_mac_addr;
	ret = rpc_qcsapi_interface_get_mac_addr(WIFINAME, &wl_mac_addr);
	if (ret < 0)
		dbG("rpc_qcsapi_interface_get_mac_addr, return: %d\n", ret);
	else
	{
		nvram_set("1:macaddr", wl_ether_etoa((struct ether_addr *) &wl_mac_addr));
		nvram_set("wl1_hwaddr", wl_ether_etoa((struct ether_addr *) &wl_mac_addr));
	}

	ret = qcsapi_wps_set_ap_pin(WIFINAME, nvram_safe_get("wps_device_pin"));
	if (ret < 0)
		dbG("Qcsapi qcsapi_wps_set_ap_pin %s error, return: %d\n", WIFINAME, ret);
	ret = qcsapi_wps_registrar_set_pp_devname(WIFINAME, 0, (const char *) get_productid());
	if (ret < 0)
		dbG("Qcsapi qcsapi_wps_registrar_set_pp_devname %s error, return: %d\n", WIFINAME, ret);
	ret = rpc_qcsapi_wifi_disable_wps(WIFINAME, !nvram_get_int("wps_enable"));
	if (ret < 0)
		dbG("Qcsapi rpc_qcsapi_wifi_disable_wps %s error, return: %d\n", WIFINAME, ret);

	nvram_set("qtn_ready", "1");
#endif

	//websSetVer();
	//2008.08 magic
	nvram_unset("login_timestamp");
	nvram_unset("login_ip");
	nvram_unset("login_ip_str");
	nvram_unset("login_port");
	MAX_login = nvram_get_int("login_max_num");
	if(MAX_login <= DEFAULT_LOGIN_MAX_NUM)
		MAX_login = DEFAULT_LOGIN_MAX_NUM;

	detect_timestamp_old = 0;
	detect_timestamp = 0;
	signal_timestamp = 0;

	http_get_access_ip();

	/* Ignore broken pipes */
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, reapchild);	// 0527 add

#ifdef RTCONFIG_HTTPS
	if (do_ssl)
		start_ssl();
#endif

	/* Initialize listen socket */
	if ((listen_fd = initialize_listen_socket(&usa)) < 2) {
		fprintf(stderr, "can't bind to any address\n" );
		exit(errno);
	}

	FILE *pid_fp;
//	if (http_port==SERVER_PORT)
	if (http_port==nvram_get_int("http_lanport"))
		strcpy(pidfile, "/var/run/httpd.pid");
	else sprintf(pidfile, "/var/run/httpd-%d.pid", http_port);

	if (!(pid_fp = fopen(pidfile, "w"))) {
		perror(pidfile);
		return errno;
	}
	fprintf(pid_fp, "%d", getpid());
	fclose(pid_fp);

	/* Init connection pool */
	FD_ZERO(&active_rfds);
	TAILQ_INIT(&pool.head);
	pool.count = 0;

	/* Loop forever handling requests */
	for (;;) {
		int max_fd, count;
		struct timeval tv;
		fd_set rfds;
		conn_item_t *item, *next;

		memcpy(&rfds, &active_rfds, sizeof(rfds));
		if (pool.count < MAX_CONN_ACCEPT) {
			FD_SET(listen_fd, &rfds);
			max_fd = listen_fd;
		} else  max_fd = -1;
		TAILQ_FOREACH(item, &pool.head, entry)
			max_fd = (item->fd > max_fd) ? item->fd : max_fd;

		/* Wait for new connection or incoming request */
		tv.tv_sec = MAX_CONN_TIMEOUT;
		tv.tv_usec = 0;
		while ((count = select(max_fd + 1, &rfds, NULL, NULL, &tv)) < 0 && errno == EINTR)
			continue;
		if (count < 0) {
			perror("select");
			return errno;
		}

		/* Check and accept new connection */
		if (count && FD_ISSET(listen_fd, &rfds)) {
			item = malloc(sizeof(*item));
			if (item == NULL) {
				perror("malloc");
				return errno;
			}
			while ((item->fd = accept(listen_fd, &item->usa.sa, &sz)) < 0 && errno == EINTR)
				continue;
			if (item->fd < 0) {
				perror("accept");
				free(item);
				continue;
			}

			/* Set the KEEPALIVE option to cull dead connections */
			setsockopt(item->fd, SOL_SOCKET, SO_KEEPALIVE, &int_1, sizeof(int_1));

			/* Add to active connections */
			FD_SET(item->fd, &active_rfds);
			TAILQ_INSERT_TAIL(&pool.head, item, entry);
			pool.count++;

			/* Continue waiting over again */
			continue;
		}

		/* Check and process pending or expired requests */
		TAILQ_FOREACH_SAFE(item, &pool.head, entry, next) {
			if (count && !FD_ISSET(item->fd, &rfds))
				continue;

			/* Delete from active connections */
			FD_CLR(item->fd, &active_rfds);
			TAILQ_REMOVE(&pool.head, item, entry);
			pool.count--;

			/* Process request if any */
			if (count) {
#ifdef RTCONFIG_HTTPS
				if (do_ssl) {
					ssl_stream_fd = item->fd;
					if (!(conn_fp = ssl_server_fopen(item->fd))) {
						perror("fdopen(ssl)");
						goto skip;
					}
				} else
#endif
				if (!(conn_fp = fdopen(item->fd, "r+"))) {
					perror("fdopen");
					goto skip;
				}

				http_login_cache(&item->usa);
				if (http_client_ip_check())
					handle_request();

				fflush(conn_fp);
#ifdef RTCONFIG_HTTPS
				if (!do_ssl)
#endif
				shutdown(item->fd, 2), item->fd = -1;
				fclose(conn_fp);

			skip:
				/* Skip the rest of */
				if (--count == 0)
					next = NULL;

			}

			/* Close timed out and/or still alive */
			if (item->fd >= 0) {
				shutdown(item->fd, 2);
				close(item->fd);
			}

			free(item);
		}
	}

	shutdown(listen_fd, 2);
	close(listen_fd);

	return 0;
}

#ifdef RTCONFIG_HTTPS
void save_cert(void)
{
	if (eval("tar", "-C", "/", "-czf", "/tmp/cert.tgz", "etc/cert.pem", "etc/key.pem") == 0) {
		if (nvram_match("jffs2_on", "1") && check_if_dir_exist("/jffs/https")) {
			system("cp /tmp/cert.tgz /jffs/https/cert.tgz");
			nvram_set("https_crt_file", "");
		} else {
			if (nvram_set_file("https_crt_file", "/tmp/cert.tgz", 8192)) {
				nvram_commit_x();
			}
		}
	}
	unlink("/tmp/cert.tgz");
}

void erase_cert(void)
{
	unlink("/etc/cert.pem");
	unlink("/etc/key.pem");
	nvram_unset("https_crt_file");
	if (check_if_file_exist("/jffs/https/cert.tgz"))
		system("rm /jffs/https/cert.tgz");
	//nvram_unset("https_crt_gen");
	nvram_set("https_crt_gen", "0");
}

void start_ssl(void)
{
	int ok;
	int save;
	int retry;
	unsigned long long sn;
	char t[32];

	//fprintf(stderr,"[httpd] start_ssl running!!\n");
	//nvram_set("https_crt_gen", "1");

	if (nvram_match("https_crt_gen", "1")) {
		erase_cert();
	}

	retry = 1;
	while (1) {
		save = nvram_match("https_crt_save", "1");

		if ((!f_exists("/etc/cert.pem")) || (!f_exists("/etc/key.pem"))) {
			ok = 0;
			if (save) {
				fprintf(stderr, "Save SSL certificate...\n"); // tmp test
				if (check_if_file_exist("/jffs/https/cert.tgz")) {
					if (eval("tar", "-xzf", "/jffs/https/cert.tgz", "-C", "/", "etc/cert.pem", "etc/key.pem") == 0){
						system("cat /etc/key.pem /etc/cert.pem > /etc/server.pem");
						ok =1;
					}
				} else {
					if (nvram_get_file("https_crt_file", "/tmp/cert.tgz", 8192)) {
						if (eval("tar", "-xzf", "/tmp/cert.tgz", "-C", "/", "etc/cert.pem", "etc/key.pem") == 0){
							system("cat /etc/key.pem /etc/cert.pem > /etc/server.pem");
							unlink("/tmp/cert.tgz");
							ok = 1;
						}
					}
				}
			}
			if (!ok) {
				erase_cert();
				syslog(LOG_NOTICE, "Generating SSL certificate...");
				fprintf(stderr, "Generating SSL certificate...\n"); // tmp test
				// browsers seems to like this when the ip address moves...	-- zzz
				f_read("/dev/urandom", &sn, sizeof(sn));

				sprintf(t, "%llu", sn & 0x7FFFFFFFFFFFFFFFULL);
				eval("gencert.sh", t);
			}
		}

		if ((save) && (*nvram_safe_get("https_crt_file") == 0) && !check_if_file_exist("/jffs/https/cert.tgz")) {
			save_cert();
		}

		if (mssl_init("/etc/cert.pem", "/etc/key.pem")) return;

		erase_cert();

		if (!retry) exit(1);
		retry = 0;
	}
}
#endif

