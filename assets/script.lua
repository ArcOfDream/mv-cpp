a_timer = 0

an_update = function(self, dt)
    a_timer = (a_timer + dt * 10)
    print(string.format("timer: %f", a_timer))

    self.pos = Vec2( -- doesn't apply this change
        math.sin(a_timer)*100,
        math.cos(a_timer)*100
    )
    print(string.format("pos: %f, %f", self.pos.x, self.pos.y))
end

my_node = Sprite.new("my_node", kleines_tex)

my_node.pos = Vec2(20, 20) -- applies this change
my_node.update = an_update


kleines_root:add_instanced_child(my_node)