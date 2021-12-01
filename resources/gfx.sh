#!/usr/bin/env bash

font='resources/fonts/sh-pinscher/SHPinscher-Regular.otf'
font_uni='resources/fonts/unibody/Unibody8Pro-RegularSmallCaps.otf'
font_uni_b='resources/fonts/unibody/Unibody8Pro-Bold.otf'

generate() {
    convert -strip -background $1 -fill $2 -font $3 -pointsize $4 -gravity center label:"$5" $6
}


generate_left() {
    convert -strip -background $1 -fill $2 -font $3 -pointsize $4 -gravity West label:"$5" $6
}

generate_action() {
    convert -strip -background "#1f2029ff" -fill "#f9d72fff" -font  "$font_uni_b" -pointsize 8 -crop 16 -gravity West label:"$1" $2
}

generate_label() {
    convert -strip -background "#665544ff" -bordercolor "#665544ff"  -border 2x  -fill "#ffffffff"  -font "$font_uni" -pointsize 8 -interline-spacing -5 -gravity Center label:"$1" $2
}

generate_text() {
    convert -strip -background "#5b607aff" -fill "#f0f1faff"  -font "$font_uni" -pointsize 12 -extent 10x10 -gravity Center label:"$1" $2
}

generate_direction() {
    convert -strip -background "#f9d72fff" -fill "#1f2029ff"  -font "$font_uni" -pointsize 7 -extent 13x10 -gravity Center label:"$1" $2
}


# scientist
convert -strip resources/gfx/sprites/scientists/dark.png -crop 16 resources/gfx/sprites/scientists/dark-%d.png
convert -strip resources/gfx/sprites/scientists/idle0.png -crop 16 resources/gfx/sprites/scientists/idle0-%d.png
convert -strip resources/gfx/sprites/scientists/stressed0.png -crop 16 resources/gfx/sprites/scientists/stressed0-%d.png
convert -strip resources/gfx/sprites/scientists/hell0.png -crop 16 resources/gfx/sprites/scientists/hell0-%d.png
convert -strip resources/gfx/sprites/scientists/idle1.png -crop 16 resources/gfx/sprites/scientists/idle1-%d.png
convert -strip resources/gfx/sprites/scientists/stressed1.png -crop 16 resources/gfx/sprites/scientists/stressed1-%d.png
convert -strip resources/gfx/sprites/scientists/hell1.png -crop 16 resources/gfx/sprites/scientists/hell1-%d.png
convert -strip resources/gfx/sprites/scientists/idle2.png -crop 16 resources/gfx/sprites/scientists/idle2-%d.png
convert -strip resources/gfx/sprites/scientists/stressed2.png -crop 16 resources/gfx/sprites/scientists/stressed2-%d.png
convert -strip resources/gfx/sprites/scientists/hell2.png -crop 16 resources/gfx/sprites/scientists/hell2-%d.png
convert -strip resources/gfx/sprites/scientists/idle3.png -crop 16 resources/gfx/sprites/scientists/idle3-%d.png
convert -strip resources/gfx/sprites/scientists/stressed3.png -crop 16 resources/gfx/sprites/scientists/stressed3-%d.png
convert -strip resources/gfx/sprites/scientists/hell3.png -crop 16 resources/gfx/sprites/scientists/hell3-%d.png

# message
generate "#000000ff" "#ffff00ff" "$font" 24 'This game is best\nenjoyed using the\noriginal N64 controller' resources/gfx/sprites/ui/message.png

# labels
generate_label  'LIGHTS' resources/gfx/sprites/ui/label_lights.png
generate_label  'RADIO' resources/gfx/sprites/ui/label_radio.png
generate_label  'TURBINES' resources/gfx/sprites/ui/label_turbines.png
generate_label  'PUMPS' resources/gfx/sprites/ui/label_pumps.png
generate_label  'CONTROL RODS' resources/gfx/sprites/ui/label_control_rods.png
generate_label  'COMPASS' resources/gfx/sprites/ui/label_wind_turbines.png
# edited by hand
#generate_label  'AZ-S' resources/gfx/sprites/ui/label_az_5.png
generate_label  'PRESSURIZER' resources/gfx/sprites/ui/label_pressurizer.png
# edited by hand
#generate_label  'D\nA\nN\nG\nE\nR\n\nL\nE\nV\nE\nL' resources/gfx/sprites/ui/label_danger.png

