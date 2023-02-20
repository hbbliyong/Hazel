#pragma once

#include "Hazel/Renderer/VertexArray.h"
namespace Hazel{
class OpenGLVertexArray:public VertexArray
{
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void Bind() const override;
  virtual void UnBind() const override;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

  virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

private:
  uint32_t m_RendererID;
  uint32_t m_VertexBufferIndex = 0;
  std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
  std::shared_ptr<IndexBuffer> m_IndexBuffer;
};
}
