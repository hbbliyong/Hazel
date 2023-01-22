#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
  
  Hazel::Log::Init();
  HZ_CODE_TRACE("Initlized log!");
  int a = 5;
  HZ_INFO("hello var={0}",a);
  auto app = Hazel::CreateApplication();
  app->Run();
  delete app;
}
#endif