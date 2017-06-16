<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=edge"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
<link rel="shortcut icon" href="images/favicon.png">
<link rel="icon" href="images/favicon.png">
<title><#Web_Title#> - OpenVPN Client Settings</title>
<link rel="stylesheet" type="text/css" href="index_style.css">
<link rel="stylesheet" type="text/css" href="form_style.css">

<script language="JavaScript" type="text/javascript" src="/state.js"></script>
<script language="JavaScript" type="text/javascript" src="/general.js"></script>
<script language="JavaScript" type="text/javascript" src="/popup.js"></script>
<script language="JavaScript" type="text/javascript" src="/help.js"></script>
<script type="text/javascript" language="JavaScript" src="/detect.js"></script>
<script language="JavaScript" type="text/javascript" src="/client_function.js"></script>
<script type="text/javascript" language="JavaScript" src="/merlin.js"></script>

<script type="text/javascript" src="/jquery.js"></script>
<script type="text/javascript" src="/switcherplugin/jquery.iphone-switch.js"></script>
<style type="text/css">
.contentM_qis{
	width:740px;
	margin-top:220px;
	margin-left:380px;
	position:absolute;
	-webkit-border-radius: 5px;
	-moz-border-radius: 5px;
	border-radius: 5px;
	z-index:200;
	background-color:#2B373B;
	display:none;
	/*behavior: url(/PIE.htc);*/
}
.QISmain{
	width:730px;
	/*font-family:Verdana, Arial, Helvetica, sans-serif;*/
	font-size:14px;
	z-index:200;
	position:relative;
	background-color:balck:
}
.QISform_wireless{
	width:600px;
	font-size:12px;
	color:#FFFFFF;
	margin-top:10px;
	*margin-left:10px;
}

.QISform_wireless thead{
	font-size:15px;
	line-height:20px;
	color:#FFFFFF;
}

.QISform_wireless th{
	padding-left:10px;
	*padding-left:30px;
	font-size:12px;
	font-weight:bolder;
	color: #FFFFFF;
	text-align:left;
}

.QISform_wireless li{
	margin-top:10px;
}
.QISGeneralFont{
	font-family:Segoe UI, Arial, sans-serif;
	margin-top:10px;
	margin-left:70px;
	*margin-left:50px;
	margin-right:30px;
	color:white;
	LINE-HEIGHT:18px;
}	
.description_down{
	margin-top:10px;
	margin-left:10px;
	padding-left:5px;
	font-weight:bold;
	line-height:140%;
	color:#ffffff;
}
#ClientList_Block_PC{
	border:1px outset #999;
	background-color:#576D73;
	position:absolute;
	*margin-top:26px;
	margin-left:53px;
	*margin-left:-189px;
	width:255px;
	text-align:left;
	height:auto;
	overflow-y:auto;
	z-index:200;
	padding: 1px;
	display:none;
}
#ClientList_Block_PC div{
	background-color:#576D73;
	height:auto;
	*height:20px;
	line-height:20px;
	text-decoration:none;
	font-family: Lucida Console;
	padding-left:2px;
}

#ClientList_Block_PC a{
	background-color:#EFEFEF;
	color:#FFF;
	font-size:12px;
	font-family:Arial, Helvetica, sans-serif;
	text-decoration:none;
}
#ClientList_Block_PC div:hover, #ClientList_Block a:hover{
	background-color:#3366FF;
	color:#FFFFFF;
	cursor:default;
}
</style>
<script>
var $j = jQuery.noConflict();
<% login_state_hook(); %>

var wan_route_x = '<% nvram_get("wan_route_x"); %>';
var wan_nat_x = '<% nvram_get("wan_nat_x"); %>';
var wan_proto = '<% nvram_get("wan_proto"); %>';
var dnscrypt_proxy = '<% nvram_get("dnscrypt_proxy"); %>';
var dnscrypt_ipv6 = '<% nvram_get("dnscrypt1_ipv6"); %>';
var machine_name = '<% get_machine_name(); %>';
var allow_routelocal = (((machine_name.search("arm") == -1) ? false : true) && (('<% nvram_get("allow_routelocal"); %>' == 1) ? true : false));

<% vpn_client_get_parameter(); %>

var openvpn_unit = '<% nvram_get("vpn_client_unit"); %>';

if (openvpn_unit == 1)
	service_state = (<% sysinfo("pid.vpnclient1"); %> > 0);
else if (openvpn_unit == 2)
	service_state = (<% sysinfo("pid.vpnclient2"); %> > 0);
else
	service_state = false;

var enforce_orig = "<% nvram_get("vpn_client_enforce"); %>";
var policy_orig = "<% nvram_get("vpn_client_rgw"); %>";
var adns_orig = "<% nvram_get("vpn_client_adns"); %>";

ciphersarray = [
		["AES-128-CBC"],
		["AES-128-CFB"],
		["AES-128-OFB"],
		["AES-192-CBC"],
		["AES-192-CFB"],
		["AES-192-OFB"],
		["AES-256-CBC"],
		["AES-256-CFB"],
		["AES-256-OFB"],
		["BF-CBC"],
		["BF-CFB"],
		["BF-OFB"],
		["CAST5-CBC"],
		["CAST5-CFB"],
		["CAST5-OFB"],
		["DES-CBC"],
		["DES-CFB"],
		["DES-EDE3-CBC"],
		["DES-EDE3-CFB"],
		["DES-EDE3-OFB"],
		["DES-EDE-CBC"],
		["DES-EDE-CFB"],
		["DES-EDE-OFB"],
		["DES-OFB"],
		["DESX-CBC"],
		["IDEA-CBC"],
		["IDEA-CFB"],
		["IDEA-OFB"]
];

var digestsarray = [
		["DSA"],
		["DSA-SHA"],
		["DSA-SHA1"],
		["DSA-SHA1-old"],
		["ecdsa-with-SHA1"],
		["MD4"],
		["MD5"],
		["MDC2"],
		["RIPEMD160"],
		["RSA-MD4"],
		["RSA-MD5"],
		["RSA-MDC2"],
		["RSA-RIPEMD160"],
		["RSA-SHA"],
		["RSA-SHA1"],
		["RSA-SHA1-2"],
		["RSA-SHA224"],
		["RSA-SHA256"],
		["RSA-SHA384"],
		["RSA-SHA512"],
		["SHA"],
		["SHA1"],
		["SHA224"],
		["SHA256"],
		["SHA384"],
		["SHA512"],
		["whirlpool"]
];

var clientlist_array = '<% nvram_get("vpn_client_clientlist"); %>';

