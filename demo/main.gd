extends Node


var t = 0
var progress = 0
const keys = [
#	"SUPER",
#	"C",
#	"H",
#	"R",
#	"O",
#	"M",
#	"ENTER",
#	"C",
#	"A",
#	"T",
#	"ENTER",
#	"F5",
#	"VOLUMEDOWN",
	"A",
	"A",
	"A",
	"A",
	"ADIAERSIS",
	"ARING",
	"DIAERSIS",
	"O",
	"PERIOD",
]


func _ready() -> void:
	pass

func _process(delta: float) -> void:
	t += delta
	if t > 0.5:
		GDVK.key_press(keys[progress])
		progress += 1
		t = 0
		if progress >= len(keys):
			print("Done")
			get_tree().quit()
