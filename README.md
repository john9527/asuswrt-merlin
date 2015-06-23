asuswrt-merlin (Forked from 374.43 by john9527)
==============

This is a fork of [asuswrt-merlin](https://github.com/RMerl/asuswrt-merlin) by [RMerl](https://github.com/RMerl), which is an enhanced version of [Asuswrt](http://event.asus.com/2013/nw/ASUSWRT/), the official firmware on [Asus routers](http://www.asus.com/Networking/).

The goal of this firmware is to provide a stable/updated version of asuswrt-merlin before [Asus locked](http://www.snbforums.com/threads/asus-locking-down-routers-to-comply-with-new-fcc-rules.18762/) (in version 376_xxxx) the ability to raise the transmission power of the router above 80mW, Asus did this to comply with FCC regulations.

The custom features of the fork which are not exposed in the gui can be set by an nvram variable. All the custom features are documented inside the `Merlin_Fork_Options` file, which you can find in the Dowload link below.

A factory default reset is NOT required if coming from any level of the fork or Merlin 374.42 or 374.43 code. Coming from any other level does require a factory default reset after the code is loaded.

####Differences with [asuswrt-merlin](https://github.com/RMerl/asuswrt-merlin)

The fork includes:

    Maintenance for documented security issues
    Maintenance for supporting open source components (such as dnsmasq, miniupnpd, etc)
    Backports of applicable fixes and new functions from Merlin's main branch
    Some unique support for options requested by users
    Older versions of the wireless drivers that some feel offer better performance (especially on the MIPS based routers)
    A different IPv6 stack which may work better in some environments
    Less of a lockdown on tweaking power levels

The fork does not include:

    The new TrendMicro DPI engine functions for ARM routers
    The enhancements to the networkmap for custom icons, client naming, etc.
    Some of the enhanced gui formatting of later releases, for instance the new wireless log
    All the changes/tweaks that ASUS may have made since the original code was released (and any newly introduced bugs :) )

####Supported routers

* RT-N16
* RT-N66U
* RT-AC66U
* RT-AC56U
* RT-AC68U
* RT-AC68P (and the retail and color versions, R and W)

_Note ; The following routers were released after the base code used for this fork was available, and are NOT supported:_

* RT-AC87U
* RT-AC3200 (and the retail R versions)

####Download links

Compiled binaries for the supported routers are available [here](http://1drv.ms/1uChm3J), if this link does not work, see the [official thread](http://www.snbforums.com/threads/fork-update-for-374-43-available-v12.18914/).

Source code releases are available [here](https://github.com/john9527/asuswrt-merlin/releases).

####Support.

For any questions / comments/ feedback / issues, report them on this [thread](http://www.snbforums.com/threads/fork-update-for-374-43-available-v12.18914/).

####Contributors.

See the contributors page [here](https://github.com/john9527/asuswrt-merlin/graphs/contributors).

####License

[GNU General Public License version 2](https://github.com/john9527/asuswrt-merlin/blob/master/License)


