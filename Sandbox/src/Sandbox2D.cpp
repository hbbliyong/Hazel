#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
  :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
  m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
  HZ_PROFILE_FUNCTION();
  {
    HZ_PROFILE_SCOPE("CameraController::OnUpdate");
    m_CameraController.OnUpdate(ts);
  }
  Hazel::RenderCommand::SetClearColor({ .1f,.1f,.1f,1 });
  Hazel::RenderCommand::Clear();

  {
    HZ_PROFILE_SCOPE("CRenderer Draw");
    Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
    Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
    Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
  

    Hazel::Renderer2D::EndScene();
  }
}

void Sandbox2D::OnImGuiRender()
{
  HZ_PROFILE_FUNCTION();
  ImGui::Begin("Settings");
  ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

  ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
  m_CameraController.OnEvent(e);
}
