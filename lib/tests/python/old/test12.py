#!/opt/BSN/bin/python
#-------------------------------------------------
#ident  "%Z%%Y%%M%  %I% %G% %U% BSN"

import sys
import string
from IB import *


#############################################################################
##
##
#############################################################################

query = sys.argv[1:] and sys.argv[1] or 'mon'
field = sys.argv[2:] and sys.argv[2] or ''
junk="/opt/BSN/data/REUTERS";
pdb = VIDB(junk);
print "This is PyIB version %s/%s" % (string.split(sys.version)[0], pdb.GetVersionID());
print "Copyright (c) 1999 Basis Systeme netzwerk/Munich";
if not pdb.IsDbCompatible():
  raise ValueError, "The specified database '%s' is not compatible with this version. Re-index!" % `junk`

elements = pdb.GetTotalRecords();

print "Database ", junk, " has ", elements, " elements";

words = pdb.Scan(query, field); 

total = words.GetTotalEntries();

print query+"*", "WITHIN:"+field; 
for i in range(0,total):
  print words[i];

