import "mv" for Math, Sprite, MyGame

var lerped_value = Math.lerp(0, 1, 0.5)

var kleines_root = MyGame.getRootNode()
var kleines_sprite = MyGame.getKleinesTexture()

var custom_sprite = Sprite.new("wren_sprite", kleines_sprite)
kleines_root.addChild(custom_sprite)

System.print("Hello, %(lerped_value)!")