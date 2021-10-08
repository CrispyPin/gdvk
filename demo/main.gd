extends Node


var t = 0
var typed = false

func _ready() -> void:
	pass

func _process(delta: float) -> void:
	t += delta
	if t > 3 and !typed:
		p()
		typed = true
	KEY_ENTER

func p():
	for w in "this was typed from within a godot plugin!".split(" "):
		word(w)
		$Keyboard.key_press("space")

func word(w):
	for c in w:
		$Keyboard.key_press(c)
