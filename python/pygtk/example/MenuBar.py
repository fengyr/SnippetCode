"""
Signal handling for MenuBar
"""

import PatrolDlg, CaptureDlg, InformationDlg, EmergencyDlg
import gobject, gtk, gtk.gdk

Blinking=0
style=None

def setWidgets(w):
    global widgets
    global style
    
    widgets=w
    widgets.CurrentDlg=None

    style=widgets["EmergencyBtn"].get_modifier_style()
    widgets.emergence=False
    gobject.timeout_add(500, BlinkEmBtn)


def Enter_Emergence():
    global widgets
    
    widgets.emergence=True
    widgets['MenuBar'].show()

def Exit_Emergence():
    global widgets
    global style
    
    widgets.emergence=False
    widgets['EmergencyBtn'].modify_style(style)

def BlinkEmBtn():
    global Blinking
    global style

    if not widgets.emergence:
        return 100
    
    if Blinking:
        widgets["EmergencyBtn"].modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("red"))
        widgets["EmergencyBtn"].modify_bg(gtk.STATE_PRELIGHT, gtk.gdk.color_parse("DarkRed"))
        Blinking=0
    else:
        widgets["EmergencyBtn"].modify_style(style)
        Blinking=1
    return 1000

def CloseCurrentWidgets():
    """
    This func hide current widgets.
    """
    global widgets
    if widgets.CurrentDlg!=None:
        widgets.CurrentDlg.hide()
        widgets.RenderThread.MulSelection=False
        widgets.CurrentDlg=None

def CheckCurrentWidgets():
    """
    This func check if current dlg is functionable.
    """
    global widgets
    if widgets.CurrentDlg!=None:
        pass

def BtnClicked(dlg):
    global widgets
    if widgets.CurrentDlg==widgets[dlg]:
        CloseCurrentWidgets()
        return
    CloseCurrentWidgets()
    widgets[dlg].show()
    widgets[dlg].set_transient_for(widgets['MainFrame'])
    if dlg=='PatrolDlg': PatrolDlg.StateCheck()
    widgets.CurrentDlg=widgets[dlg]
    if dlg=='InformationDlg': InformationDlg.SelectionChanged()
    if dlg=='EmergencyDlg': EmergencyDlg.FlushAlarmList()
    AdjustPosition()
    CheckCurrentWidgets()

def AdjustPosition():
    """
    Make sure that current dialog will never cover the current selected cell.
    """
    global widgets
    if widgets.RenderThread.MulSelection: return
        
    WinSize=widgets['MainFrame'].get_size()
    try:
        cell=widgets.RenderThread.selected[0]
        rect_size=cell.getSize()
        rect_size=(rect_size[0]*WinSize[0]/widgets.RenderThread.RealSize[0],
                   rect_size[1]*WinSize[1]/widgets.RenderThread.RealSize[1])
        rect=cell.getPosition()
        rect=(rect[0]*WinSize[0]/widgets.RenderThread.RealSize[0],
              rect[1]*WinSize[1]/widgets.RenderThread.RealSize[1])
        rect=rect+(rect[0]+rect_size[0], rect[1]+rect_size[1])
    except Exception, e:
        return

    if widgets.CurrentDlg!=None:
        w, h=widgets.CurrentDlg.get_size()
        x, y=widgets.CurrentDlg.get_position()
        h+=20
        w+=4
        if rect[0]<x+w<rect[2] or x<=rect[0]<rect[2]<=x+w:         # covered left part or in the middle
            if w<rect[0]:               # Try left
                widgets.CurrentDlg.move(x-(x+w-rect[0]), y)
            elif w<WinSize[0]-rect[2]:  # Try right
                widgets.CurrentDlg.move(rect[2], y)
        elif rect[0]<x<rect[2]:         # covered right part
            if w<WinSize[0]-rect[2]:    # Try right
                widgets.CurrentDlg.move(rect[2], y)
            elif w<rect[0]:             # Try left
                widgets.CurrentDlg.move(x-(x+w-rect[0]), y)

        if rect[1]<y+h<rect[3] or y<=rect[1]<rect[3]<=y+h:         # covered up part or in the middle
            if h<rect[1]:               # Try up
                widgets.CurrentDlg.move(x, y-(y+h-rect[1]))
            elif h<WinSize[1]-rect[3]:  # Try down
                widgets.CurrentDlg.move(x, rect[3])
        elif rect[1]<y<rect[3]:         # covered down part
            if h<WinSize[1]-rect[3]:    # Try down
                widgets.CurrentDlg.move(x, rect[3])
            elif h<rect[1]:             # Try up
                widgets.CurrentDlg.move(x, y-(y+h-rect[1]))

class MenuBarHandlers:
    def on_MonitorBtn_clicked(widget):
        BtnClicked('MonitorDlg')
        
    def on_PartolBtn_clicked(widget):
        BtnClicked('PatrolDlg')

    def on_LayoutBtn_clicked(widget):
        BtnClicked('LayoutDlg')
            
    def on_EmergencyBtn_clicked(widget):
        BtnClicked('EmergencyDlg')
        
    def on_ManageBtn_clicked(widget):
        BtnClicked('ManagementDlg')
            
    def on_TempRecBtn_clicked(widget):
        BtnClicked('TempRecDlg')
            
    def on_CaptureBtn_clicked(widget):
        BtnClicked('CaptureDlg')
        CaptureDlg.FlushImgList()
    
    def on_InformationBtn_clicked(widget):
        BtnClicked('InformationDlg')
    
    def on_SystemBtn_clicked(widget):
        BtnClicked('SystemDlg')

    def on_MatrixBtn_clicked(widget):
        BtnClicked('MatrixDlg')

## Belows are delete event for every dialog.
    def on_MonitorDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True

    def on_PatrolDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_LayoutDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_EmergencyDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True

    def on_ManagementDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_TempRecDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_CaptureDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_InformationDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
    
    def on_SystemDlg_delete_event(obj, event):
        CloseCurrentWidgets()
        return True

    def on_MatrixControl_delete_event(obj, event):
        CloseCurrentWidgets()
        return True
