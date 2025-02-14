/* Copyright (c) 2020-21 Project re-Isearch and its contributors: See CONTRIBUTORS.
It is made available and licensed under the Apache 2.0 license: see LICENSE */
#pragma ident  "@(#)reclist.cxx"

/*-@@@
File:		reclist.cxx
Description:	Class RECLIST - Database Record List
@@@*/

#include <stdio.h>
#include "reclist.hxx"
#include "common.hxx"
#include "magic.hxx"

#define INCREMENT 1000

RECLIST::RECLIST ()
{
  MaxEntries = 0;
  Table = NULL;
  TotalEntries = 0;
}

void RECLIST::AddEntry (const RECORD& RecordEntry)
{
  if (TotalEntries == MaxEntries)
    Expand ();
  Table[TotalEntries++] = RecordEntry;
}

bool RECLIST::GetEntry (const size_t Index, PRECORD RecordEntry) const
{
  if ((Index > 0) && (Index <= TotalEntries))
    {
      *RecordEntry = Table[Index - 1];
      return true;
    }
  return false;
}

void RECLIST::Expand ()
{
  Resize (TotalEntries + INCREMENT);
}

void RECLIST::CleanUp ()
{
  Resize (TotalEntries);
}

void RECLIST::Resize (const size_t Entries)
{
  PRECORD OldTable = Table;
  MaxEntries = Entries;
  Table = new RECORD[MaxEntries];

  size_t RecsToCopy;
  if (Entries >= TotalEntries)
    {
      RecsToCopy = TotalEntries;
    }
  else
    {
      RecsToCopy = Entries;
      TotalEntries = Entries;
    }
  for (size_t x = 0; x < RecsToCopy; x++)
    {
      Table[x] = OldTable[x];
    }
  if (OldTable)
    delete[]OldTable;
}

size_t RECLIST::GetTotalEntries () const
{
  return TotalEntries;
}

void RECLIST::Write(PFILE Fp) const
{
  putObjID (objRECLIST, Fp);
  ::Write((UINT4)TotalEntries, Fp); // Write count
  for (size_t i = 0; i < TotalEntries; i++)
    {
      Table[i].Write(Fp);
    }
}

bool RECLIST::Read(PFILE Fp)
{
  UINT4 Entries = 0;
  obj_t obj = getObjID (Fp);
  if (obj != objRECLIST)
    {
      PushBackObjID (obj, Fp);
    }
  else
    {
      ::Read(&Entries, Fp); // Get count
      PRECORD OldTable = Table;
      MaxEntries = Entries;
      Table = new RECORD[MaxEntries];

      for (size_t i=0; i < Entries; i++)
	{
	  Table[i].Read(Fp);
	}
      if (OldTable) delete[] OldTable;
    }
  TotalEntries = Entries;
  return obj == objRECLIST;
}

RECLIST::~RECLIST ()
{
  if (Table)
    delete[]Table;
}

