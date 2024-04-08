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

		virtual uint32_t GetColorAttachmentRendererId(uint32_t index=0) const override
		{	
			HZ_CORE_ASSERT(index < m_ColorAttachments.size(),"");
			return m_ColorAttachments[index];
		}
		virtual const FramebufferSpecification& GetSpecification() const override
		{
			return m_Specification;
		}

	private:
		uint32_t m_RendererId=0;
		uint32_t  m_DepthAttachment=0;
		FramebufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		std::vector<uint32_t> m_ColorAttachments;
	};
}