function initial()
{
	var ipv6_warning = ('<% nvram_get("ipv6_service"); %>' == "disabled") ? 0 : 1;
	if (ipv6_warning)
		document.getElementById('ipv6warning').style.display = "";

	show_menu();
	showclientlist();
	showLANIPList();

	// Cipher list
	free_options(document.form.vpn_client_cipher);
	currentcipher = "<% nvram_get("vpn_client_cipher"); %>";
	add_option(document.form.vpn_client_cipher, "Default","default",(currentcipher.toLowerCase() == "default"));
	add_option(document.form.vpn_client_cipher, "None","none",(currentcipher.toLowerCase() == "none"));

	// Digest list
	free_options(document.form.vpn_client_digest);
	currentdigest = "<% nvram_get("vpn_client_digest"); %>";
	add_option(document.form.vpn_client_digest, "Default","default",(currentdigest.toLowerCase() == "default"));
	add_option(document.form.vpn_client_digest, "None","none",(currentdigest.toLowerCase() == "none"));

	// Extract the type out of the interface name 
	// (imported ovpn can result in this being tun3, for example)
	currentiface = "<% nvram_get("vpn_client_if"); %>";
	add_option(document.form.vpn_client_if_x, "TUN","tun",(currentiface.indexOf("tun") != -1));
	add_option(document.form.vpn_client_if_x, "TAP","tap",(currentiface.indexOf("tap") != -1));

	for(var i = 0; i < ciphersarray.length; i++){
		add_option(document.form.vpn_client_cipher,
			ciphersarray[i][0], ciphersarray[i][0],
			(currentcipher.toLowerCase() == ciphersarray[i][0].toLowerCase()));
	}

	for(var i = 0; i < digestsarray.length; i++){
		add_option(document.form.vpn_client_digest,
			digestsarray[i][0], digestsarray[i][0],
			(currentdigest.toLowerCase() == digestsarray[i][0].toLowerCase()));
	}

	// Set these based on a compound field
	setRadioValue(document.form.vpn_client_x_eas, ((document.form.vpn_clientx_eas.value.indexOf(''+(openvpn_unit)) >= 0) ? "1" : "0"));
	document.form.enable_dns_ckb.checked = ('<% nvram_get("vpn_dns_mode"); %>' == 1) ? true : false;

	// Remove CR characters in custom config
	document.form.vpn_client_custom.value = document.form.vpn_client_custom.value.replace(/[\r]/g, '');

	//disable policy based routing option for tap connections
	update_rgw_options();
	document.form.vpn_client_rgw.value = policy_orig;
	document.form.vpn_client_adns.value = adns_orig;

	// Decode into editable format
	openvpn_decodeKeys(0);
	update_visibility();
}

function openvpn_decodeKeys(entities){
	var expr;

	if (entities == 1)
		expr = new RegExp('&#62;','gm');
	else
		expr = new RegExp('>','gm');

	document.getElementById('edit_vpn_crt_client1_static').value = document.getElementById('edit_vpn_crt_client1_static').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client1_ca').value = document.getElementById('edit_vpn_crt_client1_ca').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client1_key').value = document.getElementById('edit_vpn_crt_client1_key').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client1_crt').value = document.getElementById('edit_vpn_crt_client1_crt').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client1_crl').value = document.getElementById('edit_vpn_crt_client1_crl').value.replace(expr,"\r\n");

	document.getElementById('edit_vpn_crt_client2_static').value = document.getElementById('edit_vpn_crt_client2_static').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client2_ca').value = document.getElementById('edit_vpn_crt_client2_ca').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client2_key').value = document.getElementById('edit_vpn_crt_client2_key').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client2_crt').value = document.getElementById('edit_vpn_crt_client2_crt').value.replace(expr,"\r\n");
	document.getElementById('edit_vpn_crt_client2_crl').value = document.getElementById('edit_vpn_crt_client2_crl').value.replace(expr,"\r\n");

}

function update_rgw_options(){
	currentpolicy = document.form.vpn_client_rgw.value;
	iface = document.form.vpn_client_if_x.value;

	if ((iface == "tap") && (currentpolicy == 2)) {
		currentpolicy = 1;
		document.form.vpn_client_rgw.value = 1;
	}

	free_options(document.form.vpn_client_rgw);
	add_option(document.form.vpn_client_rgw, "No","0",(currentpolicy == 0));
	add_option(document.form.vpn_client_rgw, "All","1",(currentpolicy == 1));
	if (iface == "tun")
		add_option(document.form.vpn_client_rgw, "Policy Rules","2",(currentpolicy == 2));
}

function update_visibility(){

	fw = document.form.vpn_client_firewall.value;
	auth = document.form.vpn_client_crypt.value;
	iface = document.form.vpn_client_if_x.value;
	bridge = getRadioValue(document.form.vpn_client_bridge);
	nat = getRadioValue(document.form.vpn_client_nat);
	hmac = document.form.vpn_client_hmac.value;
	rgw = document.form.vpn_client_rgw.value;
	tlsremote = getRadioValue(document.form.vpn_client_tlsremote);
	userauth = (getRadioValue(document.form.vpn_client_userauth) == 1) && (auth == 'tls') ? 1 : 0;
	useronly = userauth && getRadioValue(document.form.vpn_client_useronly);
	adns = document.form.vpn_client_adns.value;
	ncp = document.form.vpn_client_ncp_enable.value;
	dnsf = document.form.dnsfilter_enable_x.value;
	rstrict = document.form.vpn_reverse_strict.value;

	showhide("client_userauth", (auth == "tls"));
	showhide("client_hmac", (auth == "tls"));
	showhide("client_custom_crypto_text", (auth == "custom"));
	showhide("client_tls_crypto_text", (auth != "custom"));         //add by Viz

	showhide("client_username", userauth);
	showhide("client_password", userauth);
	showhide("client_useronly", userauth);

	showhide("client_ca_warn_text", useronly);
	showhide("client_bridge", (iface == "tap"));

	showhide("client_bridge_warn_text", (bridge == 0));
	showhide("client_nat", ((fw != "custom") && (iface == "tun" || bridge == 0)));
	showhide("client_nat_warn_text", ((fw != "custom") && ((nat == 0) || (auth == "secret" && iface == "tun"))));

	showhide("client_local_1", (iface == "tun" && auth == "secret"));
	showhide("client_local_2", (iface == "tap" && (bridge == 0 && auth == "secret")));

	showhide("client_adns", (auth == "tls"));
	showhide("client_reneg", (auth == "tls"));
	showhide("client_gateway_label", (iface == "tap" && rgw > 0));
	showhide("vpn_client_gw", (iface == "tap" && rgw > 0));
	showhide("client_tlsremote", (auth == "tls"));

	showhide("vpn_client_cn", ((auth == "tls") && (tlsremote == 1)));
	showhide("client_cn_label", ((auth == "tls") && (tlsremote == 1)));
	showhide("clientlist_Block", (rgw == 2));
	showhide("selectiveTable", (rgw == 2));
	showhide("client_enforce", (rgw == 2));
	showhide("client_cipher", (ncp != 2));
	showhide("ncp_enable", (auth == "tls"));
	showhide("ncp_ciphers", ((ncp > 0) && (auth == "tls")));

	showhide("dnscrypt_adns", (dnscrypt_proxy == 1));
	showhide("enable_dns_span", (adns >= 3 && rgw == 2 && dnsf == 0));
	showhide("dnscrypt_opt", (adns >= 3 && rgw == 2 && dnsf == 0));
	showhide("dnsfilter_opt", (adns >= 3 && rgw == 2 && dnsf == 1));

	if (rgw == 2) {
		$('dnscrypt_opt').innerHTML = "";
		if (adns == 3) {
			if (dnscrypt_proxy == 1 && dnscrypt_ipv6 == 0 && allow_routelocal)
				$('dnscrypt_opt').innerHTML = "DNSCrypt Resolver";
			else
				$('dnscrypt_opt').innerHTML = "WAN DNS Server";
			showhide("dnscrypt_opt", true);
		} else if (adns == 4) {
			$('dnscrypt_opt').innerHTML = "WAN DNS Server";
			showhide("dnscrypt_opt", true);
		}
		else
			showhide("dnscrypt_opt", false);

		if ((!allow_routelocal || dnscrypt_ipv6 == 1 || adns == 1 || adns == 2) && dnscrypt_proxy == 1) {
			$('dnscrypt_opt').innerHTML = $('dnscrypt_opt').innerHTML + "&nbsp;&nbsp;(DNSCrypt unavailable)";
			showhide("dnscrypt_opt", true);
		}

		if (dnsf == 1) {
			showhide("enable_dns_span", false);
			showhide("dnscrypt_opt", false);
			showhide("dnsfilter_opt", true);
		}
	}

	if (rstrict == 1 && adns == 2) {
		showhide("dnscrypt_opt", true);
		$('dnscrypt_opt').innerHTML = $('dnscrypt_opt').innerHTML + "&nbsp;&nbsp;Reverse Strict set";
	}

// Since instancing certs/keys would waste many KBs of nvram,
// we instead handle these at the webui level, loading both instances.
	showhide("edit_vpn_crt_client1_ca",(openvpn_unit == "1"));
	showhide("edit_vpn_crt_client1_crt", (openvpn_unit == "1"));
	showhide("edit_vpn_crt_client1_crl", (openvpn_unit == "1"));
	showhide("edit_vpn_crt_client1_key",(openvpn_unit == "1"));
	showhide("edit_vpn_crt_client1_static",(openvpn_unit == "1"));
	showhide("edit_vpn_crt_client2_ca",(openvpn_unit == "2"));
	showhide("edit_vpn_crt_client2_crt", (openvpn_unit == "2"));
	showhide("edit_vpn_crt_client2_crl", (openvpn_unit == "2"));
	showhide("edit_vpn_crt_client2_key",(openvpn_unit == "2"));
	showhide("edit_vpn_crt_client2_static",(openvpn_unit == "2"));
}

