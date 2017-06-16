## Process this file with automake to produce Makefile.in.

EXTRA_DIST = service.fedora service.forking service.redhat sysv.lsb sysv.redhat

MAINTAINERCLEANFILES = Makefile.in

CLEANFILES = haveged haveged.service

do_subst = sed -e 's,[@]SBIN_DIR[@],$(sbindir),g'
src_tmpl = @HA_DISTRO@
unit_dir = @HA_UNITD@

if ENABLE_NOINIT
## user will install manually.
install-exec-hook:
	@echo "no init script installed";
endif

if ENABLE_SYSV
## legacy init script - installation via automake defaults

initdir = $(sysconfdir)/init.d
init_SCRIPTS = haveged

haveged:	$(src_tmpl) Makefile
			$(do_subst) < $(srcdir)/$(src_tmpl) > haveged;

endif

if ENABLE_SYSTEMD
## systemd script - lookup unitdir if not specified

install-exec-hook:
	$(do_subst) < $(srcdir)/$(src_tmpl) > haveged.service;

install-data-hook:
if ENABLE_SYSTEMD_LOOKUP
	install -p -m644 haveged.service `pkg-config --variable=systemdsystemunitdir systemd`/haveged.service;
else
	install -p -m644 haveged.service ($unit_dir)/haveged.service;
endif
	systemctl enable haveged.service;

endif
