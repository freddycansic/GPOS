#include "Application.h"

#include "imgui/imgui.h"

#include "engine/Files.h"
#include "engine/Input.h"
#include "engine/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"
#include "engine/rendering/shapes/Line.h"

#include "maths/Maths.h"

void Application::init()
{
	ShapeRenderer::init();

	tex1 = std::make_unique<Texture>(Files::internal("textures/image.png"));
	tex2 = std::make_unique<Texture>(Files::internal("textures/hashinshin.png"));

	cube1 = Cube(0, 0, 0, 10.0f);
	cube2 = Cube(-5, 5, 0, 10.0f);
	cube3 = Cube(-8, -5, 0, 2);

	rect1 = Rectangle(5, 5, 5, 5);
	rect2 = Rectangle(8, 9, 3, 10);

	Window::beginCursorCapture();
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, -1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
Vec3 cameraOrbit, cameraTarget;
Vec3 a = { 1, 2, 3 };
float increment = 3.0f;

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

	if (a.y > 3.0f)
		increment = -3.0f * Window::deltatime();
	if (a.y < -3.0f)
		increment = 3.0f * Window::deltatime();

	a.y += increment;
	a.z += increment;

	const Vec3 b = { -1, -2, -3 };

	ShapeRenderer::draw(Line(a, b, 0.1f), {1.0f, 1.0f, 1.0f, 1.0f});
	ShapeRenderer::draw(Cube(a.x, a.y, a.z, 0.3f), { 1.0f, 0.0f, 0.0f, 1.0f });
	ShapeRenderer::draw(Cube(b.x, b.y, b.z, 0.3f), { 1.0f, 0.0f, 0.0f, 1.0f });

	ShapeRenderer::draw(Cube(0, 2, 0, 0.3f), { 0.0f, 0.0f, 0.0f, 1.0f });

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