function dnscrypt_warn(){
	var adns = document.form.vpn_client_adns.value;
	if (dnscrypt_proxy == 1 && (adns == 1 || adns == 2 || adns == 3) && !allow_routelocal)
		alert("WARNING:\nThis DNS configuration will prevent the use of DNSCrypt");
}

function set_Keys(auth){
	cal_panel_block();
	if((auth=='tls') || (auth=="secret")){
		$j("#tlsKey_panel").fadeIn(300);
	}       
}
 
function cancel_Key_panel(auth){
	if((auth == 'tls') || (auth == "secret")){
		this.FromObject ="0";
		$j("#tlsKey_panel").fadeOut(300);	

		if (openvpn_unit == 1) {
			setTimeout("document.getElementById('edit_vpn_crt_client1_static').value = '<% nvram_clean_get("vpn_crt_client1_static"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client1_ca').value = '<% nvram_clean_get("vpn_crt_client1_ca"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client1_crt').value = '<% nvram_clean_get("vpn_crt_client1_crt"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client1_crl').value = '<% nvram_clean_get("vpn_crt_client1_crl"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client1_key').value = '<% nvram_clean_get("vpn_crt_client1_key"); %>';", 300);
		} else {
			setTimeout("document.getElementById('edit_vpn_crt_client2_static').value = '<% nvram_clean_get("vpn_crt_client2_static"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client2_ca').value = '<% nvram_clean_get("vpn_crt_client2_ca"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client2_crt').value = '<% nvram_clean_get("vpn_crt_client2_crt"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client2_crl').value = '<% nvram_clean_get("vpn_crt_client2_crl"); %>';", 300);
			setTimeout("document.getElementById('edit_vpn_crt_client2_key').value = '<% nvram_clean_get("vpn_crt_client2_key"); %>';", 300);
		}
	}

	setTimeout("openvpn_decodeKeys(1);", 400);
}

function save_keys(auth){
	if((auth == 'tls') || (auth == "secret")){
		if (openvpn_unit == "1") {
			if ( check_key(document.getElementById('edit_vpn_crt_client1_static')) &&
			     check_key(document.getElementById('edit_vpn_crt_client1_ca')) &&
			     check_key(document.getElementById('edit_vpn_crt_client1_crt')) &&
			     check_key(document.getElementById('edit_vpn_crt_client1_crl')) &&
			     check_key(document.getElementById('edit_vpn_crt_client1_key')) ){

				document.form.vpn_crt_client1_static.value = document.getElementById('edit_vpn_crt_client1_static').value;
				document.form.vpn_crt_client1_ca.value = document.getElementById('edit_vpn_crt_client1_ca').value;
				document.form.vpn_crt_client1_crt.value = document.getElementById('edit_vpn_crt_client1_crt').value;
				document.form.vpn_crt_client1_crl.value = document.getElementById('edit_vpn_crt_client1_crl').value;
				document.form.vpn_crt_client1_key.value = document.getElementById('edit_vpn_crt_client1_key').value;
				cancel_Key_panel('tls');
			}else {
				alert("One or more key values are not in the correct format!");
			}
		} else {
			if ( check_key(document.getElementById('edit_vpn_crt_client2_static')) &&
			     check_key(document.getElementById('edit_vpn_crt_client2_ca')) &&
			     check_key(document.getElementById('edit_vpn_crt_client2_crt')) &&
			     check_key(document.getElementById('edit_vpn_crt_client2_crl')) &&
			     check_key(document.getElementById('edit_vpn_crt_client2_key')) ){

				document.form.vpn_crt_client2_static.value = document.getElementById('edit_vpn_crt_client2_static').value;
				document.form.vpn_crt_client2_ca.value = document.getElementById('edit_vpn_crt_client2_ca').value;
				document.form.vpn_crt_client2_crt.value = document.getElementById('edit_vpn_crt_client2_crt').value;
				document.form.vpn_crt_client2_crl.value = document.getElementById('edit_vpn_crt_client2_crl').value;
				document.form.vpn_crt_client2_key.value = document.getElementById('edit_vpn_crt_client2_key').value;
				cancel_Key_panel('tls');
			}else {
				alert("One or more key values are not in the correct format!");
			}
		}
	}
}

function check_key(key){
	if(key.value.length == 0)
		return true;
	if(((key.value.indexOf("-----BEGIN")) == 0) ||  ((key.value.indexOf("none")) == 0) || ((key.value.indexOf("/jffs")) == 0)){
		return true;
	} else {
		return false;
	}
}

function cal_panel_block(){
	var blockmarginLeft;
	if (window.innerWidth)
		winWidth = window.innerWidth;
	else if ((document.body) && (document.body.clientWidth))
		winWidth = document.body.clientWidth;
		
	if (document.documentElement  && document.documentElement.clientHeight && document.documentElement.clientWidth){
		winWidth = document.documentElement.clientWidth;
	}

	if(winWidth >1050){	
		winPadding = (winWidth-1050)/2;	
		winWidth = 1105;
		blockmarginLeft= (winWidth*0.15)+winPadding;
	}
	else if(winWidth <=1050){
		blockmarginLeft= (winWidth)*0.15+document.body.scrollLeft;	

	}

	$("tlsKey_panel").style.marginLeft = blockmarginLeft+"px";
}


