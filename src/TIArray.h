//2014.10.15: Ported TIArray<> to Arduino herein. TArray<> is now found in TArray.h
//Arduino doesn't do exceptions, so XError removed. Need to add some error checking back
//Did not port TISArray or TIArrayExt
//Noted that, contrary to stated, TIArray does NOT own anything by default

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
#ifndef __TEMPLATE_TIARRAY_H
#define __TEMPLATE_TIARRAY_H

//#include <iostream>

//#include "template/xerror.h"
//#include "template/list.h"

#include <Arduino.h>

using namespace std;

/*Template class TIArray holds an array of object pointers to object
type T. TIArray assumes no ownership of all objects added to it.
*/

template < class T >
class TIArray
{
private:
   int _nSize; //size of the array
   T** _tiData; //array of object pointers

protected:
   boolean TestIndex( int i ) const
   {
      if ( i < 0 || i >= _nSize )
         return false;
	  else return true;
   }

   int Expand( int );
   int Contract( int );

public:
   TIArray( void )
   {
      _nSize = 0;
      _tiData = 0;
   }
   TIArray( int nSize );
   TIArray( const TIArray & );

   TIArray & operator = ( const TIArray & );

   virtual ~TIArray( void );

   T * & operator[] ( int i ) const
   {
#ifdef __VALIDATE__
      if(!TestIndex( i )) return null;
#endif
      return _tiData[i];
   }

   virtual int Add( T * );
//   virtual int AddNoRepeat( T * );
//   virtual int AddAt( T *, int );

   int Delete( int );
   int Destroy( int );

   int GetSize( void )const
   { return _nSize; }

   int GetItemsInContainer( void )const;

   int FindIndex( T * ) const;

   int Resize( int );

   int Empty( void );
   int Flush( void );
   int Clear( void );
   int Destroy( void );

   boolean TestNullInvariant( void )const;
   boolean TestInvariant( void )const;
};



template < class T >
boolean TIArray < T >::TestInvariant( void )const
{
   if ( _tiData && !_nSize )
      return false;
   if ( !_tiData && _nSize )
      return false;
   if ( _nSize < 0 )
      return false;
   return true;
}

template < class T >
boolean TIArray < T >::TestNullInvariant( void )const /* ensure that there are no NULL pointers */
{
   if(!TestInvariant()) return false;
   for ( int i = 0; i < _nSize; i++ )
   {
      if ( !_tiData[i] )
         return false;
   }
   return true;
}