generate_text 'a'  resources/gfx/sprites/ui/text_a.png
generate_text 'b'  resources/gfx/sprites/ui/text_b.png
generate_text 'c'  resources/gfx/sprites/ui/text_c.png
generate_text 'd'  resources/gfx/sprites/ui/text_d.png
generate_text 'e'  resources/gfx/sprites/ui/text_e.png
generate_text 'f'  resources/gfx/sprites/ui/text_f.png
generate_text '0'  resources/gfx/sprites/ui/text_0.png
generate_text '1'  resources/gfx/sprites/ui/text_1.png
generate_text '2'  resources/gfx/sprites/ui/text_2.png
generate_text '3'  resources/gfx/sprites/ui/text_3.png
generate_text '4'  resources/gfx/sprites/ui/text_4.png

generate_direction 'NW'  resources/gfx/sprites/ui/dir_nw.png
generate_direction 'N'  resources/gfx/sprites/ui/dir_n.png
generate_direction 'NE'  resources/gfx/sprites/ui/dir_ne.png
generate_direction 'W'  resources/gfx/sprites/ui/dir_w.png
generate_direction 'E'  resources/gfx/sprites/ui/dir_e.png
generate_direction 'SW'  resources/gfx/sprites/ui/dir_sw.png
generate_direction 'S'  resources/gfx/sprites/ui/dir_s.png
generate_direction 'SE'  resources/gfx/sprites/ui/dir_se.png

# actions
pos=("A1" "A2" "A3" "A4" "B1" "B2" "B3" "B4" "C1" "C2" "C3" "C4" "D1" "D2" "D3" "D4" "E1" "E2" "E3" "E4" "F1" "F2" "F3" "F4")
colors=("red" "blue" "green" "orange")
for po in ${pos[@]}; do
    for color in ${colors[@]}; do
        generate_action "Set $color\nControl\nRod\nto $po" resources/gfx/sprites/actions/rod_"$color"_"$po"-%d.png
    done
done
generate_action 'Turn\nOFF the\lights' resources/gfx/sprites/actions/power_off-%d.png
generate_action 'Turn the\lights\nback ON' resources/gfx/sprites/actions/power_on-%d.png
freqs=("100" "101" "107" "108" "110" "111" "114" "115" "117" "118" "124" "125" "126" "127" "131" "132" "133" "134" "139" "140" "141" "143" "144" "146" "147" "148" "150" "151" "153" "156" "157" "158" "159" "160" "160" "163" "164" "165" "166" "167" "170" "172" "173" "173" "174" "176" "177" "177" "179" "180" "180" "181" "183" "184" "186" "187" "189" "190" "190" "191" "193" "193" "194" "196" "197" "197" "198" "200" "203" "204" "205" "206" "207" "210" "210" "211" "212" "213" "214" "217" "218" "219" "220" "222" "223" "224" "225" "226" "227" "229" "230" "231" "232" "235" "236" "237" "238" "239" "239" "242" "243" "244" "245" "246" "249" "251" "252" "252" "253" "255" "256" "256" "258" "259" "259" "260" "262" "263" "265" "266" "268" "269" "269" "270" "272" "272" "273" "275" "276" "276" "277" "279" "282" "283" "284" "285" "286" "289" "289" "290" "291" "292" "293" "296" "297" "298" "299" "301" "302" "303" "304" "305" "306" "308" "309" "310" "311" "314" "315" "316" "317" "318" "318" "321" "322" "323" "324" "325" "328" "330" "331" "331" "332" "334" "335" "335" "337" "338" "338" "339" "341" "342" "344" "345" "347" "348" "348" "349" "351" "351" "352" "354" "355" "355" "356" "358" "361" "362" "363" "364" "365" "368" "368" "369" "370" "371" "372" "375" "377" "378" "380" "381" "382" "384" "385" "387" "388" "389" "394" "395" "396" "397" "401" "402" "403" "404" "410" "411" "413" "414" "417" "418" "420" "421" "427" "428" "430" "434" "435" "437" "444" "447" "451" "454" "461" "468" "60" "67" "74" "77" "81" "84" "91" "93" "94" "98")
for freq in ${freqs[@]}; do
    generate_action "Set freq\nto\n${freq}Hz" resources/gfx/sprites/actions/freq-"$freq"-%d.png
done
generate_action 'Call\nSpare\nParts\n(see menu)' resources/gfx/sprites/actions/call-spare-%d.png

