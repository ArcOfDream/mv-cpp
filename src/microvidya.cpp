#define PROJECT_NAME "microvidya"
#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES
#define SOL_ALL_SAFETIES_ON 1

// #include "imgui.h"
// #include "fontstash.h"
#include "mv/mv.h"
#include "mv/graphics/fontrender.h"
#include "imgui.h"
#include "soloud.h"
#include "soloud_error.h"
#include "soloud_freeverbfilter.h"
#include "soloud_speech.h"
#include <glm/glm.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <math.h>
#include <memory>
#include <stdio.h>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

float time_elapsed;

using namespace mv;

void load_middleclass(sol::state &lua) {
    lua.require_script("middleclass", R"(
local middleclass = {
  _VERSION     = 'middleclass v4.1.1',
  _DESCRIPTION = 'Object Orientation for Lua',
  _URL         = 'https://github.com/kikito/middleclass',
  _LICENSE     = [[
    MIT LICENSE

    Copyright (c) 2011 Enrique García Cota

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ]]
}

local function _createIndexWrapper(aClass, f)
  if f == nil then
    return aClass.__instanceDict
  elseif type(f) == "function" then
    return function(self, name)
      local value = aClass.__instanceDict[name]

      if value ~= nil then
        return value
      else
        return (f(self, name))
      end
    end
  else -- if  type(f) == "table" then
    return function(self, name)
      local value = aClass.__instanceDict[name]

      if value ~= nil then
        return value
      else
        return f[name]
      end
    end
  end
end

local function _propagateInstanceMethod(aClass, name, f)
  f = name == "__index" and _createIndexWrapper(aClass, f) or f
  aClass.__instanceDict[name] = f

  for subclass in pairs(aClass.subclasses) do
    if rawget(subclass.__declaredMethods, name) == nil then
      _propagateInstanceMethod(subclass, name, f)
    end
  end
end

local function _declareInstanceMethod(aClass, name, f)
  aClass.__declaredMethods[name] = f

  if f == nil and aClass.super then
    f = aClass.super.__instanceDict[name]
  end

  _propagateInstanceMethod(aClass, name, f)
end

local function _tostring(self) return "class " .. self.name end
local function _call(self, ...) return self:new(...) end

local function _createClass(name, super)
  local dict = {}
  dict.__index = dict

  local aClass = { name = name, super = super, static = {},
                   __instanceDict = dict, __declaredMethods = {},
                   subclasses = setmetatable({}, {__mode='k'})  }

  if super then
    setmetatable(aClass.static, {
      __index = function(_,k)
        local result = rawget(dict,k)
        if result == nil then
          return super.static[k]
        end
        return result
      end
    })
  else
    setmetatable(aClass.static, { __index = function(_,k) return rawget(dict,k) end })
  end

  setmetatable(aClass, { __index = aClass.static, __tostring = _tostring,
                         __call = _call, __newindex = _declareInstanceMethod })

  return aClass
end

local function _includeMixin(aClass, mixin)
  assert(type(mixin) == 'table', "mixin must be a table")

  for name,method in pairs(mixin) do
    if name ~= "included" and name ~= "static" then aClass[name] = method end
  end

  for name,method in pairs(mixin.static or {}) do
    aClass.static[name] = method
  end

  if type(mixin.included)=="function" then mixin:included(aClass) end
  return aClass
end

local DefaultMixin = {
  __tostring   = function(self) return "instance of " .. tostring(self.class) end,

  initialize   = function(self, ...) end,

  isInstanceOf = function(self, aClass)
    return type(aClass) == 'table'
       and type(self) == 'table'
       and (self.class == aClass
            or type(self.class) == 'table'
            and type(self.class.isSubclassOf) == 'function'
            and self.class:isSubclassOf(aClass))
  end,

  static = {
    allocate = function(self)
      assert(type(self) == 'table', "Make sure that you are using 'Class:allocate' instead of 'Class.allocate'")
      return setmetatable({ class = self }, self.__instanceDict)
    end,

    new = function(self, ...)
      assert(type(self) == 'table', "Make sure that you are using 'Class:new' instead of 'Class.new'")
      local instance = self:allocate()
      instance:initialize(...)
      return instance
    end,

    subclass = function(self, name)
      assert(type(self) == 'table', "Make sure that you are using 'Class:subclass' instead of 'Class.subclass'")
      assert(type(name) == "string", "You must provide a name(string) for your class")

      local subclass = _createClass(name, self)

      for methodName, f in pairs(self.__instanceDict) do
        if not (methodName == "__index" and type(f) == "table") then
          _propagateInstanceMethod(subclass, methodName, f)
        end
      end
      subclass.initialize = function(instance, ...) return self.initialize(instance, ...) end

      self.subclasses[subclass] = true
      self:subclassed(subclass)

      return subclass
    end,

    subclassed = function(self, other) end,

    isSubclassOf = function(self, other)
      return type(other)      == 'table' and
             type(self.super) == 'table' and
             ( self.super == other or self.super:isSubclassOf(other) )
    end,

    include = function(self, ...)
      assert(type(self) == 'table', "Make sure you that you are using 'Class:include' instead of 'Class.include'")
      for _,mixin in ipairs({...}) do _includeMixin(self, mixin) end
      return self
    end
  }
}

function middleclass.class(name, super)
  assert(type(name) == 'string', "A name (string) is needed for the new class")
  return super and super:subclass(name) or _includeMixin(_createClass(name), DefaultMixin)
end

setmetatable(middleclass, { __call = function(_, ...) return middleclass.class(...) end })

return middleclass

    )");
}

