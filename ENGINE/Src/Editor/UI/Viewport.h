#ifndef VIEWPORT_H
#define VIEWPORT_H

class Window;

class Viewport {
public:
	// Not sure what to do about this will have to find a better solution.
	// The camera will need to access these values for updating the aspect ratio
	static float VIEWPORT_WIDTH;
	static float VIEWPORT_HEIGHT;

	Viewport();
	~Viewport();

	void Draw(unsigned int textureColorBuffer, Window& window);
private:
};

#endif 

/*
	class Viewport : public Panel {
	public:
		int& windowWidth;
		int& windowHeight;
		Framebuffer*& framebuffer;
		bool& isViewportFocused;

		Viewport(int& windowWidth, int& windowHeight, Framebuffer*& framebuffer, bool& isViewportFocused) :
			windowWidth(windowWidth), windowHeight(windowHeight),
			framebuffer(framebuffer),
			isViewportFocused(isViewportFocused)
		{}

		void Render(Mesh*& selectedMesh, std::vector<Mesh*>& meshes)  override {
			ImGui::Begin("3D Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
			// Assuming framebuffer->getTexture() returns the OpenGL texture ID
			GLuint my_opengl_texture = framebuffer->textureColorbuffer;
			ImVec2 size = ImVec2((float)windowWidth, (float)windowHeight); // Size of the texture
			ImGui::Image((void*)(intptr_t)my_opengl_texture, size);
			isViewportFocused = ImGui::IsWindowFocused();
			ImGui::End();
		}
	};
*/