#include "hzpch.h"
#include "Hazel/Renderer/Renderer2D.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
namespace Hazel {

  struct Renderer2DStorage {
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;
  };

  static Renderer2DStorage* s_Data;

  void Hazel::Renderer2D::Init()
  {
    s_Data = new Renderer2DStorage();
    s_Data->QuadVertexArray = VertexArray::Create();

    float squareVertices[5 * 4] = {
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    squareVB->SetLayout(
      {
         { ShaderDataType::Float3,"a_Position" },
         { ShaderDataType::Float2,"a_TexCoord" },
      });
    s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0,1,2,2,3,0 };
    Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

    s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

    s_Data->WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetInt("u_Texture", 0);
  }

  void Hazel::Renderer2D::ShutDown()
  {
    delete s_Data;
  }

  void Hazel::Renderer2D::BeginScene(const OrthographicCamera& camera)
  {
    s_Data->TextureShader->Bind();
    s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectMatrix());
  }

  void Hazel::Renderer2D::EndScene()
  {

  }

  void Hazel::Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
  {
    DrawQuad({ position.x,position.y,.0f }, size, color);
  }

  void Hazel::Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
    s_Data->WhiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
  }


  void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
  {
    DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
  {
    HZ_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", tintColor);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
  {
    HZ_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", color);
    s_Data->TextureShader->SetFloat("u_TilingFactor", 1.0f);
    s_Data->WhiteTexture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
      * glm::rotate(glm::mat4(1.0f), rotation, { .0f,.0f,1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
  {
    DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
  {
    HZ_PROFILE_FUNCTION();

    s_Data->TextureShader->SetFloat4("u_Color", tintColor);
    s_Data->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
    texture->Bind();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
      * glm::rotate(glm::mat4(1.0f), rotation, { .0f,.0f,1.0f })
      * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
    s_Data->TextureShader->SetMat4("u_Transform", transform);

    s_Data->QuadVertexArray->Bind();
    RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
  }
}