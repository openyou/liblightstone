---
layout: post
project: liblightstone
title: Coming soon to a liblightstone near you - Win32 support again
---

After yanking win32 HID support 6 months ago, I finally decided to take the time to sit down and see if I could get HID working again with the WDK. Low and behold, I was just missing some include paths, and the code worked mostly unchanged. I'll be folding the Win32 code back into libnputil, which means liblightstone and libomron should both work on windows again soon. And god, I really do hate the WDK.
