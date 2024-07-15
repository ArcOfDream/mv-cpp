#include "mv/scripting/register_types.h"
#include "sol/sol.hpp"

#include "mv/objects/node.h"
#include "mv/objects/sprite.h"
#include "mv/resource/texture.h"

#include <glm/ext/matrix_float3x3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <memory>

namespace mv {

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

void register_glm_types(sol::state &lua) {
    // glm::vec2
    lua.new_usertype<glm::vec2>("Vec2",
    sol::constructors<
        glm::vec2(), 
        glm::vec2(float),
        glm::vec2(float, float)>(),
    sol::call_constructor, [](float x, float y) { return glm::vec2{x, y}; },
    "x", &glm::vec2::x,
    "y", &glm::vec2::y);
    
    // glm::vec3
    lua.new_usertype<glm::vec3>("Vec3",
    sol::constructors<
        glm::vec3(), 
        glm::vec3(float), 
        glm::vec3(float, float, float),
        glm::vec3(glm::vec2, float), 
        glm::vec3(float, glm::vec2)>(),
    sol::call_constructor, [](float x, float y, float z) { return glm::vec3{x, y, z}; },
    "x", &glm::vec3::x,
    "y", &glm::vec3::y,
    "z", &glm::vec3::z);
    
    // glm::vec4
    lua.new_usertype<glm::vec4>("Vec4",
    sol::constructors<
        glm::vec4(), 
        glm::vec4(float), 
        glm::vec4(float, float, float, float),
        glm::vec4(glm::vec2, float, float),
        glm::vec4(float, glm::vec2, float),
        glm::vec4(float, float, glm::vec2),
        glm::vec4(glm::vec2, glm::vec2),
        glm::vec4(glm::vec3, float),
        glm::vec4(float, glm::vec3)>(),
    sol::call_constructor, [](float x, float y, float z, float w) { return glm::vec4{x, y, z, w}; },
    "x", &glm::vec4::x,
    "y", &glm::vec4::y,
    "z", &glm::vec4::z,
    "w", &glm::vec4::w);
    
    // glm::mat3
    lua.new_usertype<glm::mat3>("Mat3",
    sol::constructors<
        glm::mat3(),
        glm::mat3(float)
    >());
    
    // TODO: expose more of GLM as needed
}

void register_resource_types(sol::state &lua) {
    // mv:Texture
    lua.new_usertype<Texture>("Texture",
    sol::constructors<Texture(const char*)>());
    
    // TODO: expose mv::Material when ready 
    // and provide a builder to build shaders from source

    lua.set_function("load_texture_from_source", load_texture_from_source);
    lua.set_function("load_texture_from_file", load_texture_from_file);
    lua.set_function("load_texture_raw", load_texture_raw);
}

void register_node_types(sol::state &lua) {
    // mv::Node
    lua.new_usertype<Node>("Node",
    sol::constructors<Node(sol::this_state, const char*)>(),
    sol::meta_function::index, &Node::lua_dynamic_get,
    sol::meta_function::new_index, &Node::lua_dynamic_set,
    sol::meta_function::length, [](Node& n) { return n.entries.size(); },
    "init", &Node::lua_init,
    // "update", &Node::lua_update,
    "update", sol::property(
        [](Node &node) { return node.lua_update; },
        [](Node &node, sol::function func) { node.lua_update = func; }
    ),
    "draw", &Node::lua_draw,
    "input", &Node::lua_input,
    
    // NOTE: Likely need a better way to expose this
    "children", &Node::children,
    
    // I probably don't actually need to do it like this, and just have 
    // the Node template exposed.
    "add_instanced_child", sol::overload(
        &Node::add_instanced_child<Sprite>,
        &Node::add_instanced_child<Node>
    )
    );

    // mv::Sprite
    lua.new_usertype<Sprite>("Sprite",
    sol::constructors<
        Sprite(sol::this_state, const char*, std::shared_ptr<Texture>),
        Sprite(sol::this_state, const char*)
    >(),
    sol::base_classes, sol::bases<Node>(),
    sol::meta_function::index, &Sprite::lua_dynamic_get,
    sol::meta_function::new_index, &Sprite::lua_dynamic_set,
    sol::meta_function::length, [](Sprite& n) { return n.entries.size(); },

    "pos", sol::property(&Sprite::get_pos, &Sprite::set_pos),
    "offset", sol::property(&Sprite::get_offset, &Sprite::set_offset),
    "scale", sol::property(&Sprite::get_scale, &Sprite::set_scale),
    "angle", sol::property(&Sprite::get_angle, &Sprite::set_angle),
    "angle_degrees", sol::property(&Sprite::get_angle_degrees, &Sprite::set_angle_degrees),
    "texture", sol::property(&Sprite::get_texture, &Sprite::set_texture),
    "color", sol::property(&Sprite::get_color, &Sprite::set_color),
    "centered", sol::property(&Sprite::is_centered, &Sprite::set_centered),
    
    "set_pos", &Sprite::set_pos,
    "get_pos", &Sprite::get_pos,
    "set_offset", &Sprite::set_offset,
    "get_offset", &Sprite::get_offset,
    "set_scale", &Sprite::set_scale,
    "get_scale", &Sprite::get_scale,
    "set_angle", &Sprite::set_angle,
    "get_angle", &Sprite::get_angle,
    "set_angle_degrees", &Sprite::set_angle_degrees,
    "get_angle_degrees", &Sprite::get_angle_degrees,
    "set_texture", &Sprite::set_texture,
    "get_texture", &Sprite::get_texture,
    "set_color", &Sprite::set_color,
    "get_color", &Sprite::get_color,
    "is_centered", &Sprite::is_centered,
    "set_centered", &Sprite::set_centered
    );
}

}