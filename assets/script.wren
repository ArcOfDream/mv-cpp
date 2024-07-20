import "mv" for Math, Sprite, MyGame, Vec2

class MySprite {
    construct new() {
        _timer = 0
    }

    init(self) {}

    update(self, dt) {
        _timer = _timer + dt * 4
        self.pos = Vec2.new(100 * _timer.sin, 100 * _timer.cos)
    }

    draw(self) {}
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
