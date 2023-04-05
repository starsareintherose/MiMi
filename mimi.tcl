package require Tk

proc ? L {lindex $L [expr {int (rand ()* [llength $L])}]}

proc validateFile name {
  set ext [file extension $name]
  if {$ext ni {.fas .fasta .nex .nexus .tnt .ss .phy .phylip}} {
    tk_messageBox -icon error -type ok -message "extension of file must be fas fasta nex nexus tnt ss phy phylip"
    return 0
  } else {
    return 1
  }
}

proc runMimi {} {
  global input output mimi
  if {[validateFile $input] && [validateFile $output]} {
    if {[file exists $mimi]} {
      exec $mimi -i $input -o $output
      tk_messageBox -icon info -type ok -message "Success"
    } else {
      tk_messageBox -icon error -type ok -message "Can't find the path of MiMi"
    }
  }
}

proc clearAll {} {
  global input output mimi
  set input ""
  set output ""
}

proc chooseMimi {} {
  global mimi
  set file [tk_getOpenFile]
  if {$file ne ""} {
    set mimi $file
  }
}

proc chooseInput {} {
  global input
  set file [tk_getOpenFile]
  if {$file ne ""} {
    set input $file
  }
}

proc chooseOutput {} {
  global output
  set file [tk_getSaveFile]
  if {$file ne ""} {
    set output $file
  }
}

proc quitApp {} {
  exit
}

proc helpApp {} { 
  tk_messageBox -icon info -type ok -message "Thanks to my beloved daughter MiMi (Ragdoll cat) for accompanying me in my life, mom loves you forever.\n\nGuoyi"
}

#bind . <Escape> {quitApp}
wm protocol . WM_DELETE_WINDOW {quitApp}

wm iconphoto . [image create photo -file logo.gif]

set input ""
set output ""
set mimi ""

wm title . "Mimi GUI"

# 创建菜单栏
menu .menubar
.menubar add cascade -label "Settings" -menu .menubar.file
menu .menubar.file
.menubar.file add command -label "Select MiMi Path" -command chooseMimi
.menubar.file add separator
.menubar.file add command -label "Quit" -command quitApp

.menubar add cascade -label "Help" -menu .menubar.help
menu .menubar.help
.menubar.help add command -label "About" -command helpApp

# 配置菜单栏到顶层窗口
. configure -menu .menubar

# 创建输入输出框和按钮
grid [ttk::frame .c -padding "3 3 12 12"] -column 0 -row 0 -sticky nwes
grid columnconfigure . 0 -weight 1; grid rowconfigure . 0 -weight 1

grid [ttk::label .c.ilbl -text "Input"] -column 1 -row 1 -sticky e
grid [ttk::entry .c.input -width 20 -textvariable input] -column 2 -row 1 -sticky we
grid [ttk::button .c.ibtn -text "Select" -command chooseInput] -column 3 -row 1

grid [ttk::label .c.olbl -text "Output"] -column 1 -row 2 -sticky e
grid [ttk::entry .c.output -width 20 -textvariable output] -column 2 -row 2 -sticky we
grid [ttk::button .c.obtn -text "Select" -command chooseOutput] -column 3 -row 2

grid [ttk::button .c.run -text "Run" -command runMimi] -column 2 -row 3
grid [ttk::button .c.clear -text "Clean" -command clearAll] -column 3 -row 3

foreach w [winfo children .c] {grid configure $w -padx 5 -pady 5 }

vwait forever
