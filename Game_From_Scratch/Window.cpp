#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>


int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		
		SDL_Log("SDL Init Error: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow(
		"Game From Scratch",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
	);

	if (!window) {

		SDL_Log("SDL Create Window Error: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	Uint32 count = 0;
	if (!SDL_Vulkan_GetInstanceExtensions(window, &count, nullptr)) {
		
		SDL_Log("SDL Vulkan Instance Extention Error: %s", SDL_GetError());
		return 1;
	}

	const char** extensions = new const char* [count];

	if (!SDL_Vulkan_GetInstanceExtensions(window, &count, extensions)) {
		
		SDL_Log("SDL Vulkan Instance Extention Error: %s", SDL_GetError());
		delete[] extensions;
		return 1;
	}

	VkInstance instance;
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.enabledExtensionCount = count;
	createInfo.ppEnabledExtensionNames = extensions;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {

		SDL_Log("Vulkan Instance Creation Error: %s", SDL_GetError());
		delete[] extensions;
		return 1;
	}


	delete[] extensions;

	bool running = true;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}

			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					running = false;
				}
			}
		}

		SDL_Delay(16);
	}

	vkDestroyInstance(instance, nullptr);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}