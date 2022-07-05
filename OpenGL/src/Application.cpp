#include "Application.h"

#include "imgui/imgui.h"

#include "engine/Files.h"
#include "engine/Input.h"
#include "engine/Keys.h"
#include "engine/rendering/Renderer.h"
#include "engine/rendering/ShapeRenderer.h"
#include "engine/Window.h"

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

	//const int numCubes = std::pow(5, 3);
	//const float sideLength = std::cbrt(numCubes);

	//for (float i = -sideLength; i < sideLength; i += 2) {
	//	for (float j = -sideLength; j < sideLength; j += 2) {
	//		for (float k = -sideLength; k < sideLength; k += 2) {
	//			cubes.push_back(std::pair<Cube, Vec4>({ i, j, k, 1.0f }, { static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f }));
	//		}
	//	}
	//}

	Window::beginCursorCapture();
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, -1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
Vec3 cameraOrbit, cameraTarget;

bool capture = false;

constexpr float radius = 30.0f;

Vec2 a = { -3, -5 };
float increment = 3.0f;

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	// lock orbit point
	//if (Input::isKeyDown(Keys::F)) cameraOrbit = cameraTarget;

	// begin orbiting
	//if (Input::isKeyDown(Keys::SPACE)) {
	//	//cameraTarget = cameraOrbit;

	//	const float yawRad = Maths::radians(Input::getMouseYaw());
	//	const float pitchRad = Maths::radians(Input::getMousePitch());

	//	cameraPos = {
	//		-sin(yawRad) * radius * cos(pitchRad),
	//		-sin(pitchRad) * radius,
	//		-cos(yawRad) * radius * cos(pitchRad)
	//	};
	//	
	//	view = Mat4::lookAt(cameraPos, cameraOrbit, cameraUp);

	//}
	//if (Input::isKeyJustReleased(Keys::SPACE)) {
	//	if (capture) {
	//		Window::endCursorCapture();
	//		capture = false;
	//	} else {
	//		Window::beginCursorCapture();
	//		capture = true;
	//	}
	//}

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

	ShapeRenderer::draw(Cube(a.x, a.y, 0, 0.3f), { 1.0f, 0.0f, 0.0f, 1.0f });

	const Vec2 b = { 1, 2 };
	ShapeRenderer::draw(Cube(b.x, b.y, 0, 0.3f), { 0.0f, 1.0f, 0.0f, 1.0f });

	const float m = (a.y - b.y) / (a.x - b.x);
	const float nm = -1 / m;

	// point a
	const float ac = (a.y - a.x * nm);

	constexpr float r = 0.3f;

	const float x1 = (-nm*ac+nm*a.y+a.x+std::sqrt(std::pow(nm, 2) * std::pow(r, 2) - std::pow(nm, 2) * std::pow(a.x, 2) + 2*nm*a.x*a.y - 2*nm*ac*a.x + 2*ac*a.y + std::pow(r, 2) - std::pow(ac, 2) - std::pow(a.y, 2))) / (std::pow(nm, 2) + 1);

	const float x2 = (-nm * ac + nm * a.y + a.x - std::sqrt(std::pow(nm, 2) * std::pow(r, 2) - std::pow(nm, 2) * std::pow(a.x, 2) + 2 * nm * a.x * a.y - 2 * nm * ac * a.x + 2 * ac * a.y + std::pow(r, 2) - std::pow(ac, 2) - std::pow(a.y, 2))) / (std::pow(nm, 2) + 1);
	
	const float y1 = x1 * nm + ac;
	const float y2 = x2 * nm + ac;

	// point b
	const float bc = (b.y - b.x * nm);

	const float x3 = (-nm * bc + nm * b.y + b.x + std::sqrt(std::pow(nm, 2) * std::pow(r, 2) - std::pow(nm, 2) * std::pow(b.x, 2) + 2 * nm * b.x * b.y - 2 * nm * bc * b.x + 2 * bc * b.y + std::pow(r, 2) - std::pow(bc, 2) - std::pow(b.y, 2))) / (std::pow(nm, 2) + 1);

	const float x4 = (-nm * bc + nm * b.y + b.x - std::sqrt(std::pow(nm, 2) * std::pow(r, 2) - std::pow(nm, 2) * std::pow(b.x, 2) + 2 * nm * b.x * b.y - 2 * nm * bc * b.x + 2 * bc * b.y + std::pow(r, 2) - std::pow(bc, 2) - std::pow(b.y, 2))) / (std::pow(nm, 2) + 1);

	const float y3 = x3 * nm + bc;
	const float y4 = x4 * nm + bc;

	ShapeRenderer::draw(Cube(x1, y1, 0, 0.15f), { 0.0f, 1.0f, 1.0f, 1.0f });
	ShapeRenderer::draw(Cube(x2, y2, 0, 0.15f), { 0.0f, 1.0f, 1.0f, 1.0f });
	ShapeRenderer::draw(Cube(x3, y3, 0, 0.15f), { 0.0f, 1.0f, 1.0f, 1.0f });
	ShapeRenderer::draw(Cube(x4, y4, 0, 0.15f), { 0.0f, 1.0f, 1.0f, 1.0f });

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