function applyRule(){

	if (service_state) {
		document.form.action_wait.value = 15;
		document.form.action_script.value = "restart_vpnclient"+openvpn_unit;
	}

	tmp_value = "";

	for (var i=1; i < 3; i++) {
		if (i == openvpn_unit) {
			if (getRadioValue(document.form.vpn_client_x_eas) == 1)
				tmp_value += ""+i+",";
		} else {
			if (document.form.vpn_clientx_eas.value.indexOf(''+(i)) >= 0)
				tmp_value += ""+i+",";
		}
	}
	document.form.vpn_clientx_eas.value = tmp_value;

	document.form.vpn_client_if.value = document.form.vpn_client_if_x.value;

	var rule_num = $('clientlist_table').rows.length;
	var item_num = $('clientlist_table').rows[0].cells.length;
	var tmp_value = "";

	for(i=0; i<rule_num; i++){
		tmp_value += "<";
		for(j=0; j<item_num-1; j++){
			tmp_value += $('clientlist_table').rows[i].cells[j].innerHTML;
			if(j != item_num-2)
				tmp_value += ">";
		}
	}
	if(tmp_value == "<"+"<#IPConnection_VSList_Norule#>" || tmp_value == "<")
		tmp_value = "";
	document.form.vpn_client_clientlist.value = tmp_value;

	if (((enforce_orig != getRadioValue(document.form.vpn_client_enforce)) ||
	     (policy_orig != document.form.vpn_client_rgw.value)) &&
	     (!service_state))
		document.form.action_script.value += "start_vpnrouting"+openvpn_unit;

	document.form.vpn_dns_mode.value = ((document.form.vpn_client_adns.value >= 3 && document.form.vpn_client_rgw.value == 2 && document.form.dnsfilter_enable_x.value == 0) ? ((document.form.enable_dns_ckb.checked) ? 1 : 0) : 0);

	$("vpn_client_password").type = "text"; // workaround for password save prompt in firefox

	document.form.showloading_x.value = (document.form.showloading_x.value == "1") ? "0" : "1"; //force progress bar to always be shown
	showLoading();
	document.form.submit();
}

function change_vpn_unit(val){
	document.form.action_mode.value = "change_vpn_client_unit";
	document.form.action = "apply.cgi";
	document.form.target = "";
	document.form.submit();
}

/* password item show or not */
function pass_checked(obj){
	switchType(obj, document.form.show_pass_1.checked, true);
}

function ImportOvpn(){
	if (document.getElementById('ovpnfile').value == "") return false;

	document.getElementById('importOvpnFile').style.display = "none";
	document.getElementById('loadingicon').style.display = "";

	document.form.action = "vpnupload.cgi";
	document.form.enctype = "multipart/form-data";
	document.form.encoding = "multipart/form-data";

	document.form.submit();
	setTimeout("ovpnFileChecker();",2000);
}

var vpn_upload_state = "init";
function ovpnFileChecker(){
	document.getElementById("importOvpnFile").innerHTML = "<#Main_alert_proceeding_desc3#>";

	$j.ajax({
			url: '/ajax_openvpn_server.asp',
			dataType: 'script',
			timeout: 1500,
			error: function(xhr){
				setTimeout("ovpnFileChecker();",1000);
			},

			success: function(){
				document.getElementById('importOvpnFile').style.display = "";
				document.getElementById('loadingicon').style.display = "none";

				if(vpn_upload_state == "init"){
					setTimeout("ovpnFileChecker();",1000);
				}
				else if(vpn_upload_state > 0){
					document.getElementById("importOvpnFile").innerHTML = "Failed!";
					alert("Error " + vpn_upload_state +" while importing file - invalid key and/or certificate!\nFix your config file, then import it again.");
					setTimeout("location.href='Advanced_OpenVPNClient_Content.asp';", 3000);
				}
				else{
					setTimeout("location.href='Advanced_OpenVPNClient_Content.asp';", 3000);
				}
			}
	});
}

function update_local_ip(object){

	if (object.name == "vpn_client_local_1")
		document.form.vpn_client_local_2.value = object.value;
	else if (object.name == "vpn_client_local_2")
		document.form.vpn_client_local_1.value = object.value;

	document.form.vpn_client_local.value = object.value;
}

function showclientlist(){
	var clientlist_row = clientlist_array.split('&#60');
	var code = "";
	var width = ["24%", "29%", "25%", "10%", "12%"];

	code +='<table width="100%" cellspacing="0" cellpadding="4" align="center" class="list_table" id="clientlist_table">';
	if(clientlist_row.length == 1)
		code +='<tr><td style="color:#FFCC00;" colspan="6"><#IPConnection_VSList_Norule#></td></tr>';
	else{
		for(var i = 1; i < clientlist_row.length; i++){
			code +='<tr id="row'+i+'">';
			var clientlist_col = clientlist_row[i].split('&#62');
				for(var j = 0; j < clientlist_col.length; j++){
					code +='<td width="' + width[j] +'">'+ clientlist_col[j] +'</td>';
				}
				if (j < 4) {
					code +='<td width="' + width[j++] +'">VPN</td>';
				}
				code +='<td width="' + width[j] +'">';

				code +='<input class="remove_btn" onclick="del_Row(this);" value=""/></td></tr>';
		}
	}

  code +='</table>';
	$("clientlist_Block").innerHTML = code;
}

function addRow(obj, head){
	if(head == 1)
		clientlist_array += "&#60" /*&#60*/
	else
		clientlist_array += "&#62" /*&#62*/

	clientlist_array += obj.value;
	obj.value = "";
}

function addRow_Group(upper){
	var rule_num = $('clientlist_table').rows.length;
	var item_num = $('clientlist_table').rows[0].cells.length;
	if(rule_num >= upper){
		alert("<#JS_itemlimit1#> " + upper + " <#JS_itemlimit2#>");
		return false;
	}

	if(document.form.clientlist_ipAddr.value=="")
		document.form.clientlist_ipAddr.value="0.0.0.0";

	if(document.form.clientlist_dstipAddr.value=="")
		document.form.clientlist_dstipAddr.value="0.0.0.0";

	if (!validate_ipcidr(document.form.clientlist_ipAddr)) {
		document.form.clientlist_ipAddr.focus();
		document.form.clientlist_ipAddr.select();
		return false;
	}

	if (!validate_ipcidr(document.form.clientlist_dstipAddr)) {
		document.form.clientlist_dstipAddr.focus();
		document.form.clientlist_dstipAddr.select();
		return false;
	}

	if(item_num >=2){
		for(i=0; i<rule_num; i++){
				if(document.form.clientlist_ipAddr.value.toLowerCase() == $('clientlist_table').rows[i].cells[1].innerHTML.toLowerCase() &&
				   document.form.clientlist_dstipAddr.value.toLowerCase() == $('clientlist_table').rows[i].cells[2].innerHTML.toLowerCase()){
					alert("<#JS_duplicate#>");
					document.form.clientlist_ipAddr.focus();
					document.form.clientlist_ipAddr.select();
					return false;
				}
		}
	}

	addRow(document.form.clientlist_deviceName ,1);
	addRow(document.form.clientlist_ipAddr, 0);
	addRow(document.form.clientlist_dstipAddr, 0);
	addRow(document.form.clientlist_iface, 0);
	document.form.clientlist_iface.value = "VPN";
	showclientlist();
}

