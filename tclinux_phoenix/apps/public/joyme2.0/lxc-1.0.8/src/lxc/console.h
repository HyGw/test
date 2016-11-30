/*
 * lxc: linux Container library
 *
 * (C) Copyright IBM Corp. 2007, 2010
 *
 * Authors:
 * Daniel Lezcano <daniel.lezcano at free.fr>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef __LXC_CONSOLE_H
#define __LXC_CONSOLE_H

struct lxc_epoll_descr;
struct lxc_container;

extern int  lxc_console_allocate(struct lxc_conf *conf, int sockfd, int *ttynum);
extern int  lxc_console_create(struct lxc_conf *);
extern void lxc_console_delete(struct lxc_console *);
extern void lxc_console_free(struct lxc_conf *conf, int fd);

extern int  lxc_console_mainloop_add(struct lxc_epoll_descr *, struct lxc_handler *);
extern void lxc_console_sigwinch(int sig);
extern int  lxc_console(struct lxc_container *c, int ttynum,
		        int stdinfd, int stdoutfd, int stderrfd,
		        int escape);
extern int  lxc_console_getfd(struct lxc_container *c, int *ttynum,
			      int *masterfd);
extern int  lxc_console_set_stdfds(struct lxc_handler *);

#endif
