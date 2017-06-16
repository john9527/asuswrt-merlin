include ../common.mak

all:

install:
	install -d $(INSTALLDIR)/sbin
	install -d $(INSTALLDIR)/usr/sbin
	install -d $(INSTALLDIR)/lib

ifeq ($(TCONFIG_USB),y)
# Optware perl symlink
	install -d $(INSTALLDIR)/usr/bin
	ln -sf /opt/bin/perl $(INSTALLDIR)/usr/bin/perl
endif

# cron helper
	install -m 0700 cru $(INSTALLDIR)/usr/sbin

# playing with ipkg / from OpenWrt svn
#!!TB	install -m 0700 ipkg $(INSTALLDIR)/usr/sbin

# system info
	install -m 0700 sysinfo $(INSTALLDIR)/usr/sbin

# keep winscp happy
#	install -m 0700 groups $(INSTALLDIR)/usr/bin

# chpasswd script
	install -m 0700 chpasswd.sh $(INSTALLDIR)/usr/sbin

# dnscrypt resolver updater
ifeq ($(RTCONFIG_DNSCRYPT),y)
	install -m 0700 dnscrypt-update-resolvers.sh $(INSTALLDIR)/usr/sbin
endif

# ipv6 hosts updater
ifeq ($(RTCONFIG_DNSMASQ),y)
	install -m 0700 -D v6hosts.sh $(INSTALLDIR)/usr/sbin
	install -m 0700 -D updatehosts.sh $(INSTALLDIR)/usr/sbin
endif

# Utilites to move certs
	install -m 0700 -D sshd2nvram $(INSTALLDIR)/usr/sbin
	install -m 0700 -D sshd2jffs $(INSTALLDIR)/usr/sbin
	install -m 0700 -D ovpn2nvram $(INSTALLDIR)/usr/sbin
	install -m 0700 -D ovpn2jffs $(INSTALLDIR)/usr/sbin
	install -m 0700 -D https2nvram $(INSTALLDIR)/usr/sbin
	install -m 0700 -D https2jffs $(INSTALLDIR)/usr/sbin

# Easy-rsa scripts (for OpenVPN)
ifeq ($(RTCONFIG_OPENVPN),y)
	install -m 0700 setuprsa.sh $(INSTALLDIR)/usr/sbin
	install -m 0700 -D updown.sh $(INSTALLDIR)/usr/sbin
	install -m 0700 -D vpnrouting.sh $(INSTALLDIR)/usr/sbin
endif

# Entware setup script
	install -m 0700 entware-setup.sh $(INSTALLDIR)/usr/sbin

# Postconf helper script
	install -m 0755 helper.sh $(INSTALLDIR)/usr/sbin

# Conntrack connection counts script
	install -m 0755 conntrkcnt.sh $(INSTALLDIR)/usr/sbin

ifeq ($(RTN14U)$(RTAC52U)$(RTAC51U)$(RTN11P),y)
# for read/write Ralink PHY registers
        install -D mii_mgr $(INSTALLDIR)/bin/mii_mgr
endif
clean:
