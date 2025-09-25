#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
# TODO : change creation date
Created on Wed Jan 17 09:50:33 2024

# TODO : change author
@author: brett
"""

import subprocess

"""
takes a hexa challenge as input
returns the signature related to the challenge and my pvkey
"""
def autoSign(chall):
    # write the challenge in file, to avoid the '\n' char at the end of string
    f = open("chall.txt", "w") # TODO : make sure the name file matches
    f.write("")
    f.write(chall)
    f.close()
    
    # openssl command to sign the challenge, reading chall in "chall.txt" and printing in hex format
    # TODO : change 'key_arlandis.pem' with your private key file
    cmd = "openssl dgst -sha256 -sign private_key.pem < chall.txt -hex"
    return_code = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    
    # truncate the given output to only return the hexa number
    res = ""
    for line in return_code.stdout.readlines():
        res = str(line[17:])
    return res
    
    
    
    
    