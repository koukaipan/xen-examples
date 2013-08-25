#!/bin/sh
dom_id=`xm domid Simplest_Kernel`
echo "Making /local/domain/$dom_id writable..."
sudo xenstore-chmod "/local/domain/$dom_id" b

