#include "ApplicationHandler.h"
#include "Application.h"

int main(void)

{
	ApplicationConfig config;
	config.window.width = 3 * 1920 / 4;
	config.window.height = 3 * 1080 / 4;
	config.window.title = "Hi";

	// all variables inside app must have a default constructor or have default values
	Application app;
	ApplicationHandler appHandler(app, config);
	
	//Window::init();

	//WindowConfig config;
	//config.width = 3 * Window::getDisplayWidth() / 4;
	//config.height = 3 * Window::getDisplayHeight() / 4;

	//Window window(config);
	//
	//Rectangle rect(window.getWidth()/2, window.getHeight()/2, 100, 100);

	//Renderer::init();
	//ShapeRenderer::init();

	//Texture tex1(Files::internal("textures/image.png"));

	//ImGui::CreateContext();
	//ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
	//ImGui_ImplOpenGL3_Init();
	//ImGui::StyleColorsDark();

	//Mat4 proj = Mat4::ortho(0, window.getWidth(), 0, window.getHeight()); // TODO add perspective matrix
	//
	//float xTranslate = 0.0f, yTranslate = 0.0f;

	//while (!window.shouldClose()) {
	//	float delta = window.getDelta();
	//	
	//	ImGui_ImplOpenGL3_NewFrame();
	//	ImGui_ImplGlfw_NewFrame();
	//	ImGui::NewFrame();
	//	
	//	Renderer::clear(0.42f, 0.42f, 0.42f);

	//	// translation, rotation, scale function = scale, rotate, translate matrix
	//	Mat4 model = Mat4::identity.translate(xTranslate, yTranslate, 0.0f);
	//	Mat4 view = Mat4::identity; // TODO
	//	Mat4 mvp = proj * view * model;
	//	
	//	// push mvp uniform to shader // TODO TEMPORARY
	//	ShapeRenderer::s_Shader->bind();
	//	ShapeRenderer::s_Shader->setUniformMat4("u_ModelViewProj", mvp);
	//	
	//	ShapeRenderer::begin();
	//	ShapeRenderer::draw(rect, tex1);
	//	ShapeRenderer::draw(Rectangle(400.0f, 400.0f, 100.0f, 100.0f), {1.0f, 0.7f, 1.0f, 1.0f});
	//	ShapeRenderer::end();

	//	{
	//		ImGui::SetNextWindowPos(ImVec2(10, 10));
	//		ImGui::Begin("Debug", (bool*)1, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	//		
	//		ImGui::SliderFloat("X", &xTranslate, window.getWidth() / -2, window.getWidth() / 2);
	//		ImGui::SliderFloat("Y", &yTranslate, window.getHeight() / -2, window.getHeight() / 2);
	//		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	//		
	//		ImGui::End();
	//	}

	//	ImGui::Render();
	//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//	window.update();
	//}

	//ImGui_ImplGlfw_Shutdown();
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui::DestroyContext();

	return 0;
}