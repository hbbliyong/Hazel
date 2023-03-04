#include "hzpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>
namespace Hazel {

  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
  {
    HZ_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
  {
    glCreateBuffers(1, &m_RendererId);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    glDeleteBuffers(1, &m_RendererId);
  }
  void OpenGLVertexBuffer::Bind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
  }
  void OpenGLVertexBuffer::Unbind() const
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    :m_Count(count)
  {
    glCreateBuffers(1, &m_RendererID);
    // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
       // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }
  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    glDeleteBuffers(1, &m_RendererID);
  }
  void OpenGLIndexBuffer::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }
  void OpenGLIndexBuffer::UnBind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}