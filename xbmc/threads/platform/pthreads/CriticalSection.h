/*
 *      Copyright (C) 2005-2011 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include <pthread.h>
#include <errno.h>

#include "threads/Lockables.h"
#include "threads/Helpers.h"

namespace XbmcThreads
{

  // forward declare in preparation for the friend declaration
  class ConditionVariable;

  namespace pthreads
  {
    class RecursiveMutex
    {
      pthread_mutexattr_t* getRecusiveAttr();
      pthread_mutex_t mutex;

      // needs acces to 'mutex'
      friend class XbmcThreads::ConditionVariable;
    public:
      inline RecursiveMutex()
      {
        XBMC_ASSERT_ZERO(pthread_mutex_init(&mutex,getRecusiveAttr()));
      }
      
      inline ~RecursiveMutex()
      {
        XBMC_ASSERT_ZERO(pthread_mutex_destroy(&mutex));
      }

      inline void lock()
      {
        XBMC_ASSERT_ZERO(pthread_mutex_lock(&mutex));
      }

      inline void unlock()
      {
        XBMC_ASSERT_ZERO(pthread_mutex_unlock(&mutex));
      }
        
      inline bool try_lock()
      {
        int pthread_mutex_trylock_result = pthread_mutex_trylock(&mutex);
        assert(pthread_mutex_trylock_result == 0 || pthread_mutex_trylock_result == EBUSY);
        return !pthread_mutex_trylock_result;
      }
    };
  }
}


/**
 * A CCriticalSection is a CountingLockable whose implementation is a 
 *  native recursive mutex.
 *
 * This is not a typedef because of a number of "class CCriticalSection;" 
 *  forward declarations in the code that break when it's done that way.
 */
class CCriticalSection : public XbmcThreads::CountingLockable<XbmcThreads::pthreads::RecursiveMutex> {};

