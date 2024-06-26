#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <condition_variable>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include "soloud.h"
#include "graphics/renderer.h"
#include "resource/resource.h"

#pragma once

namespace mv {
class Context : public std::enable_shared_from_this<Context> {
    
    int window_height;
    int window_width;
    std::string window_title;
    
    std::unordered_map<std::string, IResource> data;
    unsigned int fps;
    unsigned int target_fps = 60;
    unsigned long min_ticks = 1000/target_fps;
    bool should_close = false;


    void engine_input();
    void engine_update(double dt);
    void engine_draw();
    void engine_exit();

  protected:
    Renderer &renderer = Renderer::get();
    SoLoud::Soloud soloud;
    SDL_GLContext gl_context;
    SDL_Window *window;
    
    std::mutex mutex;
    std::condition_variable cv;
    std::condition_variable cv_main;
    std::thread draw_thread;
    bool draw_ready = false;
    bool draw_complete = false;
    bool draw_running = false;

  public:
    Context();
    ~Context();
    Context(int width, int height, std::string title);
    std::shared_ptr<Renderer> get_renderer();
    void set_target_fps(unsigned int);
    void engine_init();
    void run();
    void stop();
    void draw_loop();

    virtual void init() {};
    virtual void input(SDL_Event&) {};
    virtual void update(double) {};
    virtual void draw() {};
    virtual void exit() {};

    std::shared_ptr<Context> getptr() {
      return shared_from_this();
    }

    int get_fps() { return fps; }
};
} // namespace mv