function del_Row(r){
	var i=r.parentNode.parentNode.rowIndex;
	$('clientlist_table').deleteRow(i);

	var clientlist_value = "";
	for(k=0; k<$('clientlist_table').rows.length; k++){
		clientlist_value += "&#60";
		clientlist_value += $('clientlist_table').rows[k].cells[0].innerHTML;
		clientlist_value += "&#62";
		clientlist_value += $('clientlist_table').rows[k].cells[1].innerHTML;
		clientlist_value += "&#62";
		clientlist_value += $('clientlist_table').rows[k].cells[2].innerHTML;
		clientlist_value += "&#62";
		clientlist_value += $('clientlist_table').rows[k].cells[3].innerHTML;
	}

	clientlist_array = clientlist_value;
	if(clientlist_array == "")
		showclientlist();
}

function showLANIPList(){
	var htmlCode = "";
	var show_name = "";
	var show_ip = "";
	var client_list_array = '<% get_client_detail_info(); %>';
	var client_list_row = client_list_array.split('<');

	for(var i = 1; i < client_list_row.length; i++){
		var client_list_col = client_list_row[i].split('>');
		if(client_list_col[1] && client_list_col[1].length > 20)
			show_name = client_list_col[1].substring(0, 16) + "..";
		else
			show_name = client_list_col[1];
		show_ip = client_list_col[2];

		htmlCode += '<a><div onmouseover="over_var=1;" onmouseout="over_var=0;" onclick="setClientIP(\'';
		htmlCode += show_name;
		htmlCode += '\', \'';
		htmlCode += show_ip;
		htmlCode += '\');"><strong>';
		htmlCode += show_name;
		htmlCode += '</strong> ('+show_ip+')</div></a><!--[if lte IE 6.5]><iframe class="hackiframe2"></iframe><![endif]-->';


	}

	$("ClientList_Block_PC").innerHTML = htmlCode;
}

function setClientIP(_name, _ipaddr){
	document.form.clientlist_deviceName.value = _name;
	document.form.clientlist_ipAddr.value = _ipaddr;
	hideClients_Block();
	over_var = 0;
}

var over_var = 0;
var isMenuopen = 0;
function hideClients_Block(){
	$("pull_arrow").src = "/images/arrow-down.gif";
	$('ClientList_Block_PC').style.display='none';
	isMenuopen = 0;
}

function pullLANIPList(obj){
	if(isMenuopen == 0){
		obj.src = "/images/arrow-top.gif"
		$("ClientList_Block_PC").style.display = 'block';
		document.form.clientlist_deviceName.focus();
		isMenuopen = 1;
	}
	else
		hideClients_Block();
}

function defaultSettings() {
	if (confirm("WARNING: This will reset this OpenVPN client to factory default settings!\n\nKeys and certificates associated to this instance will also be DELETED!\n\nProceed?")) {
		document.form.action_script.value = "stop_vpnclient" + openvpn_unit + ";clearvpnclient" + openvpn_unit;
		parent.showLoading();
		document.form.submit();
	} else {
		return false;
	}
}

</script>
</head>

<body onload="initial();" onunLoad="return unload_body();">
	<div id="tlsKey_panel"  class="contentM_qis" style="box-shadow: 3px 3px 10px #000;">
		<table class="QISform_wireless" border=0 align="center" cellpadding="5" cellspacing="0">
			<tr>
				<div class="description_down">Keys and Certificates</div>
			</tr>
			<tr>
				<div style="margin-left:30px; margin-top:10px;">
					<p>Only paste the content of the <span style="color:#FFCC00;">----- BEGIN xxx ----- </span>/<span style="color:#FFCC00;"> ----- END xxx -----</span> block (including those two lines).

					<p>Alternatively, enter the path to the location of the key or certificate on JFFS. For example, <span style="color:#FFCC00;">/jffs/openvpn/client1/ca.crt</span>
					<p>Limit: 3499 characters per field
				</div>
				<div style="margin:5px;*margin-left:-5px;"><img style="width: 730px; height: 2px;" src="/images/New_ui/export/line_export.png"></div>
			</tr>			
			<!--===================================Beginning of tls Content===========================================-->

		    <tr>
				<td valign="top">
			   		<table width="700px" border="0" cellpadding="4" cellspacing="0">
			        	<tbody>
							<tr>
					        	<td valign="top">
									<table width="100%" id="page1_tls" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable">
										<tr>
											<th>Static Key</th>
											<td>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client1_static" name="edit_vpn_crt_client1_static" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client1_static"); %></textarea>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client2_static" name="edit_vpn_crt_client2_static" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client2_static"); %></textarea>
											</td>
										</tr>
										<tr>
											<th id="manualCa">Certificate Authority</th>
											<td>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client1_ca" name="edit_vpn_crt_client1_ca" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client1_ca"); %></textarea>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client2_ca" name="edit_vpn_crt_client2_ca" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client2_ca"); %></textarea>
											</td>
										</tr>
										<tr>
											<th id="manualCert">Client Certificate</th>
											<td>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client1_crt" name="edit_vpn_crt_client1_crt" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client1_crt"); %></textarea>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client2_crt" name="edit_vpn_crt_client2_crt" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client2_crt"); %></textarea>
											</td>
										</tr>
										<tr>
											<th id="manualKey">Client Key</th>
											<td>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client1_key" name="edit_vpn_crt_client1_key" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client1_key"); %></textarea>
												<textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client2_key" name="edit_vpn_crt_client2_key" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client2_key"); %></textarea>
											</td>
										</tr>
										<tr>
                                                                                        <th id="manualCrl">Certificate Revocation List (Optional)</th>
                                                                                        <td>
                                                                                                <textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client1_crl" name="edit_vpn_crt_client1_crl" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client1_crl"); %></textarea>
                                                                                                <textarea rows="8" class="textarea_ssh_table" id="edit_vpn_crt_client2_crl" name="edit_vpn_crt_client2_crl" cols="65" maxlength="3499"><% nvram_clean_get("vpn_crt_client2_crl"); %></textarea>
                                                                                        </td>
                                                                                </tr>
									</table>
								</td>
							</tr>						
						</tbody>						
	  				</table>
					<div style="margin-top:5px;width:100%;text-align:center;">
						<input class="button_gen" type="button" onclick="cancel_Key_panel('tls');" value="<#CTL_Cancel#>">
						<input class="button_gen" type="button" onclick="save_keys('tls');" value="<#CTL_onlysave#>">	
					</div>					
				</td>
			</tr>
      <!--===================================Ending of tls Content===========================================-->			

		</table>		
	</div>

<div id="TopBanner"></div>

<div id="Loading" class="popup_bg"></div>

<iframe name="hidden_frame" id="hidden_frame" src="" width="0" height="0" frameborder="0"></iframe>

