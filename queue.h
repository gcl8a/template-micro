//queue class, started 2004-12-07
//based on arrays, with an index pointer to next

#ifndef __TEMPLATE_QUEUE_H
#define __TEMPLATE_QUEUE_H

#ifndef __TEMPLATE_ARRAY_H
#include <template/TList.h>
#endif


template < class T >
class TQueue : public TList < T >
{
protected:
   T * _tIndex;

public:
   T & Add( const T & t );

   T & operator [] ( int i )
   {
      if ( _tIndex + i > _tData = GetSize() ) return _tIndex + i - GetSize();
      else
         return _tIndex + i;
   }

   T & Front( void ) { return * _tIndex; }

   T & Back( void )
   {
      return _tIndex > _tData ? * ( _tIndex - 1 ) :
      * ( _tIndex + GetSize() - 1 );
   }
};


template < class T >
T & TQueue < T >::Add( const T & t )
{
   _tIndex++ = t;
   if ( _tIndex > _tData + GetSize() ) _tIndex = _tData;
   return * _tIndex;
}


#endif
