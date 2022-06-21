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

	const int numCubes = std::pow(5, 3);
	const float sideLength = std::cbrt(numCubes);

	for (float i = -sideLength; i < sideLength; i += 2) {
		for (float j = -sideLength; j < sideLength; j += 2) {
			for (float k = -sideLength; k < sideLength; k += 2) {
				cubes.push_back(std::pair<Cube, Vec4>({ i, j, k, 1.0f }, { static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), 1.0f }));
			}
		}
	}

	//for (int i = 0; i < 100; ++i) {
	//	std::cout << Maths::randf(0.0f, 10.0f) << std::endl;
	//	std::cout << Maths::randint(0, 10) << std::endl;
	//}

	Window::beginCursorCapture();
}

Vec3 cameraPos = { 0.0f, 0.0f, 30.0f };
Vec3 cameraFront = { 0.0f, 0.0f, -1.0f }; // point forward
Vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
Vec3 cameraOrbit, cameraTarget;
Mat4 view;

constexpr float radius = 30.0f;

void Application::render()
{
	Renderer::clear(0.42f, 0.42f, 0.42f);

	//Renderer::setViewMatrix(Mat4::identity.rotate(-viewTransform.rot.x, -viewTransform.rot.y, viewTransform.rot.z).translate(viewTransform.tra.x, viewTransform.tra.y, viewTransform.tra.z).scale(viewTransform.sca.x, viewTransform.sca.y, viewTransform.sca.z));

	// lock orbit point
	if (Input::isKeyDown(Keys::F)) cameraOrbit = cameraTarget;

	// begin orbiting
	if (Input::isKeyDown(Keys::SPACE)) {
		//cameraTarget = cameraOrbit;

		const float yawRad = Maths::radians(Input::getMouseYaw());
		const float pitchRad = Maths::radians(Input::getMousePitch());

		cameraPos = {
			-sin(yawRad) * radius * cos(pitchRad),
			-sin(pitchRad) * radius,
			-cos(yawRad) * radius * cos(pitchRad)
		};
		
		view = Mat4::lookAt(cameraPos, cameraOrbit, cameraUp);

	}
	else {
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

	//ShapeRenderer::draw(cube1, *tex1);
	//ShapeRenderer::draw(cube2, *tex2);
	//Cube facing(cameraFront, 5.0f);
	//ShapeRenderer::draw(facing, { 1.0f, 0.0f, 0.0f, 1.0f });
	//ShapeRenderer::draw(cube3, {0, 1, 0, 1});
	//ShapeRenderer::draw(rect1, {1, 1, 0, 1});

	for (auto& cube : cubes) {
		ShapeRenderer::draw(cube.first, cube.second);
	}

	//ShapeRenderer::draw(Cube(0, 0, 0, 1), { 1, 1, 0, 1 });
	//ShapeRenderer::draw(Cube(0, 1, 4, 1), *tex1);

	ShapeRenderer::end();
}

void Application::imGuiRender()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::Begin("Debug", reinterpret_cast<bool*>(1), ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("Cube");

	ImGui::Text("Translation");
	ImGui::SliderFloat("##1", &cubeTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##2", &cubeTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##3", &cubeTransform.tra.z, -100.0f, 100.0f);

	ImGui::Text("Rotation");
	ImGui::SliderFloat("##4", &cubeTransform.rot.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##5", &cubeTransform.rot.z, -100.0f, 100.0f);
	ImGui::SliderFloat("##6", &cubeTransform.rot.x, -100.0f, 100.0f);
	
	ImGui::Text("Scale");
	ImGui::SliderFloat("##7", &cubeTransform.sca.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##8", &cubeTransform.sca.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##9", &cubeTransform.sca.z, -100.0f, 100.0f);

	ImGui::Text("View");
	
	ImGui::Text("Translation");
	ImGui::SliderFloat("##10", &viewTransform.tra.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##11", &viewTransform.tra.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##12", &viewTransform.tra.z, -100.0f, 100.0f);
							
	ImGui::Text("Rotation");
	ImGui::SliderFloat("##13", &viewTransform.rot.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##14", &viewTransform.rot.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##15", &viewTransform.rot.z, -100.0f, 100.0f);
							
	ImGui::Text("Scale");	
	ImGui::SliderFloat("##16", &viewTransform.sca.x, -100.0f, 100.0f);
	ImGui::SliderFloat("##17", &viewTransform.sca.y, -100.0f, 100.0f);
	ImGui::SliderFloat("##18", &viewTransform.sca.z, -100.0f, 100.0f);

	ImGui::Text("%.1f FPS", static_cast<double>(ImGui::GetIO().Framerate));
	
	ImGui::End();
}

void Application::destroy()
{

}