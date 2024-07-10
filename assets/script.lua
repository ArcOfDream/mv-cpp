local a_timer = 0.0
MySprite = {}
MySprite.__index = MySprite

-- setmetatable(MySprite, Sprite)

function MySprite.instance(...)
    local self = Sprite.new(...)
    self.update = MySprite.update
    setmetatable(self.__metatable, MySprite)
    return self
end

function MySprite:update(dt)
    print("update from lua")
    
    a_timer = a_timer + dt
    print(a_timer)

    -- for some reason this doesn't want to work
    self.pos = vec2(
        sin(a_timer)*100,
        cos(a_timer)*100
    )
end

my_node = MySprite.instance("my_node", kleines_tex)
my_node.a_value = 1337
my_node.some_vec = vec2(1, 2)
print(my_node.a_value)

kleines_root:add_instanced_child(my_node)