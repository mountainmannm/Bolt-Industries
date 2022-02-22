print("Starting")

import board


from kmk.kmk_keyboard import KMKKeyboard
from kmk.keys import KC
from kmk.matrix import DiodeOrientation
from kmk.modules.layers import Layers
from kmk.keys import KC, make_key
#from kmk.consts import UnicodeMode
#from kmk.handlers.sequences import compile_unicode_string_sequences as cuss
Pico87 = KMKKeyboard()

Pico87.col_pins =( 
        board.GP0,
        board.GP1,
        board.GP2,
        board.GP3,     
        board.GP4,
        board.GP5,
        board.GP6,
        board.GP7,
	board.GP8,
        board.GP9,
        board.GP10,
        board.GP11,
        board.GP12,
        board.GP13,
        board.GP14,
        board.GP15,
        board.GP16,
        board.GP17
        )
Pico87.row_pins = (
        board.GP18,
	board.GP19,
	board.GP20,
	board.GP21,
	board.GP22,
        board.GP26,
       ) 
    
Pico87.diode_orientation = DiodeOrientation.COLUMNS


import board
print(dir(board))



layers = Layers()
Pico87.modules = [layers]
#Pico87.unicode_mode = UnicodeMode.LINUX

#from kmk.handlers.sequences import compile_unicode_string_sequences as cuss

#emoticons = cuss({
#	'BEER': r'🍺',
#	'HAND_WAVE': r'👋',
#})

#SMILE = KC.LCTL,(KC.LSHIFT, KC.U)
#SF_MINS = KC.MT(KC.MINS, KC.LSHIFT)

Pico87.keymap = [
   
  [ # Layer 0 QWERTY
KC.ESC,   KC.NO, KC.F1, KC.F2,  KC.F3,  KC.F4, KC.NO,  KC.F5, KC.F6, KC.F7,   KC.F8,   KC.F9,    KC.F10,   KC.F11,  KC.F12, KC.PSCREEN, KC.SCROLLLOCK, KC.PAUSE,
KC.GRAVE, KC.N1, KC.N2, KC.N3,  KC.N4,  KC.N5, KC.N6,  KC.N7, KC.N8, KC.N9,   KC.N0,   KC.MINUS, KC.EQUAL, KC.NO,     KC.BSPC, KC.INS,  KC.HOME,       KC.PGUP,
KC.TAB,   KC.NO,  KC.Q,  KC.W,   KC.E,   KC.R,  KC.T,   KC.Y,  KC.U,  KC.I,    KC.O,    KC.P,     KC.LBRC,  KC.RBRC,   KC.BSLS, KC.DEL, KC.END,       KC.PGDN,
KC.CAPS,  KC.NO,    KC.A,  KC.S,   KC.D,   KC.F,  KC.G,   KC.H,  KC.J,  KC.K,    KC.L,    KC.SCLN,  KC.QUOT,  KC.ENT,    KC.NO, KC.NO,      KC.NO,      KC.NO,
KC.NO,    KC.LSHIFT, KC.Z,  KC.X,   KC.C,   KC.V,  KC.B,   KC.N,  KC.M,  KC.COMM, KC.DOT,  KC.SLSH,  KC.NO,    KC.RSHIFT, KC.NO,    KC.NO,   KC.UP,   KC.NO,
KC.LCTL,  KC.LGUI,   KC.NO, KC.LALT, KC.NO, KC.NO, KC.SPC, KC.NO, KC.NO, KC.NO,   KC.RALT, KC.RGUI,  KC.NO,    KC.MO(1),     KC.RCTRL, KC.LEFT, KC.DOWN, KC.RIGHT,
  ],
  
    [# Layer 1
KC.ESC,   KC.NO, KC.F1, KC.F2,  KC.F3,  KC.F4, KC.NO,  KC.F5, KC.F6, KC.F7,   KC.F8,   KC.F9,    KC.F10,   KC.F11,  KC.F12, KC.PSCREEN, KC.SCROLLLOCK, KC.PAUSE,
KC.GRAVE, KC.N1, KC.N2, KC.N3,  KC.N4,  KC.N5, KC.N6,  KC.N7, KC.N8, KC.N9,   KC.N0,   KC.MINUS, KC.EQUAL, KC.NO,     KC.BSPC, KC.INS,  KC.HOME,       KC.PGUP,
KC.TAB,   KC.NO,  KC.Q,  KC.W,   KC.E,   KC.R,  KC.T,   KC.Y,  KC.U,  KC.I,    KC.O,    KC.P,     KC.LBRC,  KC.RBRC,   KC.BSLS, KC.DEL, KC.END,       KC.PGDN,
KC.CAPS,  KC.NO,    KC.A,  KC.S,   KC.D,   KC.F,  KC.G,   KC.H,  KC.J,  KC.K,    KC.L,    KC.SCLN,  KC.QUOT,  KC.ENT,    KC.NO, KC.NO,      KC.NO,      KC.NO,
KC.NO,    KC.LSHIFT, KC.Z,  KC.X,   KC.C,   KC.V,  KC.B,   KC.N,  KC.M,  KC.COMM, KC.DOT,  KC.SLSH,  KC.NO,    KC.RSHIFT, KC.NO,    KC.NO,   KC.UP,   KC.NO,
KC.LCTL,  KC.LGUI,   KC.NO, KC.LALT, KC.NO, KC.NO, KC.SPC, KC.NO, KC.NO, KC.NO,   KC.RALT, KC.RGUI,  KC.NO,    KC.MO(1),     KC.RCTRL, KC.LEFT, KC.DOWN, KC.RIGHT,
  ],
]


if __name__ == '__main__':
    Pico87.go()