# tutorial
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the stations' resources/gfx/sprites/ui/how_to.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the LEFT station' resources/gfx/sprites/ui/how_to_left.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the CENTER station' resources/gfx/sprites/ui/how_to_center.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the RIGHT station' resources/gfx/sprites/ui/how_to_right.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'LEFT' resources/gfx/sprites/ui/left.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'CENTER' resources/gfx/sprites/ui/center.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'RIGHT' resources/gfx/sprites/ui/right.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'Use L and R to change station' resources/gfx/sprites/ui/use_l_r.png
generate_left "#1f2029ff" "#f9d72fff" "$font" 16 "Use d-pad to tweak the RADIO\n\nDo (not) use Z to press AZ-5\n\nUse the joystick to set the COMPASS" resources/gfx/sprites/ui/tuto_left.png
generate_left "#1f2029ff" "#f9d72fff" "$font" 16 "Use d-pad and C buttons to interact\nwith the CONTROL RODS\n\nPress B to switch the LIGHTS\n\nDouble / triple tap A to activate\nthe PRESSURIZER" resources/gfx/sprites/ui/tuto_center.png
generate_left "#1f2029ff" "#f9d72fff" "$font" 16 "Rotate the joystick to activate the PUMPS\n\nPress Z to switch between the TURBINES & KEYPAD\n\nUse A to interact with TURBINES & KEYPAD\n\nPress B to clear out KEYPAD" resources/gfx/sprites/ui/tuto_right.png


# title screen
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 24 "Fission\nFailure\n64" resources/gfx/sprites/ui/logo.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "START" resources/gfx/sprites/ui/caps_start.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "START" resources/gfx/sprites/ui/caps_start_selected.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "TUTORIAL" resources/gfx/sprites/ui/caps_tutorial.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "TUTORIAL" resources/gfx/sprites/ui/caps_tutorial_selected.png

# win / game over screens
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 24 "Well Done!" resources/gfx/sprites/ui/win.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 24 "Game Over..." resources/gfx/sprites/ui/gameover.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "CONTINUE" resources/gfx/sprites/ui/continue.png

# pause screen
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 24 "Pause" resources/gfx/sprites/ui/pause_big.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Resume" resources/gfx/sprites/ui/resume.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "Resume" resources/gfx/sprites/ui/resume_selected.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Phonebook" resources/gfx/sprites/ui/phonebook.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "Phonebook" resources/gfx/sprites/ui/phonebook_selected.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Tutorial" resources/gfx/sprites/ui/tutorial.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "Tutorial" resources/gfx/sprites/ui/tutorial_selected.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Credits" resources/gfx/sprites/ui/credits.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "Credits" resources/gfx/sprites/ui/credits_selected.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Quit" resources/gfx/sprites/ui/quit.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 "Quit" resources/gfx/sprites/ui/quit_selected.png


# credits screen
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 24 "Credits" resources/gfx/sprites/ui/credits_big.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "programming" resources/gfx/sprites/ui/programming.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "Isabel Jimenez & Victor Vieux" resources/gfx/sprites/ui/isabel_victor.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "www.vrgl117.games" resources/gfx/sprites/ui/vrgl117games.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "art" resources/gfx/sprites/ui/art.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "jphosho" resources/gfx/sprites/ui/jphosho.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "IG: @jphosho" resources/gfx/sprites/ui/atjphosho.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "music" resources/gfx/sprites/ui/music.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "manuhoz" resources/gfx/sprites/ui/manuhoz.png
generate "#1f2029ff" "#F0F1FAff" "$font" 18 "IG/TT: @radiatorhymn" resources/gfx/sprites/ui/radiatorhymn.png

# phonebook
convert -strip -background "#F0F1FAff" -bordercolor "#F0F1FAff"  -border 2x  -fill "#1f2029ff"  -font "$font_uni" -pointsize 12 -interline-spacing -5 -gravity Center label:"phonebook\n\nCommissioner............0598-4627\nEmergency Services............911\nFuel Procurement......5294-2647\nIncident Response.....4676-4729\nNintendo...............800-255-3700\nNuclear Safety..........5733-4738\nPresident..................2982-2941\nPublic Affairs..........1029-4859\nReactor Supervisor..7268-2638\nSpare Parts...............3905-1273\nTreasury...................3648-3957" resources/gfx/sprites/phonebook.png

# studio logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_1.png

# n64 + n64brew jam logo
convert -strip -brightness-contrast -10 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/sprites/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_1.png
