extends Node


var t = 0
var progress = 0
const keys = [
	"SUPER",
	"C",
	"H",
	"R",
	"O",
	"M",
	"ENTER",
	"C",
	"A",
	"T",
	"ENTER",
	"F5",
]

func _ready() -> void:
	pass

func _process(delta: float) -> void:
	t += delta
	if t > 0.5:
		$Keyboard.key_press(keys[progress])
		progress += 1
		t = 0
		if progress >= len(keys):
			get_tree().quit()
