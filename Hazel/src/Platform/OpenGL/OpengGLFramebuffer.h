#pragma once
#include "Hazel/Renderer/Framebuffer.h"
namespace Hazel
{
	class OpengGLFramebuffer :
		public Framebuffer
	{
	public:
		OpengGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpengGLFramebuffer();

		void Invalidate();

		virtual void  Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererId() const override
		{								 
			return m_ColorAttachment;
		}
		virtual const FramebufferSpecification& GetSpecification() const override
		{
			return m_Specification;
		}

	private:
		uint32_t m_RendererId=0;
		uint32_t m_ColorAttachment=0, m_DepthAttachment=0;
		FramebufferSpecification m_Specification;
	};
}
