#!/bin/sh

source /usr/sbin/helper.sh
SECS=1262278080

cd /etc
cp -L openssl.cnf openssl.config

NVCN=`nvram get https_crt_cn`
if [ "$NVCN" == "" ]; then
	NVCN=`nvram get lan_ipaddr`
fi

OLDIFS=$IFS
IFS=","
I=0

echo "$I.organizationName=O" >> /etc/openssl.config
echo "$I.organizationName_value=$(uname -o)" >> /etc/openssl.config

for CN in $NVCN; do
        echo "$I.commonName=CN" >> /etc/openssl.config
        echo "$I.commonName_value=$CN" >> /etc/openssl.config
        I=$(($I + 1))
done
IFS=$OLDIFS

pc_insert "[ CA_default ]" "copy_extensions = copy" /etc/openssl.config
pc_insert "[ v3_ca ]" "subjectAltName = @alt_names" /etc/openssl.config
#pc_insert "[ v3_ca ]" "certificatePolicies = 2.5.29.32.0" /etc/openssl.config
pc_insert "[ v3_req ]" "subjectAltName = @alt_names" /etc/openssl.config

# Complete SAN definitions
echo "[ alt_names ]" >> /etc/openssl.config

I=0
# Add IPs to SAN
LANIP=`nvram get lan_ipaddr`
echo "IP.$I = $LANIP" >> /etc/openssl.config
echo "DNS.$I = $LANIP" >> /etc/openssl.config	# workaround for IE not supporting IP SAN
I=$(($I + 1))

# Add DNS names to SAN
SWMODE=$(nvram get sw_mode)
if [ $SWMODE -eq 1 ] # only add DUT_DOMAIN for router mode
then
	echo "DNS.$I = router.asus.com" >> /etc/openssl.config
	I=$(($I + 1))
fi

# Add hostnames
LANDOMAIN=$(nvram get lan_domain)
LANHOSTNAME=$(nvram get lan_hostname)
COMPUTERNAME=$(nvram get computer_name)
if [ "$LANHOSTNAME" != "" ]
then
	echo "DNS.$I = $LANHOSTNAME" >> /etc/openssl.config
	I=$(($I + 1))

	if [ "$LANDOMAIN" != "" ]
	then
		echo "DNS.$I = $LANHOSTNAME.$LANDOMAIN" >> /etc/openssl.config
		I=$(($I + 1))
	fi
fi

if [ "$COMPUTERNAME" != "" ]
then
	echo "DNS.$I = $COMPUTERNAME" >> /etc/openssl.config
	I=$(($I + 1))

	if [ "$LANDOMAIN" != "" ]
	then
		echo "DNS.$I = $COMPUTERNAME.$LANDOMAIN" >> /etc/openssl.config
		I=$(($I + 1))
	fi
fi

# Add DDNS
DDNSHOSTNAME=$(nvram get ddns_hostname_x)
DDNSSERVER=$(nvram get ddns_server_x)
DDNSUSER=$(nvram get ddns_username_x)
if [ "$(nvram get ddns_enable_x)" == "1" -a "$DDNSSERVER" != "WWW.DNSOMATIC.COM" -a "$DDNSHOSTNAME" != "" ]
then
	if [ "$DDNSSERVER" == "WWW.NAMECHEAP.COM" -a "$DDNSUSER" != "" ]
	then
		echo "DNS.$I = $DDNSHOSTNAME.$DDNSUSER" >> /etc/openssl.config
		I=$(($I + 1))
	else
		echo "DNS.$I = $DDNSHOSTNAME" >> /etc/openssl.config
		I=$(($I + 1))
	fi
fi

# create the key and certificate request
#OPENSSL_CONF=/etc/openssl.config openssl req -new -out /tmp/cert.csr -keyout /tmp/privkey.pem -newkey rsa:1024 -passout pass:password
# remove the passphrase from the key
#OPENSSL_CONF=/etc/openssl.config openssl rsa -in /tmp/privkey.pem -out key.pem -passin pass:password
# convert the certificate request into a signed certificate
#OPENSSL_CONF=/etc/openssl.config RANDFILE=/dev/urandom openssl x509 -in /tmp/cert.csr -out cert.pem -req -signkey key.pem -days 3653 -sha256

# create the key
openssl genrsa -out key.pem 2048 -config /etc/openssl.config
# create certificate request and sign it
RANDFILE=/dev/urandom openssl req -new -x509 -key key.pem -sha256 -out cert.pem -days 3653 -config /etc/openssl.config

#	openssl x509 -in /etc/cert.pem -text -noout

# server.pem for WebDav SSL
cat key.pem cert.pem > server.pem

if [ "$(nvram get https_crt_save)" == "0" ]; then
	rm -f /etc/openssl.config
fi
