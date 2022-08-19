#include "Application.h"

#include "imgui/imgui.h"

#include "engine/Files.h"
#include "engine/Input.h"
#include "engine/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/shapes/Line.h"

void Application::init()
{
	ShapeRenderer::init();

	tex1 = std::make_unique<Texture>(Files::internal("textures/image.png"));
	tex2 = std::make_unique<Texture>(Files::internal("textures/hashinshin.png"));

	Window::beginCursorCapture();
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, -1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
Vec3 cameraOrbit, cameraTarget;

float increment = 0.01f;
const Vec3 p1 = { 0.1f, 0.4f, 0.6f };
Vec3 p2 = { -0.5f, -0.5f, -0.5f };

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);
	
	cameraFront = Input::getCameraDirection();

	// camera position movement
	const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? 2.0f : 1.0f);

	//if (Input::isKeyDown(Keys::W)) {
	if (Input::isKeyDown(Keys::W)) {
		cameraPos += cameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::S)) {
		cameraPos -= cameraFront * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::D)) {
		cameraPos -= cameraFront.cross(cameraUp).normalise() * moveSpeed * Window::deltatime();
	}
	if (Input::isKeyDown(Keys::A)) {
		cameraPos += cameraFront.cross(cameraUp).normalise() * moveSpeed * Window::deltatime();
	}

	const Mat4 view = Mat4::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	Renderer::setViewMatrix(view);
	
	ShapeRenderer::begin();

	if (p2.y > 2.5f)
		increment = -0.01f;
	if (p2.y < -2.5f)
		increment = 0.01f;

	p2.y += increment;
	p2.z += increment;

	ShapeRenderer::draw(Cube(p1, 0.05f), { 1.0f, 0.0f, 0.0f, 1.0f });
	ShapeRenderer::draw(Cube(p2, 0.05f), { 1.0f, 0.0f, 0.0f, 1.0f });

	ShapeRenderer::draw(Line(p1, p2, 0.01f), {1.0f, 1.0f, 0.0f, 1.0f});

	ShapeRenderer::draw(Line({ -100, 0, 0 }, { 100, 0, 0 }, 0.01f), { 1, 0, 0, 1 }); // X
	ShapeRenderer::draw(Line({ 0, -100, 0 }, { 0, 100, 0 }, 0.01f), { 0, 1, 0, 1 }); // Y
	ShapeRenderer::draw(Line({ 0, 0, -100 }, { 0, 0, 100 }, 0.01f), { 0, 0, 1, 1 }); // Z

	ShapeRenderer::end();
}

void Application::imGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));


	ImGui::End();
}

void Application::destroy()
{

}