<form method="post" name="form" id="ruleForm" action="/start_apply.htm" target="hidden_frame">
<input type="hidden" name="current_page" value="Advanced_OpenVPNClient_Content.asp">
<input type="hidden" name="next_page" value="Advanced_OpenVPNClient_Content.asp">
<input type="hidden" name="modified" value="0">
<input type="hidden" name="action_mode" value="apply">
<input type="hidden" name="action_script" value="">
<input type="hidden" name="action_wait" value="5">
<input type="hidden" name="first_time" value="">
<input type="hidden" name="SystemCmd" value="">
<input type="hidden" name="preferred_lang" id="preferred_lang" value="<% nvram_get("preferred_lang"); %>">
<input type="hidden" name="firmver" value="<% nvram_get("firmver"); %>">
<input type="hidden" name="showloading_x" value="<% nvram_get("showloading_x"); %>">
<input type="hidden" name="vpn_clientx_eas" value="<% nvram_clean_get("vpn_clientx_eas"); %>">
<input type="hidden" name="vpn_dns_mode" value="<% nvram_clean_get("vpn_dns_mode"); %>">
<input type="hidden" name="vpn_crt_client1_ca" value="<% nvram_clean_get("vpn_crt_client1_ca"); %>">
<input type="hidden" name="vpn_crt_client1_crt" value="<% nvram_clean_get("vpn_crt_client1_crt"); %>">
<input type="hidden" name="vpn_crt_client1_crl" value="<% nvram_clean_get("vpn_crt_client1_crl"); %>">
<input type="hidden" name="vpn_crt_client1_key" value="<% nvram_clean_get("vpn_crt_client1_key"); %>">
<input type="hidden" name="vpn_crt_client1_static" value="<% nvram_clean_get("vpn_crt_client1_static"); %>">
<input type="hidden" name="vpn_crt_client2_ca" value="<% nvram_clean_get("vpn_crt_client2_ca"); %>">
<input type="hidden" name="vpn_crt_client2_crt" value="<% nvram_clean_get("vpn_crt_client2_crt"); %>">
<input type="hidden" name="vpn_crt_client2_crl"value="<% nvram_clean_get("vpn_crt_client2_crl"; %>">
<input type="hidden" name="vpn_crt_client2_key" value="<% nvram_clean_get("vpn_crt_client2_key"); %>">
<input type="hidden" name="vpn_crt_client2_static" value="<% nvram_clean_get("vpn_crt_client2_static"); %>">
<input type="hidden" name="vpn_upload_type" value="ovpn">
<input type="hidden" name="vpn_upload_unit" value="<% nvram_get("vpn_client_unit"); %>">
<input type="hidden" name="vpn_client_if" value="<% nvram_get("vpn_client_if"); %>">
<input type="hidden" name="vpn_client_local" value="<% nvram_get("vpn_client_local"); %>">
<input type="hidden" name="vpn_client_clientlist" value="<% nvram_clean_get("vpn_client_clientlist"); %>">
<input type="hidden" name="dnsfilter_enable_x" value="<% nvram_get("dnsfilter_enable_x"); %>">
<input type="hidden" name="vpn_reverse_strict" value="<% nvram_get("vpn_reverse_strict"); %>">