class MyGame : public Context {
    // soloud engine
    SoLoud::Speech speech;
    SoLoud::PXTone pxt;
    SoLoud::FreeverbFilter verb;

    std::shared_ptr<mv::Texture> kleines_ptr;
    std::unique_ptr<Sprite> kleines_root;
    Sprite *kleines_child = nullptr;

    FontRender f;

    Camera2D cam;
    float time = 0.0f;
    float separation = 0.0f;

    bool speak_already_pressed = false;
    // unsigned int speak_handle = 0;
    unsigned int pxt_handle = 0;
    char speak_buffer[128] = "assets/pantest.ptcop";

  public:
    using Context::Context;

    void on_talk_press() {
        soloud.stop(pxt_handle);
        // speech.setText(speak_buffer);
        unsigned int err = pxt.load(speak_buffer);
        if (err == SoLoud::SO_NO_ERROR)
            pxt_handle = soloud.play(pxt, 0.9f, 0.0f);
    }

    void init() override {
        set_target_fps(60);
        kleines_ptr =
            load_texture_from_source("kleines", kleines_png, kleines_png_size);

        renderer.set_camera(&cam);

        soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF |
                        SoLoud::Soloud::ENABLE_VISUALIZATION,
                    SoLoud::Soloud::AUTO, 44100, 1024, 2);

        kleines_root =
            std::make_unique<Sprite>("kleines root", kleines_ptr->getptr());

        
        for (int i = 0; i < 512; i++) {
            // auto c = kleines_root->add_child<Sprite>("kleines child",
            //                                          kleines_ptr->getptr());
            auto c = std::make_unique<Sprite>("kleines child", kleines_ptr->getptr());

            c->set_pos({(rand() % 1500) - 750, (rand() % 1500) - 750});
            c->set_pos({(rand() % 1500) - 750, (rand() % 1500) - 750});
            c->set_scale({0.5f, 0.5f});
            c->set_color({1,1,1,0.2});

            kleines_root->add_instanced_child(c.release());
        }
        kleines_child = kleines_root->add_child<Sprite>("kleines child",
                                                        kleines_ptr->getptr());

        kleines_child->set_pos({200, 0});
        kleines_child->set_scale({0.2f, 0.2f});
        kleines_child->set_color({1, 1, 1, 0.2});

        load_middleclass(lua);
        lua.set("kleines_root", kleines_root.get());
        lua.set("kleines_tex", kleines_ptr->getptr());

        sol::optional<sol::error> err = lua.safe_script_file("assets/script.lua");

        if (err.has_value()) {
          printf("Error during script compilation!");
        }
        
        f = {256, 256};
        f.setup_context();
        f.add_font_mem("pixo", Pixolletta8px_ttf, Pixolletta8px_ttf_size);
        f.add_font_mem("silkscreen", slkscr_ttf, slkscr_ttf_size);
    }

    void update(double dt) override {
        time += dt;
        Sprite &k = *kleines_root;

        k.set_angle_degrees(time * 0.01);
        // k.set_scale({sinf(time*0.2), 1});
        k.set_pos({cosf(time * 1.3f) * 200, sinf(time * 0.77f) * 200});
        kleines_child->set_angle_degrees(time * 0.01);
        // cam.position.x = sinf(time*0.25f) * 100;
        // cam.position.y = cosf(time*0.25f) * 100;
        // cam.position = {0, sinf(time*0.5f)*100};
        // cam.rotation = M_PI/4;

        kleines_root->_update(dt);
    }

    void draw() override {
        char str[20];
        snprintf(str, sizeof(str), "fps: %i", get_fps());

        renderer.push_quad({0, 0}, {64, 64}, {0.5, 0.5, 1, 1},
                           kleines_ptr->get_id());
        kleines_root->_draw();

        f.color = {1, 1, 0, 1};
        f.font("pixo");
        fonsSetSize(f.ctx, 16);
        // fonsSetBlur(f.ctx, 2);
        fonsSetAlign(f.ctx, FONS_ALIGN_CENTER);
        fonsDrawText(f.ctx, 0, 0, "methinks this should be ok", nullptr);

        fonsDrawText(f.ctx, 0, 16, str, nullptr);

        fonsDrawDebug(f.ctx, -256, -256);
        
        /* GUI */
        ImGui::Begin("Kleines");

        ImGui::SeparatorText("Camera");
        ImGui::Text("Position: [%f, %f]", cam.position.x, cam.position.y);
        ImGui::Text("Rotation: %f", cam.rotation);

        ImGui::SeparatorText("Kleines");
        ImGui::SliderFloat("Separation", &separation, -200.0f, 200.0f);

        ImGui::SeparatorText("SoLoud");
        ImGui::InputText("Path", speak_buffer, 128);

        if (ImGui::Button("Play!") && !speak_already_pressed) {
            on_talk_press();
            speak_already_pressed = true;
        } else {
            speak_already_pressed = false;
        }

        ImGui::End();
    }
};

int main(int, char **) {
    printf("The project name is %s\n", PROJECT_NAME);

    auto game = std::make_shared<MyGame>(320, 240, "microvidya");


    game->engine_init();
    game->run();

    return 0;
}
