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

generate_action_tutorial() {
    convert -strip -background "#1f2029ff" -fill "#f9d72fff" -font  "$font_uni_b" -pointsize 8 -crop 16 -gravity Center label:"$1" $2
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
convert -strip resources/gfx/pngs/scientists/dark.png -crop 16 resources/gfx/sprites/scientists/dark-%d.png
convert -strip resources/gfx/pngs/scientists/idle0.png -crop 16 resources/gfx/sprites/scientists/idle0-%d.png
convert -strip resources/gfx/pngs/scientists/stressed0.png -crop 16 resources/gfx/sprites/scientists/stressed0-%d.png
convert -strip resources/gfx/pngs/scientists/hell0.png -crop 16 resources/gfx/sprites/scientists/hell0-%d.png
convert -strip resources/gfx/pngs/scientists/idle1.png -crop 16 resources/gfx/sprites/scientists/idle1-%d.png
convert -strip resources/gfx/pngs/scientists/stressed1.png -crop 16 resources/gfx/sprites/scientists/stressed1-%d.png
convert -strip resources/gfx/pngs/scientists/hell1.png -crop 16 resources/gfx/sprites/scientists/hell1-%d.png
convert -strip resources/gfx/pngs/scientists/idle2.png -crop 16 resources/gfx/sprites/scientists/idle2-%d.png
convert -strip resources/gfx/pngs/scientists/stressed2.png -crop 16 resources/gfx/sprites/scientists/stressed2-%d.png
convert -strip resources/gfx/pngs/scientists/hell2.png -crop 16 resources/gfx/sprites/scientists/hell2-%d.png
convert -strip resources/gfx/pngs/scientists/idle3.png -crop 16 resources/gfx/sprites/scientists/idle3-%d.png
convert -strip resources/gfx/pngs/scientists/stressed3.png -crop 16 resources/gfx/sprites/scientists/stressed3-%d.png
convert -strip resources/gfx/pngs/scientists/hell3.png -crop 16 resources/gfx/sprites/scientists/hell3-%d.png

# window
convert -strip resources/gfx/pngs/window/idle.png -crop 16 resources/gfx/sprites/window/idle-%d.png
convert -strip resources/gfx/pngs/window/stressed.png -crop 16 resources/gfx/sprites/window/stressed-%d.png
convert -strip resources/gfx/pngs/window/hell.png -crop 16 resources/gfx/sprites/window/hell-%d.png
convert -strip resources/gfx/pngs/window/hell_alt.png -crop 16 resources/gfx/sprites/window/hell_alt-%d.png


# message
generate  "#1f2029ff" "#f9d72fff" "$font" 20 'This game is best enjoyed\nusing the original N64 controller' resources/gfx/sprites/ui/message.png
generate  "#1f2029ff" "#f9d72fff" "$font" 20 'Please insert your Rumble Pak now\nif you wish to play with it' resources/gfx/sprites/ui/rumble_not_detected.png

# labels
generate_label  'LIGHTS' resources/gfx/sprites/ui/label_lights.png
generate_label  'E.M.F' resources/gfx/sprites/ui/label_radio.png
generate_label  'TURBINES' resources/gfx/sprites/ui/label_turbines.png
generate_label  'PUMPS' resources/gfx/sprites/ui/label_pumps.png
generate_label  'CONTROL RODS' resources/gfx/sprites/ui/label_control_rods.png
generate_label  'COMPASS' resources/gfx/sprites/ui/label_wind_turbines.png
# edited by hand
#generate_label  'AZ-S' resources/gfx/sprites/ui/label_az_5.png
generate_label  'PRESSURIZER' resources/gfx/sprites/ui/label_pressurizer.png
# edited by hand
generate_label  'G\nE\nI\nG\nE\nR\n\nL\nE\nV\nE\nL' resources/gfx/sprites/ui/label_danger.png

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



generate_action_tutorial "Welcome new team member!\n\nThe Fukumilebyl power plant\nis in dire need of your help!" resources/gfx/sprites/actions/tuto-welcome-%d.png
generate_action_tutorial "Your team of brave scientists\nwill be doing all the manual labor\nbut they need the reactor's components\nto be set properly from the control room." resources/gfx/sprites/actions/tuto-intro-%d.png
generate_action_tutorial "You will be in charge of three stations.\nLet's start by getting familiar\nwith the center station.\nYour team is counting on you!\nThey face grave danger!" resources/gfx/sprites/actions/tuto-center-%d.png
generate_action_tutorial "Keep an eye on the\nGeiger counter level!\nThe plant can explode\nif it's too high!\n" resources/gfx/sprites/actions/tuto-geiger-%d.png
generate_action_tutorial "Good Luck!\n" resources/gfx/sprites/actions/tuto-luck-%d.png

pos=("A1" "A2" "A3" "A4" "B1" "B2" "B3" "B4" "C1" "C2" "C3" "C4" "D1" "D2" "D3" "D4" "E1" "E2" "E3" "E4" "F1" "F2" "F3" "F4")
colors=("red" "blue" "green" "orange")
for po in ${pos[@]}; do
    for color in ${colors[@]}; do
        generate_action "Set $color\nControl\nRod\nto $po" resources/gfx/sprites/actions/rod_"$color"_"$po"-%d.png
    done
done
freqs=("200" "300" "400" "500" "150" "250" "350" "450" "100" "200" "300" "400" "50" "150" "250" "350" "225" "325" "425" "525" "175" "275" "375" "475" "125" "225" "325" "425" "75" "175" "275" "375" "250" "350" "450" "550" "200" "300" "400" "500" "150" "250" "350" "450" "100" "200" "300" "400" "275" "375" "475" "575" "225" "325" "425" "525" "175" "275" "375" "475" "125" "225" "325" "425" "195" "295" "395" "495" "145" "245" "345" "445" "95" "195" "295" "395" "45" "145" "245" "345" "220" "320" "420" "520" "170" "270" "370" "470" "120" "220" "320" "420" "70" "170" "270" "370" "245" "345" "445" "545" "195" "295" "395" "495" "145" "245" "345" "445" "95" "195" "295" "395" "270" "370" "470" "570" "220" "320" "420" "520" "170" "270" "370" "470" "120" "220" "320" "420" "190" "290" "390" "490" "140" "240" "340" "440" "90" "190" "290" "390" "40" "140" "240" "340" "215" "315" "415" "515" "165" "265" "365" "465" "115" "215" "315" "415" "65" "165" "265" "365" "240" "340" "440" "540" "190" "290" "390" "490" "140" "240" "340" "440" "90" "190" "290" "390" "265" "365" "465" "565" "215" "315" "415" "515" "165" "265" "365" "465" "115" "215" "315" "415" "185" "285" "385" "485" "135" "235" "335" "435" "85" "185" "285" "385" "35" "135" "235" "335" "210" "310" "410" "510" "160" "260" "360" "460" "110" "210" "310" "410" "60" "160" "260" "360" "235" "335" "435" "535" "185" "285" "385" "485" "135" "235" "335" "435" "85" "185" "285" "385" "260" "360" "460" "560" "210" "310" "410" "510" "160" "260" "360" "460" "110" "210" "310" "410")
for freq in ${freqs[@]}; do
    generate_action "Set E.M.F\nMonitor\nto\n${freq}Hz" resources/gfx/sprites/actions/freq-"$freq"-%d.png
done
press=("1000" "2000" "3000" "4000")
for pres in ${press[@]}; do
    generate_action "Set\npressure\nto\n${pres}Pa" resources/gfx/sprites/actions/press-"$pres"-%d.png
done

generate_action_tutorial "Set pressure to\n2000Pa by double tapping\nA button" resources/gfx/sprites/actions/tuto-press-2000-%d.png


dirs=("NorthWest" "North" "NorthEast" "West" "East" "SouthWest" "South" "SouthEast")
for dir in ${dirs[@]}; do
    generate_action "Set\nCompass\nto\n${dir}" resources/gfx/sprites/actions/compass-"$dir"-%d.png
done

powers=("0" "125" "250" "375" "500")
for power in ${powers[@]}; do
    generate_action "Set\nTurbines\nto\n${power}W" resources/gfx/sprites/actions/power-"$power"-%d.png
done
generate_action 'Activate\nthe Pumps' resources/gfx/sprites/actions/pumps-%d.png
generate_action 'Call\nSpare\nParts\n(see menu)' resources/gfx/sprites/actions/call-spare-%d.png

generate_action 'Press AZ-5\nwhen\nturbines\nare off' resources/gfx/sprites/actions/az5-turbines-%d.png
generate_action 'Press AZ-5\nwhen\ncompass\npoints\nNorth' resources/gfx/sprites/actions/az5-compass-%d.png

# tutorial
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the stations' resources/gfx/sprites/ui/how_to.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the LEFT station' resources/gfx/sprites/ui/how_to_left.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the CENTER station' resources/gfx/sprites/ui/how_to_center.png
generate "#1f2029ff" "#f9d72fff" "$font_uni_b" 10 'How to manage the RIGHT station' resources/gfx/sprites/ui/how_to_right.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'LEFT' resources/gfx/sprites/ui/left.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'CENTER' resources/gfx/sprites/ui/center.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'RIGHT' resources/gfx/sprites/ui/right.png
generate "#1f2029ff" "#f9d72fff" "$font" 18 'Use L and R to change station' resources/gfx/sprites/ui/use_l_r.png
generate_left "#1f2029ff" "#f9d72fff" "$font" 16 "Use d-pad to tweak the RADIO\n\nUse Z to press AZ-5\n\nUse the joystick to set the COMPASS" resources/gfx/sprites/ui/tuto_left.png
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
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "Isabel Jimenez & Victor Vieux" resources/gfx/sprites/ui/isabel_victor.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "www.vrgl117.games" resources/gfx/sprites/ui/vrgl117games.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "art" resources/gfx/sprites/ui/art.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "jphosho" resources/gfx/sprites/ui/jphosho.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "IG: @jphosho" resources/gfx/sprites/ui/atjphosho.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "music" resources/gfx/sprites/ui/music.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "manuhoz" resources/gfx/sprites/ui/manuhoz.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "IG/TT: @radiatorhymn" resources/gfx/sprites/ui/radiatorhymn.png
generate "#1f2029ff" "#f9d72fff" "$font_uni" 16 "sfx" resources/gfx/sprites/ui/sfx.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "GowlerMusic" resources/gfx/sprites/ui/gowlermusic.png
generate "#1f2029ff" "#F0F1FAff" "$font" 16 "LittleRobotSoundFactory" resources/gfx/sprites/ui/littlerobotsoundfactory.png

# phonebook
convert -strip -background "#F0F1FAff" -bordercolor "#F0F1FAff"  -border 2x  -fill "#1f2029ff"  -font "$font_uni" -pointsize 12 -interline-spacing -5 -gravity Center label:"phonebook\n\nCommissioner............0598-4627\nEmergency Services............911\nFuel Procurement......5294-2647\nIncident Response.....4676-4729\nNintendo...............800-255-3700\nNuclear Safety..........5733-4738\nPresident..................2982-2941\nPublic Affairs..........1029-4859\nReactor Supervisor..7268-2638\nSpare Parts...............3905-1273\nTreasury...................3648-3957" resources/gfx/sprites/phonebook.png

# studio logo
convert -strip -brightness-contrast -10 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/pngs/intro/vrgl117_logo.png resources/gfx/sprites/intro/vrgl117_logo_1.png

# n64 + n64brew jam logo
convert -strip -brightness-contrast -10 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_9.png
convert -strip -brightness-contrast -20 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_8.png
convert -strip -brightness-contrast -30 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_7.png
convert -strip -brightness-contrast -40 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_6.png
convert -strip -brightness-contrast -50 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_5.png
convert -strip -brightness-contrast -60 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_4.png
convert -strip -brightness-contrast -70 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_3.png
convert -strip -brightness-contrast -80 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_2.png
convert -strip -brightness-contrast -90 resources/gfx/pngs/intro/n64brew_jam_logo.png resources/gfx/sprites/intro/n64brew_jam_logo_1.png