<table class="content" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="17">&nbsp;</td>
    <td valign="top" width="202">
      <div id="mainMenu"></div>
      <div id="subMenu"></div>
    </td>
    <td valign="top">
        <div id="tabMenu" class="submenuBlock"></div>

      <!--===================================Beginning of Main Content===========================================-->
      <table width="98%" border="0" align="left" cellpadding="0" cellspacing="0">
        <tr>
          <td valign="top">
            <table width="760px" border="0" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTitle" id="FormTitle">
                <tbody>
                <tr bgcolor="#4D595D">
                <td valign="top">
                <div>&nbsp;</div>
                <div class="formfonttitle">OpenVPN Client Settings</div>
                <div style="margin-left:5px;margin-top:10px;margin-bottom:10px"><img src="/images/New_ui/export/line_export.png"></div>
		<div class="formfontdesc">
                        <p>Before starting the service make sure you properly configure it, including
                           the required keys,<br>otherwise you will be unable to turn it on.
                        <p><br>In case of problem, see the <a style="font-weight: bolder;text-decoration:underline;" class="hyperlink" href="Main_LogStatus_Content.asp">System Log</a> for any error message related to openvpn.
                </div>
		<span id="ipv6warning" style="color:#FFCC00;display:none;"><br>&nbsp;&nbsp;WARNING: You are running in a dual stack environemnt (IPv4 / IPv6). &nbsp;IPv6 enabled websites may bypass the VPN.<br>&nbsp;&nbsp;To ensure all traffic is routed through the VPN, disable IPv6 on the client or the router.<br>&nbsp;</span>

				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable">
					<thead>
						<tr>
							<td colspan="2">Client control</td>
						</tr>
					</thead>
					<tr id="client_unit">
						<th>Select client instance</th>
						<td>
			        		<select name="vpn_client_unit" class="input_option" onChange="change_vpn_unit(this.value);">
								<option value="1" <% nvram_match("vpn_client_unit","1","selected"); %> >Client 1</option>
								<option value="2" <% nvram_match("vpn_client_unit","2","selected"); %> >Client 2</option>
							</select>
			   			</td>
					</tr>
					<tr id="service_enable_button">
						<th>Service state</th>
						<td>
							<div class="left" style="width:94px; float:left; cursor:pointer;" id="radio_service_enable"></div>
							<script type="text/javascript">

								$j('#radio_service_enable').iphoneSwitch(service_state,
									 function() {
										document.form.action_wait.value = 15;
										document.form.action_script.value = "start_vpnclient" + openvpn_unit;
										document.form.showloading_x.value = (document.form.showloading_x.value == "1") ? "0" : "1";
										parent.showLoading();
										document.form.submit();
										return true;
									 },
									 function() {
										document.form.action_wait.value = 15;
										document.form.action_script.value = "stop_vpnclient" + openvpn_unit;
										document.form.showloading_x.value = (document.form.showloading_x.value == "1") ? "0" : "1";
										parent.showLoading();
										document.form.submit();
										return true;
									 },
									 {
										switch_on_container_path: '/switcherplugin/iphone_switch_container_off.png'
									 }
								);
							</script>
							<span>Warning: any unsaved change will be lost.</span>
					    </td>
					</tr>
					<tr>
							<th>Import ovpn file</th>
						<td>
							<input id="ovpnfile" type="file" name="file" class="input" style="color:#FFCC00;*color:#000;">
							<input id="" class="button_gen" onclick="ImportOvpn();" type="button" value="<#CTL_upload#>" />
								<img id="loadingicon" style="margin-left:5px;display:none;" src="/images/InternetScan.gif">
								<span id="importOvpnFile" style="display:none;"><#Main_alert_proceeding_desc3#></span>
						</td>
					</tr>

				</table>

				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable">
					<thead>
						<tr>
							<td colspan="2">Basic Settings</td>
						</tr>
					</thead>

					<tr>
						<th>Start with WAN</th>
						<td>
							<input type="radio" name="vpn_client_x_eas" class="input" value="1"><#checkbox_Yes#>
							<input type="radio" name="vpn_client_x_eas" class="input" value="0"><#checkbox_No#>
						</td>
 					</tr>

					<tr>
						<th>Interface Type</th>
			        		<td>
							<select name="vpn_client_if_x"  onclick="update_rgw_options();update_visibility();" class="input_option">
							</select>
			   			</td>
					</tr>

					<tr>
						<th>Protocol</th>
			        		<td>
			       				<select name="vpn_client_proto" class="input_option">
								<option value="tcp-client" <% nvram_match("vpn_client_proto","tcp-client","selected"); %> >TCP</option>
								<option value="udp" <% nvram_match("vpn_client_proto","udp","selected"); %> >UDP</option>
							</select>
			   			</td>
					</tr>

					<tr>
						<th>Server Address and Port</th>
						<td>
							<label>Address:</label><input type="text" maxlength="128" class="input_25_table" name="vpn_client_addr" value="<% nvram_get("vpn_client_addr"); %>">
							<label style="margin-left: 4em;">Port:</label><input type="text" maxlength="5" class="input_6_table" name="vpn_client_port" onKeyPress="return is_number(this,event);" onblur="validate_number_range(this, 1, 65535)" value="<% nvram_get("vpn_client_port"); %>" >
						</td>
					</tr>

					<tr>
						<th>Firewall</th>
			        	<td>
			        		<select name="vpn_client_firewall" class="input_option" onclick="update_visibility();" >
								<option value="auto" <% nvram_match("vpn_client_firewall","auto","selected"); %> >Automatic</option>
								<option value="custom" <% nvram_match("vpn_client_firewall","custom","selected"); %> >Custom</option>
							</select>
			   			</td>
					</tr>

					<tr>
						<th>Authorization Mode</th>
			        	<td>
			        		<select name="vpn_client_crypt" class="input_option" onclick="update_visibility();">
								<option value="tls" <% nvram_match("vpn_client_crypt","tls","selected"); %> >TLS</option>
								<option value="secret" <% nvram_match("vpn_client_crypt","secret","selected"); %> >Static Key</option>
								<option value="custom" <% nvram_match("vpn_client_crypt","custom","selected"); %> >Custom</option>
							</select>
							<span id="client_tls_crypto_text" onclick="set_Keys('tls');" style="text-decoration:underline;cursor:pointer;">Content modification of Keys &amp; Certificates.</span>
							<span id="client_custom_crypto_text">(Must be manually configured!)</span>
			   			</td>
					</tr>

					<tr id="client_userauth">
						<th>Username/Password Authentication</th>
						<td>
							<input type="radio" name="vpn_client_userauth" class="input" value="1" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_userauth", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_userauth" class="input" value="0" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_userauth", "0", "checked"); %>><#checkbox_No#>
						</td>
 					</tr>

					<tr id="client_username">
						<th>Username</th>
						<td>
							<input type="text" maxlength="50" class="input_25_table" name="vpn_client_username" value="<% nvram_get("vpn_client_username"); %>" >
						</td>
					</tr>
					<tr id="client_password">
						<th>Password</th>
						<td>
							<input type="password" readonly maxlength="50" class="input_25_table" id="vpn_client_password" name="vpn_client_password" value="<% nvram_get("vpn_client_password"); %>" onFocus="$(this).removeAttribute('readonly');">
							<input type="checkbox" name="show_pass_1" onclick="pass_checked(document.form.vpn_client_password)"><#QIS_show_pass#>
						</td>
					</tr>
					<tr id="client_useronly">
						<th>Username Auth. Only<br><i>(Must define certificate authority)</i></th>
						<td>
							<input type="radio" name="vpn_client_useronly" class="input" value="1" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_useronly", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_useronly" class="input" value="0" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_useronly", "0", "checked"); %>><#checkbox_No#>
							<span id="client_ca_warn_text">Warning: You must define a Certificate Authority.</span>
						</td>
 					</tr>

					<tr id="client_hmac">
						<th>TLS control channel security<br><i>(tls-auth / tls-crypt)</i></th>
			        	<td>
			        		<select name="vpn_client_hmac" class="input_option">
								<option value="-1" <% nvram_match("vpn_client_hmac","-1","selected"); %> >Disabled</option>
								<option value="2" <% nvram_match("vpn_client_hmac","2","selected"); %> >Bi-directional Auth</option>
								<option value="0" <% nvram_match("vpn_client_hmac","0","selected"); %> >Incoming Auth (0)</option>
								<option value="1" <% nvram_match("vpn_client_hmac","1","selected"); %> >Outgoing Auth (1)</option>
								<option value="3" <% nvram_match("vpn_client_hmac","3","selected"); %> >Encrypt channel</option>
							</select>
			   			</td>
					</tr>

					<tr id="client_bridge">
						<th>Server is on the same subnet</th>
						<td>
							<input type="radio" name="vpn_client_bridge" class="input" value="1" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_bridge", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_bridge" class="input" value="0" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_bridge", "0", "checked"); %>><#checkbox_No#>
							<span id="client_bridge_warn_text">Warning: Cannot bridge distinct subnets. Will default to routed mode.</span>
						</td>
 					</tr>

					<tr id="client_nat">
						<th>Create NAT on tunnel<br><i>(Router must be configured manually)</i></th>
						<td>
							<input type="radio" name="vpn_client_nat" class="input" value="1" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_nat", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_nat" class="input" value="0" onclick="update_visibility();" <% nvram_match_x("", "vpn_client_nat", "0", "checked"); %>><#checkbox_No#>
							<span id="client_nat_warn_text">Routes must be configured manually.</span>

						</td>
 					</tr>

					<tr id="client_local_1">
						<th>Local/remote endpoint addresses</th>
						<td>
							<input type="text" maxlength="15" class="input_15_table" name="vpn_client_local_1" onkeypress="return is_ipaddr(this, event);" onblur="update_local_ip(this);" value="<% nvram_get("vpn_client_local"); %>">
							<input type="text" maxlength="15" class="input_15_table" name="vpn_client_remote" onkeypress="return is_ipaddr(this, event);" value="<% nvram_get("vpn_client_remote"); %>">
						</td>
					</tr>

					<tr id="client_local_2">
						<th>Tunnel address/netmask</th>
						<td>
							<input type="text" maxlength="15" class="input_15_table" name="vpn_client_local_2" onkeypress="return is_ipaddr(this, event);" onblur="update_local_ip(this);" value="<% nvram_get("vpn_client_local"); %>">
							<input type="text" maxlength="15" class="input_15_table" name="vpn_client_nm" onkeypress="return is_ipaddr(this, event);" value="<% nvram_get("vpn_client_nm"); %>">
						</td>
					</tr>
				</table>

				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" bordercolor="#6b8fa3"  class="FormTable">
					<thead>
						<tr>
							<td colspan="2">Advanced Settings</td>
						</tr>
					</thead>

					<tr>
						<th>Poll Interval<br><i>(in minutes, 0 to disable)</i></th>
						<td>
							<input type="text" maxlength="4" class="input_6_table" name="vpn_client_poll" onKeyPress="return is_number(this,event);" onblur="validate_number_range(this, 0, 1440)" value="<% nvram_get("vpn_client_poll"); %>">
						</td>
					</tr>

					<tr id="client_adns">
						<th>Accept DNS Configuration</th>
						<td>
							<select name="vpn_client_adns" class="input_option" onclick="update_visibility();dnscrypt_warn();">
								<option value="0" <% nvram_match("vpn_client_adns","0","selected"); %> >Disabled</option>
								<option value="1" <% nvram_match("vpn_client_adns","1","selected"); %> >Relaxed</option>
								<option value="2" <% nvram_match("vpn_client_adns","2","selected"); %> >Strict</option>
								<option value="3" <% nvram_match("vpn_client_adns","3","selected"); %> >Exclusive</option>
								<option id="dnscrypt_adns" value="4" <% nvram_match("vpn_client_adns","4","selected"); %> >DNSCrypt</option>
							</select>
							<span id="enable_dns_span"><input type="checkbox" name="enable_dns_ckb" id="enable_dns_ckb" value="" style="margin-left:20px;" onclick="document.form.vpn_dns_mode.value=(this.checked==true)?1:0;">WAN clients use</input></span>
							<span id="dnscrypt_opt">Unknown</span>
							<span id="dnsfilter_opt">&nbsp;&nbsp;Clients use DNSFilter</span>
			   			</td>
					</tr>

					<tr id="ncp_enable">
						<th><a class="hintstyle" href="javascript:void(0);" onClick="openHint(48,1);">Cipher Negotiation</a></th>
						<td>
							<select name="vpn_client_ncp_enable" onclick="update_visibility();" class="input_option">
								<option value="0" <% nvram_match("vpn_client_ncp_enable","0","selected"); %> >Disabled</option>
								<option value="1" <% nvram_match("vpn_client_ncp_enable","1","selected"); %> >Enable (with fallback)</option>
								<option value="2" <% nvram_match("vpn_client_ncp_enable","2","selected"); %> >Enable</option>
							</select>
						</td>
					</tr>

					<tr id="ncp_ciphers">
						<th>Negotiable ciphers</th>
						<td>
							<input type="text" maxlength="255" class="input_32_table" name="vpn_client_ncp_ciphers" value="<% nvram_get("vpn_client_ncp_ciphers"); %>" >
						</td>
					</tr>

					<tr id="client_cipher">
						<th>Legacy/fallback cipher</th>
			        	<td>
			        		<select name="vpn_client_cipher" class="input_option">
								<option value="<% nvram_get("vpn_client_cipher"); %>" selected><% nvram_get("vpn_client_cipher"); %></option>
							</select>
			   			</td>
					</tr>

					<tr>
						<th>Auth digest</th>
						<td>
							<select name="vpn_client_digest" class="input_option">
								<option value="<% nvram_get("vpn_client_digest"); %>" selected><% nvram_get("vpn_client_digest"); %></option>
							</select>
						</td>
					</tr>

					<tr>
						<th>Compression</th>
			        	<td>
			        		<select name="vpn_client_comp" class="input_option">
								<option value="-1" <% nvram_match("vpn_client_comp","-1","selected"); %> >Disabled</option>
								<option value="no" <% nvram_match("vpn_client_comp","no","selected"); %> >None</option>
								<option value="yes" <% nvram_match("vpn_client_comp","yes","selected"); %> >LZO</option>
								<option value="adaptive" <% nvram_match("vpn_client_comp","adaptive","selected"); %> > LZO Adaptive</option>
								<option value="lz4" <% nvram_match("vpn_client_comp","lz4","selected"); %> >LZ4</option>
 							</select>
			   			</td>
					</tr>

					<tr id="client_reneg">
						<th>TLS Renegotiation Time<br><i>(in seconds, -1 for default)</i></th>
						<td>
							<input type="text" maxlength="5" class="input_6_table" name="vpn_client_reneg" onblur="validate_range(this, -1, 2147483647)" value="<% nvram_get("vpn_client_reneg"); %>">
						</td>
					</tr>

					<tr>
						<th>Connection Retry<br><i>(in seconds, -1 for infinite)</i></th>
						<td>
							<input type="text" maxlength="5" class="input_6_table" name="vpn_client_retry" onblur="validate_range(this, -1, 32767)" value="<% nvram_get("vpn_client_retry"); %>">
						</td>
					</tr>

					<tr id="client_tlsremote">
						<th>Verify Server Certificate</th>
						<td>
							<input type="radio" name="vpn_client_tlsremote" class="input" onclick="update_visibility();" value="1" <% nvram_match_x("", "vpn_client_tlsremote", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_tlsremote" class="input" onclick="update_visibility();" value="0" <% nvram_match_x("", "vpn_client_tlsremote", "0", "checked"); %>><#checkbox_No#>
							<label style="padding-left:3em;" id="client_cn_label">Common name:</label><input type="text" maxlength="64" class="input_25_table" id="vpn_client_cn" name="vpn_client_cn" value="<% nvram_get("vpn_client_cn"); %>">
						</td>
 					</tr>
					<tr>
						<th>Redirect Internet traffic</th>
						<td colspan="2">
							<select name="vpn_client_rgw" class="input_option" onChange="update_visibility();">
							</select>
							<label style="padding-left:3em;" id="client_gateway_label">Gateway:</label><input type="text" maxlength="15" class="input_15_table" id="vpn_client_gw" name="vpn_client_gw" onkeypress="return validate_ipaddr_final(this, event);" value="<% nvram_get("vpn_client_gw"); %>">
						</td>
					</tr>
					<tr id="client_enforce">
						<th>Block routed clients if tunnel goes down</th>
						<td>
							<input type="radio" name="vpn_client_enforce" class="input" value="1" <% nvram_match_x("", "vpn_client_enforce", "1", "checked"); %>><#checkbox_Yes#>
							<input type="radio" name="vpn_client_enforce" class="input" value="0" <% nvram_match_x("", "vpn_client_enforce", "0", "checked"); %>><#checkbox_No#>
						</td>
					</tr>
				</table>

				<table id="selectiveTable" width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;">
					<thead>
						<tr>
							<td colspan="5">Rules for routing client traffic through the tunnel (<#List_limit#>&nbsp;128)</td>
						</tr>
					</thead>
					<tr>
						<th><#IPConnection_autofwDesc_itemname#></th>
						<th>Source IP</th>
						<th>Destination IP</th>
						<th>Iface</th>
						<th><#list_add_delete#></th>
					</tr>
					<tr>
						<td width="24%">
							<input type="text" class="input_15_table" maxlength="15" name="clientlist_deviceName" onClick="hideClients_Block();" onkeypress="return is_alphanum(this,event);">
						</td>
						<td width="29%">
							<input type="text" class="input_18_table" maxlength="18" name="clientlist_ipAddr">
							<img id="pull_arrow" height="14px;" src="/images/arrow-down.gif" style="position:absolute;*margin-left:-3px;*margin-top:1px;" onclick="pullLANIPList(this);" title="<#select_device_name#>" onmouseover="over_var=1;" onmouseout="over_var=0;">
							<div id="ClientList_Block_PC" class="ClientList_Block_PC"></div>
						</td>
						<td width="25%">
							<input type="text" class="input_18_table" maxlength="18" name="clientlist_dstipAddr">
						</td>
						<td width="10%">
							<select name="clientlist_iface" class="input_option">
								<option value="WAN">WAN</option>
								<option value="VPN" selected>VPN</option>
							</select>
						</td>
						<td width="12%">
							<div>
								<input type="button" class="add_btn" onClick="addRow_Group(128);" value="">
							</div>
						</td>
					</tr>
				</table>
				<div id="clientlist_Block"></div>

				<table width="100%" border="1" align="center" cellpadding="4" cellspacing="0" class="FormTable_table" style="margin-top:8px;">
					<thead>
						<tr>
							<td>Custom Configuration</td>
						</tr>
					</thead>
					<tr>
						<td>
							<textarea rows="8" class="textarea_ssh_table" name="vpn_client_custom" cols="55" maxlength="15000"><% nvram_clean_get("vpn_client_custom"); %></textarea>
						</td>
					</tr>
					</table>
					<div class="apply_gen">
						<input type="button" id="restoreButton" class="button_gen" value="<#Setting_factorydefault_value#>" onclick="defaultSettings();">
						<input name="button" type="button" class="button_gen" onclick="applyRule();" value="<#CTL_apply#>"/>
			        </div>
				</td></tr>
	        </tbody>
            </table>
            </form>
            </td>

       </tr>
      </table>
      <!--===================================Ending of Main Content===========================================-->
    </td>
    <td width="10" align="center" valign="top">&nbsp;</td>
  </tr>
</table>
<div id="footer"></div>
</body>
</html>

