/*  Project mastermi
    EDRON
    Copyright © 1995. All Rights Reserved.

    SUBSYSTEM:    mastermi.exe Application
    FILE:         mstrmapp.cpp
    AUTHOR:       Ed N & Ron L


    OVERVIEW
    ========
    Source file for implementation of mastermiApp (TApplication).      
*/


#include <owl\owlpch.h>
#pragma hdrstop


#include "mstrmapp.h"
#include "msttdlgc.h"                        // Definition of client class.       
#include "mstrmabd.h"                        // Definition of about dialog.       


//
// Generated help file.
//
const char HelpFileName[] = "mastermi.hlp";


//{{mastermiApp Implementation}}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(mastermiApp, TApplication)
//{{mastermiAppRSP_TBL_BEGIN}}
    EV_COMMAND(CM_HELPCONTENTS, CmHelpContents),
    EV_COMMAND(CM_HELPUSING, CmHelpUsing),
    EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
//{{mastermiAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//////////////////////////////////////////////////////////
// mastermiApp
// =====
//
mastermiApp::mastermiApp () : TApplication("mastermi")
{
    HelpState = false;
    ContextHelp = false;
    HelpCursor = 0;

    // INSERT>> Your constructor code here.
}


mastermiApp::~mastermiApp ()
{
    // INSERT>> Your destructor code here.
}


bool mastermiApp::CanClose ()
{
    bool result = TApplication::CanClose();

    //
    // Close the help engine if we used it.
    //
    if (result && HelpState)
        GetMainWindow()->WinHelp(HelpFileName, HELP_QUIT, 0L);
 
    return result;
}


void mastermiApp::SetupSpeedBar (TDecoratedFrame *frame)
{
frame=frame;
}
/*
	//
	// Create default toolbar New and associate toolbar buttons with commands.
	//
	TControlBar* cb = new TControlBar(frame);
	cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
	cb->Insert(*new TSeparatorGadget(6));
	cb->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
	cb->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
	cb->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));
	cb->Insert(*new TSeparatorGadget(6));
	cb->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
	cb->Insert(*new TSeparatorGadget(6));
	cb->Insert(*new TButtonGadget(CM_EDITFIND, CM_EDITFIND));
	cb->Insert(*new TButtonGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT));
	cb->Insert(*new TSeparatorGadget(6));
	cb->Insert(*new TButtonGadget(CM_HELPCONTENTS, CM_HELPCONTENTS));
 //   cb->Insert(*new TButtonGadget(DB_KNOP_BLUE, CM_HELPCONTENTS));

	// Add fly-over help hints.
	cb->SetHintMode(TGadgetWindow::EnterHints);

	frame->Insert(*cb, TDecoratedFrame::Top);
}
*/

//////////////////////////////////////////////////////////
// mastermiApp
// =====
// Application intialization.
//
void mastermiApp::InitMainWindow ()
{
    if (nCmdShow != SW_HIDE)
        nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWMAXIMIZED : nCmdShow;

    SDIDecFrame *frame = new SDIDecFrame(0, GetName(), 0, true);
    frame->SetFlag(wfShrinkToClient);

    //
    // Assign ICON w/ this application.
    //
    frame->SetIcon(this, IDI_SDIAPPLICATION);

    //
    // Menu associated with window and accelerator table associated with table.
    //
//    frame->AssignMenu(SDI_MENU);

	//
	// Associate with the accelerator table.
	//
//    frame->Attr.AccelTable = SDI_MENU;

//    SetupSpeedBar(frame);

	TStatusBar *sb = new TStatusBar(frame, TGadget::Recessed,
									TStatusBar::CapsLock        |
                                    TStatusBar::NumLock         |
                                    TStatusBar::ScrollLock      |
                                    TStatusBar::Overtype);
    frame->Insert(*sb, TDecoratedFrame::Bottom);
  
    SetMainWindow(frame);

    frame->SetMenuDescr(TMenuDescr(SDI_MENU));

    //
    // Windows 3-D controls.
	//
    EnableCtl3d(true);
}


