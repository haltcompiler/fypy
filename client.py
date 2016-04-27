import containerblock
import sys

creds = containerblock.getCredentials(sys.argv[1])
print creds[0], creds[1]