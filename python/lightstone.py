#!/usr/bin/env python

from contextlib import contextmanager, closing
import sys
from libhid import *
import os
import re

class lightstone(object):
    # VID : PID
    LIGHTSTONE_ID_LIST = { 0x0483 : 0x0035, 0x14FA : 0x0001 }    

    hrv = 0.0
    scl = 0.0
    hid = None
    rawMsg = None

    def __init__(self):
        self._init_usb()
        self.hid = hid_new_HIDInterface()
        return

    def _init_usb(self):
        ret = hid_init()
        if ret != HID_RET_SUCCESS:
            sys.stderr.write("hid_init failed with return code %d.\n" % ret)

    @classmethod
    def open(cls):
        l = lightstone()
        l._open()
        return l

    def _open(self):
        matcher = HIDInterfaceMatcher()
        matcher.vendor_id = 0x0483
        matcher.product_id = 0x0035

        ret = hid_force_open(self.hid, 0, matcher, 3)
        if ret != HID_RET_SUCCESS:
            sys.stderr.write("hid_force_open failed with return code %d.\n" % ret)
            return False

    @contextmanager
    def closing(self):
        self.close()

    def close(self):
        ret = hid_close(self.hid)
        if ret != HID_RET_SUCCESS:
            sys.stderr.write("hid_close failed with return code %d.\n" % ret)
            return False
        hid_cleanup()
	return True
    
    def get_data(self):
        ret = 0
        message_finished = False
        while not message_finished:
            InputStruct = hid_interrupt_read(self.hid,0x81,0x8,10);            
            if InputStruct[0] != HID_RET_SUCCESS:
                continue
            InputReport = InputStruct[1]
            for msg_index in range(1, ord(InputReport[0]) + 1):
                if self.rawMsg is None and InputReport[msg_index] != '<':
                    continue
                elif self.rawMsg is None:
                    self.rawMsg = ''
                if InputReport[msg_index] != '\r' and InputReport[msg_index] != '\n':
                    self.rawMsg += InputReport[msg_index]
                elif InputReport[msg_index] == '\n':
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

