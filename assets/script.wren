import "mv" for Math, Sprite, MyGame, Vec2

class MySprite {
    construct new() {
        _timer = 0
    }

    init(self) {}

    update(self, dt) {
        _timer = _timer + dt * 10
        // Sees self as Node here
        self.pos = Vec2.new(100 * _timer.sin, 100 * _timer.cos)
    }

    draw(self) {
        // Sees self as Sprite here????
        // self.pos = Vec2.new(100 * _timer.sin, 100 * _timer.cos)
    }

    input(self) {}
}


class Game {
    construct new() {
        _a_sprite = Sprite.new(MySprite.new(), "my_sprite")
    }

    entryPoint(context) {
        var node = context.getRootNode()
        var sprite = context.getKleinesTexture()

        _a_sprite.texture = sprite

        context.addSprite(_a_sprite)
    }
}

var Instance = Game.new()
