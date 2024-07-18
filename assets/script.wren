import "mv" for Math, Sprite, MyGame, Vec2

class MySprite {
    construct new() {
        _timer = 0
    }

    init(self) {}

    update(self, dt) {
        _timer = _timer + dt

        self.pos.x = 100 * sin(_timer)
        self.pos.y = 100 * cos(_timer)
    }

    // unused atm
    draw(self) {
        System.print("drawcall invoked!")
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

        context.addNode(_a_sprite)
    }
}

var Instance = Game.new()
