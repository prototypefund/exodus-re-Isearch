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

query = sys.argv[1:] and sys.argv[1] or 'hong kong ADJ'
junk="/tmp/XML";
pdb = VIDB(junk);
print "This is PyIB version %s/%s" % (string.split(sys.version)[0], pdb.GetVersionID());
print "Copyright (c) 1999 Basis Systeme netzwerk/Munich";
if not pdb.IsDbCompatible():
  raise ValueError, "The specified database '%s' is not compatible with this version. Re-index!" % `junk`

elements = pdb.GetTotalRecords();

print "Database ", junk, " has ", elements, " elements";

if elements > 0:
	irset = pdb.SearchRpn(query, ByNewsrank); # RPN Query
	total = irset.GetTotalEntries();
	print "Searching for: ", query;
	print "Got = ", total, " Records with", irset.GetHitTotal(), "hits"; 

	start = 1;
	rset = irset.Fill(start);
  	total = rset.GetTotalEntries();
	print "RSET from Fill(", start, ") has ", total, "elements";


	# Print the results....
	for i in range(1,total+1):
	  result = rset.GetEntry(i);
	  area = pdb.Context(result, "___", "____") ;
	  datum = result.GetDate();

	  score  = result.GetScore();
	  hits   = result.GetHitTable();
	  print "[", i , "] ", rset.GetScaledScore(score, 100), " ", score, " ", pdb.Present(result, ELEMENT_Brief);
	  print "\tFormat: ", result.GetDoctype();
	  print "\tFile:", result.GetFullFileName(), "  [", result.GetRecordStart(), "-", result.GetRecordEnd(), "]";
	  print "\tDate: ", datum.RFCdate();
	  print "\tMatch: ", area;
