#include <Hazel.h>


#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"
using namespace Hazel;
class ExampleLayer :public Hazel::Layer
{
public:
  ExampleLayer()
    :Layer("Example"),m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),m_CameraPosition(0.0f)
  {
    m_VertexArray.reset(VertexArray::Create());

    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    Ref<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


    BufferLayout layout = {
      {ShaderDataType::Float3,"a_Position"},
      {ShaderDataType::Float4,"a_Color"}
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);


    unsigned int indices[3] = { 0,1,2 };
    Ref<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_SquareVA.reset(VertexArray::Create());

    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    Ref<VertexBuffer> squareVB;
    squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({
      {ShaderDataType::Float3,"a_Position"},
      {ShaderDataType::Float2,"a_TexCoord"}
      });

    m_SquareVA->AddVertexBuffer(squareVB);
    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Ref<IndexBuffer> squareIB;
    squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);



    std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

      uniform mat4 u_ViewProjection;
      uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position =u_ViewProjection* u_Transform *vec4(a_Position, 1.0);	
			}
		)";

    std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

    m_Shader.reset( Shader::Create(vertexSrc, fragmentSrc));

    std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

      uniform mat4 u_ViewProjection;
      uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position =u_ViewProjection* u_Transform *vec4(a_Position, 1.0);	
			}
		)";

    std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
      uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";
    m_FlatColorShader.reset( Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));


    std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

    std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

    m_TextureShader.reset(Hazel::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

    m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
  }

  void OnUpdate(Timestep ts) override
  {
    if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
    {
      m_CameraPosition.x -= m_CameraMoveSpeed * ts;
    }
    else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
      m_CameraPosition.x += m_CameraMoveSpeed * ts;

    if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
      m_CameraPosition.y += m_CameraMoveSpeed * ts;
    else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
      m_CameraPosition.y -= m_CameraMoveSpeed * ts;

    if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
      m_CameraRotation += m_CameraRotationSpeed * ts;
    if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
      m_CameraRotation -= m_CameraRotationSpeed * ts;

    RenderCommand::SetClearColor({ .1f,.1f,.1f,1 });
    RenderCommand::Clear();

    m_Camera.SetPosition(m_CameraPosition);
    m_Camera.SetRotation(m_CameraRotation);
   HZ_INFO("{0},{1},  {2}",m_CameraPosition.x,m_CameraPosition.y,  m_CameraRotation);
    Renderer::BeginScene(m_Camera);

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
    for (int y = 0; y < 20; y++)
    {
      for (int x = 0; x < 20; x++)
      {
        glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
        Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
      }
    }

   // Renderer::Submit(m_BlueShader, m_SquareVA);
    Renderer::Submit(m_Shader, m_VertexArray);

    m_Texture->Bind();
    Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Renderer::EndScene();
  }

  virtual void OnImGuiRender() override
  {
       ImGui::Begin("Settings");
       ImGui::ColorEdit3("Square Color",glm::value_ptr(m_SquareColor));
       ImGui::End();
  }

  void OnEvent(Hazel::Event& event) override
  {
    if (event.GetEventType() == Hazel::EventType::KeyPressed) {
      Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
      if (e.GetKeyCode() == HZ_KEY_TAB)
        HZ_TRACE("Tab key is pressed (event)!");
      HZ_TRACE("{0}", (char)e.GetKeyCode());
    }
  }
private:
  std::shared_ptr <VertexArray> m_VertexArray;
  std::shared_ptr<Shader> m_Shader;


  std::shared_ptr<Shader> m_FlatColorShader;
  std::shared_ptr<VertexArray> m_SquareVA;

  Ref<Shader>   m_TextureShader;
  Ref<Hazel::Texture2D> m_Texture;


  OrthographicCamera m_Camera;
  glm::vec3 m_CameraPosition;
  float m_CameraMoveSpeed = 5.0f;

  float m_CameraRotation = 0.0f;
  float m_CameraRotationSpeed = 180.0f;

  glm::vec3 m_SquareColor = { .2f,.3f,.8f };
};


class Sandbox :public Hazel::Application
{
public:
  Sandbox()
  {
    PushLayer(new ExampleLayer());
  }

  ~Sandbox()
  {
  }

private:

};

Hazel::Application* Hazel::CreateApplication() {
  return new Sandbox();
}