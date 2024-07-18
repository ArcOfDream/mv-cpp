import "mv" for Math, Node, Sprite, MyGame

var lerped_value = Math.lerp(0, 1, 0.5)

var a_node = Node.new("wren_node")

System.print("Hello, %(lerped_value)!")
