from Crypto.Cipher import AES
import struct
import sys
import hashlib

ext = '.cbk'
key = '0123456789012345'
IV = '0123456789012345'
containername = ''

def generateFilename(stream):
	return hashlib.md5(stream).hexdigest()


def genContainerStream(username, password):
	userlen = struct.pack('<B', len(username))
	passlen = struct.pack('<B', len(password))

	plainStream = userlen + passlen + username + password
	plainStreamlen = struct.pack('<B', len(plainStream))

	mode = AES.MODE_CBC
	encryptor = AES.new(key, mode, IV=IV)

	if len(plainStream) % 16 != 0:
	    	plainStream += ' ' * (16 - len(plainStream) % 16)
	
	cipherStream = encryptor.encrypt(plainStream)

	finalStream = plainStreamlen + IV + cipherStream

	containername = generateFilename(finalStream)
	
	with open(containername + ext, 'wb') as containerFile:
		containerFile.write(finalStream)
		containerFile.close()

	print '[+] container built !!'
	print '[+] File name :: %s' %containername


def main():
	print '[+] Reading parameters.....',

	if len(sys.argv) > 1:
		print 'done'
	
	print '[+] Generating container....'
	
	genContainerStream(sys.argv[1], sys.argv[2])
	print 'done'

if __name__ == '__main__':
	main()