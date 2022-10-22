//2012.01.06: TArray (TList) constructor added
//06.21.06: removed xerror.h
//12/06/04: added _iIndex and AddCurrent, which rolls through the array
//iIndex is the index of the last added entry
//12/06/04: changed Add and AddAt to return references. Still could be dangerous.
//11/03/04: change Add and AddAt to return pointers in TArray. Could be dangerous.
//11/02/04: added AddNoRepeat to TIArray, which only adds if the pointer isn't already in the array
//09/28/04: added FindIndex (similar to Find, but returns the index)
//01/09/03: changed NO_VALIDATEs to VALIDATEs
/* 11/11/02: fixed some ANSI incompatibilities 1/23/02: added some functionality back to TIArrayExt
9/4/01: added skeleton TIArrayExt, which allows for offset base indeces.
Much of the functionality has been lost, and some may be changed from that of TIArray */
#ifndef __TEMPLATE_ARRAY_H
#define __TEMPLATE_ARRAY_H

//#include <iostream>

//#include "template/xerror.h"
//#include "template/list.h"

using namespace std;

template < class T > class TArray
{
private:
   unsigned _nSize;
   T * _tData;
   int _iIndex; //seems to track stepping??? Wow. What a mess!!!!

public:
   TArray( void )
   {
      _nSize = 0;
      _tData = 0;
      _iIndex = 1;
   }

   TArray( unsigned nSize );
   TArray( unsigned nSize, const T & t );
   TArray( const TArray & tArray );
   //TArray( TList < T > &);

   TArray & operator = ( const TArray & tArray );

   virtual ~TArray( void )
   {
      if ( _tData )
         delete[] _tData;
   }

   T & operator[] ( unsigned i ) const
   {
#ifdef __VALIDATE__
      if ( i < 0 || i >= _nSize )
         throw XError( "Array out-of-bounds" );
#endif
      return _tData[i];
   }

   T & GetLast( void )
   { return _tData[_nSize - 1]; }

   unsigned GetSize( void )const
   { return _nSize; }

   //   int GetMax( void )const
   //   { return _nSize - 1; }

   T & Add( const T & );
   T & AddAt( const T &, unsigned );
   T & AddIndexed( const T & );

   T & Current( unsigned i = 0 ) const
   {
      int index = _iIndex + i;
      if ( index >= GetSize() ) index -= GetSize(); return ( * this ) [index];
   }

   unsigned IsFull( void ) { return _iIndex == ( GetSize() - 1 ); }

   unsigned Resize( unsigned );
protected:
   unsigned Expand( unsigned );
   unsigned Contract( unsigned );

public:
   unsigned Clear( void );

   unsigned Destroy( void )
   {
      if ( _tData )
      { delete[] _tData; }

      _tData = 0;
      _iIndex = 1;
      _nSize = 0;

      return 0;
   }

   T * Find( const T & ) const;
   unsigned FindIndex( const T & ) const;
   int operator == ( const TArray < T > & ) const;

//     ostream & operator << (ostream & , const TArray<T>&);
   //  friend istream& operator >> <>(istream&, TArray<T>&);
};

template < class T >
TArray < T >::TArray( unsigned nSize )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   _iIndex = nSize;
}

template < class T >
TArray < T >::TArray( unsigned nSize, const T & t )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = t; }
   _iIndex = nSize;
}

template < class T >
TArray < T >::TArray( const TArray < T > & tArray )
{
   _nSize = tArray._nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = tArray._tData[i]; }
   _iIndex = tArray._iIndex;
}

template < class T >
TArray < T > & TArray < T >::operator = ( const TArray < T > & tArray )
{
   _nSize = tArray._nSize;
   if ( _tData )
      delete[] _tData;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = tArray._tData[i]; }
   _iIndex = tArray._iIndex;
   return * this;
}

template < class T >
T & TArray < T >::Add( const T & t )
{
   Expand( 1 );
   ( * this ) [GetSize() - 1] = t;
   return _tData[GetSize() - 1];
}

template < class T >
T & TArray < T >::AddAt( const T & t, unsigned i )
{
   if ( i > _nSize )
   { Expand( i - _nSize - 1 ); }
   ( * this ) [i] = t;
   return _tData[i];
}

template < class T >
T & TArray < T >::AddIndexed( const T & t )
{
   _iIndex++;
   if ( _iIndex >= GetSize() ) _iIndex = 0;
   return AddAt( t, _iIndex );
}

template < class T >
unsigned TArray < T >::Resize( unsigned nNewSize )
{
   if ( nNewSize > _nSize ) Expand( nNewSize - _nSize );
   return GetSize();
}

template < class T >
unsigned TArray < T >::Expand( unsigned nBy )
{
   if ( nBy > 0 )
   {
      TArray < T > tTemp( * this );
      unsigned nOldSize = _nSize;
      _nSize += nBy;
      if ( _tData ) delete[] _tData;
      _tData = new T[_nSize];
      for ( unsigned i = 0; i < nOldSize; i++ )
      { _tData[i] = tTemp[i]; }
   }
   return GetSize();
}

template<class T>
unsigned TArray<T>::Contract(unsigned nBy)
{
	if (nBy > 0)
	{
		TArray<T> tTemp(*this);

		if (nBy > _nSize)
			_nSize = 0;
		else
			_nSize -= nBy;

		if (_tData)
		{
			delete[] _tData;
			_tData = 0;
		}
		if (_nSize)
			_tData = new T[_nSize];
		for (unsigned i = 0; i < _nSize; i++)
		{
			_tData[i] = tTemp[i];
		}
	}
	return GetSize();
}

template < class T >
T * TArray < T >::Find( const T & t ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( _tData[i] == t ) return & _tData[i]; }
   return 0;
}

template < class T >
unsigned TArray < T >::FindIndex( const T & t ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( _tData[i] == t ) return i; }
   return -1; //return value is unsigned!
}

template < class T >
int TArray < T >::operator == ( const TArray < T > & tArray ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( !( _tData[i] == tArray[i] ) ) return 0; }
   return 1;
}
/*
template < class T >
istream & operator >> ( istream & stream, TArray < T > & tArray )
{
   int nSize;
   stream >> nSize;

   for ( unsigned i = 0; i < nSize; i++ )
   {
      T t;
      stream >> t;
      tArray.Add( t );
   }
   return stream;
}

template < class T >
ostream & operator << ( ostream & stream, const TArray < T > & tArray )
{
   stream << tArray.GetSize() << endl;
   for ( unsigned i = 0; i < tArray.GetSize(); i++ )
   {
      if ( i ) stream << '\t';
      stream << tArray[i];
   }

   return stream;
}
*/

#endif
