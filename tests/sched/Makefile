# $NetBSD: Makefile,v 1.0 $
#
# Makefile para pruebas de Scheduler

NOMAN=		# defined

.include <bsd.own.mk>

TESTSDIR=	${TESTSBASE}/sched

BINDIR=		${TESTSDIR}
PROGS=		cpu_bound_test
PROGS+=		sleep_test

SRCS.cpu_bound_test=	CPU_bound_test.c
SRCS.sleep_test=	Sleep_test.c

.include <bsd.test.mk>
