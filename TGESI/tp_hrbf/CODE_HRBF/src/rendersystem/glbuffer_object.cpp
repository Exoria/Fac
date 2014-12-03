#include "glbuffer_object.h"

// -----------------------------------------------------------------------------

GlBuffer_obj::GlBuffer_obj(int type) :
    _is_cuda_registered(false),
    _size_buffer(0),
    _type(type)
{
    _data_ratio = guess_data_ratio(_type);
    glAssert( glGenBuffers(1, &_buffer_id) );
}

// -----------------------------------------------------------------------------

GlBuffer_obj::GlBuffer_obj(int nb_elt, int type, GLenum mode):
    _is_cuda_registered(false),
    _size_buffer(nb_elt),
    _type(type)
{
    _data_ratio = guess_data_ratio(type);
    glAssert( glGenBuffers(1, &_buffer_id) );
    glAssert( glBindBuffer(_type, _buffer_id) );
    glAssert( glBufferData(_type, nb_elt * _data_ratio, NULL, mode) );
    glAssert( glBindBuffer(_type, 0) );
}

// -----------------------------------------------------------------------------

GlBuffer_obj::~GlBuffer_obj() {
    assert(!_is_cuda_registered);
    glAssert( glDeleteBuffers(1, &_buffer_id) );
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::bind() const {
    glAssert( glBindBuffer(_type, _buffer_id) );
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::unbind() const {
    glAssert( glBindBuffer(_type, 0) );
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::set_data(int nb_elt,
                            const GLvoid* data,
                            GLenum mode)
{
    bind();
    _size_buffer = nb_elt;
    glAssert( glBufferData(_type, nb_elt * _data_ratio, data, mode) );
    unbind();
}

// -----------------------------------------------------------------------------

void GlBuffer_obj::get_data(int offset,
                            int nb_elt,
                            GLvoid* data) const
{
    bind();
    glAssert( glGetBufferSubData(_type, offset, nb_elt * _data_ratio, data) );
    unbind();
}

// -----------------------------------------------------------------------------

bool GlBuffer_obj::unmap() const {
    bind();
    bool state = glUnmapBuffer(_type) ? true : false;
    return state;
}

// -----------------------------------------------------------------------------
