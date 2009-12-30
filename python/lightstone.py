#!/usr/bin/env python
#################################################################
# lightstone controller object
# By Kyle Machulis <kyle@nonpolynomial.com>
# http://www.nonpolynomial.com
#
# Distributed as part of the liblightstone project
#
# Website: http://qdot.github.com/liblightstone
# Repo: http://www.github.com/qdot/liblightstone
#
# Licensed under the BSD License, as follows
#
# Copyright (c) 2009, Kyle Machulis/Nonpolynomial Labs
# All rights reserved.
#
# Redistribution and use in source and binary forms, 
# with or without modification, are permitted provided 
# that the following conditions are met:
#
#    * Redistributions of source code must retain the 
#      above copyright notice, this list of conditions 
#      and the following disclaimer.
#    * Redistributions in binary form must reproduce the 
#      above copyright notice, this list of conditions and 
#      the following disclaimer in the documentation and/or 
#      other materials provided with the distribution.
#    * Neither the name of the Nonpolynomial Labs nor the names 
#      of its contributors may be used to endorse or promote 
#      products derived from this software without specific 
#      prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#################################################################


from contextlib import contextmanager, closing
import sys
import usb
import os
import re

class lightstone(object):
    # VID : PID
    LIGHTSTONE_ID_LIST = { 0x0483 : 0x0035, 0x14FA : 0x0001 }    

    hrv = 0.0
    scl = 0.0
    hid = None
    rawMsg = None
    lightstone_device = None

    def __init__(self):
        return

    @classmethod
    def open(cls):
        l = lightstone()
        l._open()
        return l

    def _open(self):
        for key, value in self.LIGHTSTONE_ID_LIST.items():
            self.lightstone_device = usb.core.find(idVendor = key, idProduct = value)
            if self.lightstone_device is not None:
                break
        if self.lightstone_device is None:
            return False
        #self.lightstone_device.claim_interface(0)        
        self.lightstone_device.set_configuration(1)

    @contextmanager
    def closing(self):
        self.close()

    def close(self):
        if self.lightstone_device is not None:
            self.lightstone_device = None
	return True
    
    def get_data(self):
        ret = 0
        message_finished = False
        while not message_finished:
            InputStruct = self.lightstone_device.read(0x81,8, 0, 10)
            InputReport = InputStruct
            for msg_index in range(1, InputReport[0] + 1):
                current_ch = chr(InputReport[msg_index])
                if self.rawMsg is None and current_ch != '<':
                    continue
                elif self.rawMsg is None:
                    self.rawMsg = ''
                if current_ch not in ('\r', '\n'):
                    self.rawMsg += current_ch
                elif current_ch == '\n':
                    raw_re = re.compile("\<RAW\>(?P<scl>[0-9A-Fa-f]{4}) (?P<hrv>[0-9A-Fa-f]{4})\<\\\\RAW\>")
                    result = re.search(raw_re, self.rawMsg)
                    if result:
                        self.scl = int(result.group("scl"), 16) * .01
                        self.hrv = int(result.group("hrv"), 16) * .001
                    message_finished = True;
                    self.rawMsg = None
    
def main(argv = None):
    with closing(lightstone.open()) as l:
        try:
            while(1):
                l.get_data()
                print "%f %f" % (l.scl, l.hrv)
        except KeyboardInterrupt, e:
            print "Exiting"

if __name__ == '__main__':
    sys.exit(main())

