#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <memory>
#include <map>
#include <string>
#include "graphics/renderer.h"
#include "resource/resource.h"

namespace mv {
class Context {
    SDL_Window *window;
    
    int window_height;
    int window_width;
    std::string window_title;
    SDL_GLContext gl_context;
    
    std::map<std::string, IResource> data;
    unsigned int fps;
    unsigned int target_fps = 60;
    unsigned long min_ticks = 1000/target_fps;
    bool should_close = false;

    void engine_input();
    void engine_update(double dt);
    void engine_draw();
    void engine_exit();

  protected:
    Renderer renderer;

  public:
    Context();
    ~Context();
    Context(int width, int height, std::string title);
    std::shared_ptr<Renderer> get_renderer();
    void set_target_fps(unsigned int);

    virtual void init() {};
    virtual void input(SDL_Event&) {};
    virtual void update(double) {};
    virtual void draw() {};
    virtual void exit() {};
    
    void engine_init();
    void run();
};
} // namespace mv
