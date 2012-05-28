"""
Monitor program, provide:

GUI:
* VideoFrame:                           # Done
    A YUV Layer rendering 16CIF/4D1 Videos.
    * Pop Up Menu                       # Done
        * Provide video switching       # Done
        * parm setting
    * Dialog auto avoid                 # Done
* MonitorDialog:                        # Done
    Switching video source, recorded file, querying for recorded file.
    Start/End filter                    # Done
    Date selection                      # Done
    Generate rec list with each click   # Done
    switch video                        # Done
    switch record                       # Done
* MenuBar:                              # Done
    A toolbar which lead to all the functions of the system, listed as below: 
    Corp Icon: An icon of the corp.     # Done
    Monitor Button:                     # Done
    Partol Button:                      # Done
    Layout Button:                      # Done
    Emergency Button:                   # Done
    Management Button:                  # Done
    Temporarily Recording Button:       # Done
    Capture Button:                     # Done
    Information Button:                 # Done
    System Button:                      # Done
* ControlBar:                           # Done
    A toolbar provide progress bar and a speed adjusting bar.
    * Progress bar:                     # Done
    * Speed adjusting bar:              # Done
    * Previous/Next track               # Done
    * Current Playing                   # Done
* PartolDialog:                         # Done
    Set partol list, partol speed, and partol region.
    * Multiselection in station list    # Done
    * MoveIn/MoveOut                    # Done
    * Readjust orders in patrol list by Drag and Drop # Done
    * save/load/delete scheme           # Done
    * Patrol Control
    * Patrol multiselection mode        # Done
* LayoutDialog:                         # Done
    * Set Layout                        # Done
    * save/load/delete Layout scheme.   # Done
* EmergencyDialog:                      # Done
    View alarm log, write handling opinion.
    * Add alarm event                   # Done
    * Handle alarm                      # Done
      FIXME: sync with other terminals 
    * View handle opinions              # Done
* ManagementDialog:                     # Done
    A embedded HTML engine to a HTML based admin page.
* TemporarilyRecordingDialog:           # Done
    TemporarilyRecording startup, shutdown, and switch to.
    * Show list                         # Done
    * switch                            # Done
    * startup/shutdown
    * Popup menu
* CaptureDialog:                        # Done
    Video grab, view.
    * ZoomIn                            # Done
    * ZoomOut                           # Done
    * Zoom1:1                           # Done
    * Capture
    * Popup menu
* InformationDialog:                    # Designed
    Adjust of audio volumn, set video HUB and contrast, set/jumpto presetting position of camera.
    * Need cowork with Encoder
* SystemDialog:                         # Done
    View basic information, logout/reboot/shutdown system.

MileStone:

Camera control
Control Bar                             # Done
Switch video                            # Done
Switch Playback                         # Done
  Switch temp rec                       # Done
Patrol
  multiple selection                    # Done
    colorful selection                  # Cancel
Switch layout                           # Done
  save/restore layout/src
Alarm Window                            # Done
Grab video
Video encoder parm adjust               # Done
Audio support                           # Done
Engine optimization
  Plugin rubust enhancement             # Done
  decoder switching speed improvement   # Done
  Bilinear intersection enabled         # Done
  DS40XXHC cards support                # Done
  
Recorder:                               # Done
  Query recording                       # Done
  Set recording                         # Done
  5 minutes record ahead                # Done
Encoder:                                # Done
  Parm query                            # Done
  Parm adjusting                        # Done
  OSD support                           # Done
"""
##import psyco
##psyco.full()

import gtk, gtk.glade, gtk.gdk
import os, time
import sitemanager

import UI.VideoFrame, UI.MenuBar, UI.CaptureDlg, UI.ControlBar
import UI.DateSelection, UI.EmergencyDlg, UI.InformationDlg, UI.LayoutDlg
import UI.ManagementDlg, UI.MonitorDlg, UI.PatrolDlg, UI.SystemDlg
import UI.TempRecDlg, UI.LoginForm

import CameraControl


class WidgetsWrapper:
    def __init__(self):
        self.widgets = gtk.glade.XML ('./UI/ui.glade')
        for i in [UI.VideoFrame.VideoFrameHandlers,
                  UI.MenuBar.MenuBarHandlers,
                  UI.CaptureDlg.CaptureDlgHandlers,
                  UI.ControlBar.ControlBarHandlers,
                  UI.DateSelection.DateSelectionHandlers,
                  UI.EmergencyDlg.EmergencyDlgHandlers,
                  UI.InformationDlg.InformationDlgHandlers,
                  UI.LayoutDlg.LayoutDlgHandlers,
                  UI.ManagementDlg.ManagementDlgHandlers,
                  UI.MonitorDlg.MonitorDlgHandlers,
                  UI.PatrolDlg.PatrolDlgHandlers,
                  UI.SystemDlg.SystemDlgHandlers,
                  UI.TempRecDlg.TempRecDlgHandlers,
                  UI.LoginForm.LoginFormHandlers]:
            self.widgets.signal_autoconnect(i.__dict__)

        self['LoginFrame'].show()
##        self['MainFrame'].fullscreen()
##            self['MainFrame'].set_size_request(800, 600)

    def __getitem__(self, key):
        """
        Gives us the ability to do: widgets['widget_name'].action()
        """
        return self.widgets.get_widget(key)

if __name__=='__main__':
    os.nice(-9)
    gtk.threads_init()
    widgets = WidgetsWrapper()
    
    for i in [UI.VideoFrame, UI.MenuBar, UI.CaptureDlg, UI.ControlBar,
              UI.DateSelection, UI.EmergencyDlg, UI.InformationDlg,
              UI.LayoutDlg, UI.ManagementDlg, UI.MonitorDlg, UI.PatrolDlg,
              UI.SystemDlg, UI.TempRecDlg, CameraControl, UI.LoginForm]:
        i.setWidgets(widgets)
    gtk.main()
