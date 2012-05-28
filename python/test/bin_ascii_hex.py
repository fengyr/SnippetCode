import binascii

print repr(binascii.a2b_hex('0099'))
print binascii.b2a_hex('\x00\x99')
