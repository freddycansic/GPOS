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
Mat4 view;

bool capture = false;

constexpr float radius = 30.0f;

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

	if(true) {
		cameraFront = Input::getCameraDirection();
		cameraTarget = cameraPos + cameraFront * radius;

		// camera position movement
		const float moveSpeed = 10.0f * (Input::isKeyDown(Keys::LEFT_SHIFT) ? 2.0f : 1.0f);

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

		view = Mat4::lookAt(cameraPos, cameraTarget, cameraUp);
	}

	Renderer::setViewMatrix(view);

	cube1.setTranslation(cubeTransform.tra.x, cubeTransform.tra.y, cubeTransform.tra.z);
	cube1.setRotation(cubeTransform.rot.x, Window::currentTime() * 50, Window::currentTime() * 35);
	cube1.setScale(cubeTransform.sca.x, cubeTransform.sca.y, cubeTransform.sca.z);

	ShapeRenderer::begin();

	ShapeRenderer::draw(Cube(cameraOrbit.x, cameraOrbit.y, cameraOrbit.z, 0.5f), { 1.0f, 1.0f, 0.0f, 1.0f });

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