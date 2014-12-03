/***************************************************************************
 *   Copyright (C) 2012 by Mathias Paulin                                  *
 *   Mathias.Paulin@irit.fr                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

namespace rendersystem {

/**
  * @ingroup RenderSystem
  * A simple timer.
  *
  * @bug Only for Linux/UNIX/MacosX
  */
class Timer {
public:

    typedef double Time;

    Timer() : mLast ( 0. ) {
        reset();
    }

    ~Timer() {
    }

    inline void start ( void ) {
        mLast = getTime();
    }
    inline void stop ( void ) {
        mSum += getTime() - mLast;
    }
    inline void reset ( void ) {
        mSum = 0.;
    }

    inline Time value ( void ) const {
        return mSum;
    }

    static Time getTime ( void ) {
        struct timeval tv;
        struct timezone tz;
        gettimeofday ( &tv, &tz );
        return ( Time ) ( ( double ) tv.tv_sec + 1.e-6 * ( double ) tv.tv_usec );
    }

protected:

    Time mSum, mLast;

};

}
#endif
