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
#ifndef GL_ASSERT_H
#define GL_ASSERT_H

/**
 *
 * OpenGL error management class.
 *
 */

#ifndef NDEBUG
// debug mode

#include <iostream>
#include <cassert>

#ifdef __APPLE__
// MacOsX stuff
#define glAssert(code) \
    code; \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__STRING(code)<<\
                ( (err == GL_INVALID_ENUM) ? " Invalid enum" : \
                ( (err == GL_INVALID_VALUE) ? " Invalid value" : \
                ( (err == GL_INVALID_OPERATION) ? " Invalid operation" : "unknown error") \
                ) \
                )\
                << "("<<err<<")"<<std::endl; \
            assert(false); \
        } \
    }

#define glCheckError() \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<\
    ( (err == GL_INVALID_ENUM) ? " Invalid enum" : \
    ( (err == GL_INVALID_VALUE) ? " Invalid value" : \
    ( (err == GL_INVALID_OPERATION) ? " Invalid operation" : "unknown error") \
    ) \
    )\
    << "("<<err<<")"<<std::endl; \
            assert(false); \
        } \
    }

#else
// Linux stuff
#define glAssert(code) \
    code; \
{\
    GLuint err = glGetError(); \
    if (err != GL_NO_ERROR) { \
    std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__STRING(code)<<") : "<<(const char*)gluErrorString (err)<<"("<<err<<")"<<std::endl; \
    } \
    }

#define glCheckError() \
    {\
        GLuint err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            std::cerr<<"erreur OpenGL ("<<__FILE__<<":"<<__LINE__<<", "<<__STRING()<<") : "<<(const char*)gluErrorString (err)<<"("<<err<<")"<<std::endl; \
          } \
    }

#endif

#else

// No debug
#define glAssert(code) \
    code;

#endif



#endif