template < class T >
TIArray < T >::TIArray( int nSize )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tiData = new T * [_nSize];
   else
      _tiData = 0;
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = 0; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArray < T >::TIArray( const TIArray < T > & tiArray )
{
   _nSize = tiArray._nSize;
   if ( _nSize > 0 )
      _tiData = new T * [_nSize];
   else
      _tiData = 0;
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = tiArray._tiData[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArray < T >::~TIArray( void )
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   if ( _tiData )
   { delete[] _tiData; }
}

template < class T >
TIArray < T > & TIArray < T >::operator = ( const TIArray < T > & tiArray )
{
   _nSize = tiArray._nSize;
   if ( _tiData )
   {
      delete[] _tiData;
      _tiData = 0;
   }
   if ( _nSize > 0 ) _tiData = new T * [_nSize];
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = tiArray._tiData[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return * this;
}

template < class T >
int TIArray < T >::GetItemsInContainer( void )const
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   int nCount = 0;
   for ( int i = 0; i < GetSize(); i++ )
   { if ( operator[] ( i ) ) nCount++; }
   return nCount;
}

template < class T >
int TIArray < T >::FindIndex( T * pt ) const
{
   for ( int i = 0; i < _nSize; i++ )
   { if ( _tiData[i] == pt ) return i; }
   return -1;
}

template < class T >
int TIArray < T >::Add( T * pT )
{
   int i = 0;
   for ( ; i < _nSize; i++ )
   {
      if ( !_tiData[i] )
      {
         _tiData[i] = pT;
         return i;
      }
   }
   Expand( 1 );
   operator[] ( i ) = pT;
   return i;
}
/*
template < class T >
int TIArray < T >::AddNoRepeat( T * pT )
{
   int i = 0;
   //first check to see if this pointer exists
   for ( ; i < _nSize; i++ )
      if ( _tiData[i] == pT ) return i;

   //if not, look for an open spot
   for ( ; i < _nSize; i++ )
      if ( !_tiData[i] )
      {
         _tiData[i] = pT;
         return i;
      }

   //if no open spots, expand one
   Expand( 1 );
   operator[] ( i ) = pT;
   return i;
}
*/
template < class T >
int TIArray < T >::Delete( int i )
{
   if ( operator[] ( i ) )
   {
      delete _tiData[i];
      operator[] ( i ) = 0;
   }
   return GetItemsInContainer();
}

template < class T >
int TIArray < T >::Destroy( int index ) /*deletes an item and removes the slot from the
array, moving everything
else back as necessary */
{
   //delete item
   Delete( index );

   //slide remaining items up one
   int i = index;
   for ( ; i < _nSize - 1; i++ )
   { _tiData[i] = _tiData[i + 1]; }

   //make last index null
   _tiData[i] = 0;

   //remove last index
   Contract( 1 );
   return i;
}

template < class T >
int TIArray < T >::Empty( void ) /* Empties the entire array without deleting the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = 0; }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

template < class T >
int TIArray < T >::Flush( void ) /* Empties the array and deletes all of the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   {
      if ( _tiData[i] )
      {
         delete _tiData[i];
         _tiData[i] = 0;
      }
   }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

template < class T >
int TIArray < T >::Clear( void ) /* destroys entire array without deleting data pointers */
{
   if ( _tiData )
   {
      Empty();
      delete[] _tiData;
      _tiData = 0;
      _nSize = 0;
   }
   return 0;
}

template < class T >
int TIArray < T >::Destroy( void ) /* destroys entire array , deleting the
pointers in the process */
{
   if ( _tiData )
   {
      Flush();
      delete[] _tiData;
      _tiData = 0;
      _nSize = 0;
   }

   return 0;
}

template < class T >
int TIArray < T >::Resize( int nNewSize )
{
   if ( nNewSize > _nSize ) Expand( nNewSize - _nSize );
   else if ( nNewSize < _nSize ) Contract( _nSize - nNewSize );
   return _nSize;
}

template < class T >
int TIArray < T >::Expand( int nBy )
{
   if ( nBy > 0 )
   {
      if ( !_tiData )
      {
         _nSize = nBy;
         _tiData = new T * [_nSize];
         for ( int i = 0; i < _nSize; i++ )
         { _tiData[i] = 0; }
         return _nSize;
      }
      else
      {
         TIArray < T > tiTemp( * this );
         _nSize += nBy;
         delete[] _tiData;
         _tiData = new T * [_nSize];
         int i = 0;
         for ( ; i < _nSize - nBy; i++ )
         { _tiData[i] = tiTemp[i]; }
         for ( ; i < _nSize; i++ )
         { _tiData[i] = 0; }
      }
   }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return _nSize;
}

template < class T >
int TIArray < T >::Contract( int nBy )
{
   if ( nBy > _nSize )
      return 0;
   if ( nBy > 0 )
   {
      TIArray < T > tiTemp( * this );
      int nOldSize = _nSize;
      _nSize -= nBy;

      if ( _tiData )
      {
         delete[] _tiData;
         _tiData = 0;
      }
      if ( _nSize ) _tiData = new T * [_nSize];

      //copy those data not lost in contraction
      int i = 0;
      for ( ; i < _nSize; i++ )
      { _tiData[i] = tiTemp[i]; }

      for ( ; i < nOldSize; i++ )
      {
         if ( tiTemp[i] )
            return 0;
      }
   }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return _nSize;
}

#endif