//
// Build a response table for all messages/commands handled
// by the application.
//
DEFINE_RESPONSE_TABLE1(SDIDecFrame, TDecoratedFrame)
//{{SDIDecFrameRSP_TBL_BEGIN}}
//{{SDIDecFrameRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{SDIDecFrame Implementation}}


SDIDecFrame::SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, bool trackMenuSelection, TModule *module)
    : TDecoratedFrame(parent, title, clientWnd == 0 ? new mastermiTDLGClient(0) : clientWnd, trackMenuSelection, module)
{
    // INSERT>> Your constructor code here.

}


SDIDecFrame::~SDIDecFrame ()
{
	// INSERT>> Your destructor code here.

}


void SDIDecFrame::SetupWindow ()
{
    TDecoratedFrame::SetupWindow();
    
    TRect  r;
    GetWindowRect(r);

    r.bottom += 60;
    SetWindowPos(0, r, SWP_NOZORDER | SWP_NOMOVE);

    // INSERT>> Your code here.

}


//////////////////////////////////////////////////////////
// mastermiApp
// =====
// Menu Help Contents command
void mastermiApp::CmHelpContents ()
{
    //
    // Show the help table of contents.
	//
    HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_CONTENTS, 0L);
}


//////////////////////////////////////////////////////////
// mastermiApp
// =====
// Menu Help Using Help command
void mastermiApp::CmHelpUsing ()
{
    //
    // Display the contents of the Windows help file.
    //
	HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_HELPONHELP, 0L);
}


//////////////////////////////////////////////////////////
// mastermiApp
// ===========
// Menu Help About mastermi.exe command
void mastermiApp::CmHelpAbout ()
{
    //
    // Show the modal dialog.
    //
    mastermiAboutDlg(MainWindow).Execute();
}


bool mastermiApp::ProcessAppMsg (MSG& msg)
{
    if (msg.message == WM_COMMAND) {
        if (ContextHelp || (::GetKeyState(VK_F1) < 0)) {
            ContextHelp = false;
            GetMainWindow()->WinHelp(HelpFileName, HELP_CONTEXT, msg.wParam);
            return true;
        }
    } else
        switch (msg.message) {
        case WM_KEYDOWN:
			if (msg.wParam == VK_F1) {
                // If the Shift/F1 then set the help cursor and turn on the modal help state.
                if (::GetKeyState(VK_SHIFT) < 0) {
                    ContextHelp = true;
                    HelpCursor = ::LoadCursor(GetMainWindow()->GetModule()->GetInstance(), MAKEINTRESOURCE(IDC_HELPCURSOR));
                    ::SetCursor(HelpCursor);
                    return true;        // Gobble up the message.
                } else {
                    // If F1 w/o the Shift key then bring up help's main index.
                    GetMainWindow()->WinHelp(HelpFileName, HELP_INDEX, 0L);
                    return true;        // Gobble up the message.
                }
            } else {
                if (ContextHelp && (msg.wParam == VK_ESCAPE)) {
					if (HelpCursor)
                        ::DestroyCursor(HelpCursor);
                    ContextHelp = false;
                    HelpCursor = 0;
                    GetMainWindow()->SetCursor(0, IDC_ARROW);
                    return true;    // Gobble up the message.
                }
            }
            break;

        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
            if (ContextHelp) {
                ::SetCursor(HelpCursor);
				return true;        // Gobble up the message.
            }
            break;

        case WM_INITMENU:
            if (ContextHelp) {
                ::SetCursor(HelpCursor);
                return true;        // Gobble up the message.
            }
            break;
        
        case WM_ENTERIDLE:
            if (msg.wParam == MSGF_MENU)
                if (GetKeyState(VK_F1) < 0) {
					ContextHelp = true;
                    GetMainWindow()->PostMessage(WM_KEYDOWN, VK_RETURN, 0L);
                    return true;       // Gobble up the message.
                }
            break;
		case  WM_PAINT:  		break;
        default:
            ;
        };  // End of switch

    // Continue normal processing.

    return TApplication::ProcessAppMsg(msg);
}


int OwlMain (int , char* [])
{
	try
{
	    mastermiApp    app;
		return app.Run();
	}
	catch (xmsg& x) {
		::MessageBox(0, x.why().c_str(), "Exception", MB_OK);
	}

	return -1;
}



