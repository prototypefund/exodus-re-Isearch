/* Copyright (c) 2020-21 Project re-Isearch and its contributors: See CONTRIBUTORS.
It is made available and licensed under the Apache 2.0 license: see LICENSE */
/*-@@@
File:		dft.cxx
Description:	Class DFT - Data Field Table
@@@*/

#include "common.hxx"
#include "dft.hxx"

#pragma ident  "@(#)dft.cxx"
#define INCREMENT 50


DFT::DFT ()
{
  MaxEntries = 0;
  Table = NULL;
  TotalEntries = 0;
}

DFT::DFT (const DFT& OtherDft)
{
  MaxEntries = 0;
  *this = OtherDft;
}

DFT& DFT::operator =(const DFT& OtherDft)
{
  const size_t OtherTotal = OtherDft.GetTotalEntries ();
  if (OtherTotal > MaxEntries)
    {
      MaxEntries = OtherTotal;
      if (Table)
	delete[]Table;
      TotalEntries = 0;
      Table = new DF[MaxEntries];
    }
  for (TotalEntries = 0; TotalEntries < OtherTotal; TotalEntries++)
    {
      Table[TotalEntries] = OtherDft.Table[TotalEntries];
    }
  return *this;
}

void DFT::AddEntry (const DF& DfRecord)
{
#if 1
  FastAddEntry(DfRecord);
#else
  const STRING Field (DfRecord.GetFieldName());
  for (size_t i = 0; i < TotalEntries; i++)
    {
      if (Field == Table[i].GetFieldName())
	{
	  Table[i].AddFct(DfRecord.GetFcListPtr());
	  return;
	}
    }
  if (TotalEntries == MaxEntries)
    {
      Expand ();
    }
  Table[TotalEntries++] = DfRecord;
#endif
}

void DFT::FastAddEntry (const DF& DfRecord)
{
  if (!DfRecord.Ok())
    return; // Don't Add Empty fields // edz 16 Feb 2003

  if (TotalEntries == MaxEntries)
    {
      Expand ();
    }
  Table[TotalEntries++] = DfRecord;
}


bool DFT::GetEntry (const size_t Index, DF *DfRecord) const
{
  if ((Index > 0) && (Index <= TotalEntries))
    {
      *DfRecord = Table[Index - 1];
      return true;
    }
  return false;
}


const DF *DFT::GetEntryPtr(const size_t Index) const
{
 if ((Index > 0) && (Index <= TotalEntries))
    return &Table[Index - 1];
  return NULL;
}

STRING DFT::GetFieldName (const size_t Index) const
{
  if ((Index > 0) && (Index <= TotalEntries))
    return Table[Index - 1].GetFieldName();
  return NulString;
}


const FCLIST *DFT::GetFcListPtr(const size_t Index) const
{
  if ((Index > 0) && (Index <= TotalEntries))
    {
      return Table[Index - 1].GetFcListPtr();
    }
  return NULL;
}

void DFT::Expand ()
{
  Resize ((TotalEntries<<1) + INCREMENT);
}

void DFT::CleanUp ()
{
  // Shrink
  Resize (TotalEntries);
}

void DFT::Resize (const size_t Entries)
{
  DF *OldTable = Table;
  MaxEntries = Entries;
  Table = new DF[MaxEntries];

  TotalEntries = (Entries >= TotalEntries) ? TotalEntries : Entries;
  for (size_t i = 0; i < TotalEntries; i++)
    {
      Table[i] = OldTable[i];
    }
  if (OldTable)
    delete[]OldTable;
}

size_t DFT::GetTotalEntries () const
{
  return TotalEntries;
}

void DFT::Write (PFILE fp) const
{
  putObjID (objDFT, fp);
  ::Write ((UINT2)TotalEntries, fp);
  for (size_t i = 0; i < TotalEntries; i++)
    {
      Table[i].Write (fp);
    }
}

bool DFT::Read (PFILE fp)
{
  DFT Dft;

  obj_t obj = getObjID (fp);
  if (obj != objDFT)
    {
      PushBackObjID (obj, fp);
    }
  else
    {
      UINT2 NewTotal;
      DF Df;

      ::Read(&NewTotal, fp);
      for (size_t i = 0; i < NewTotal; i++)
	{
	  Df.Read (fp);
	  Dft.AddEntry (Df);
	}
    }
  *this = Dft;
  return obj == objDFT;
}

DFT::~DFT ()
{
  if (Table)
    delete[]Table;
}

