#if !defined(__mstrmapp_h)              // Sentry, use file only if it's not already included.
#define __mstrmapp_h

/*  Project mastermi
    EDRON
    Copyright © 1995. All Rights Reserved.

    SUBSYSTEM:    mastermi.exe Application
    FILE:         mstrmapp.h
    AUTHOR:       Ed N & Ron L


    OVERVIEW
    ========
    Class definition for mastermiApp (TApplication).      
*/


#include <owl\owlpch.h>
#pragma hdrstop


#include "mstrmapp.rh"            // Definition of all resources.


//
// FrameWindow must be derived to override Paint for Preview and Print.
//
//{{TDecoratedFrame = SDIDecFrame}}
class SDIDecFrame : public TDecoratedFrame {
public:
    SDIDecFrame (TWindow *parent, const char far *title, TWindow *clientWnd, bool trackMenuSelection = false, TModule *module = 0);
    ~SDIDecFrame ();

//{{mastermiAppVIRTUAL_BEGIN}}
public:
   virtual void SetupWindow ();
//{{mastermiAppVIRTUAL_END}}

//{{SDIDecFrameVIRTUAL_BEGIN}}
public:
//{{SDIDecFrameVIRTUAL_END}}

//{{SDIDecFrameRSP_TBL_BEGIN}}
protected:
//{{SDIDecFrameRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(SDIDecFrame);
};    //{{SDIDecFrame}}


//{{TApplication = mastermiApp}}
class mastermiApp : public TApplication {
private:
    bool            HelpState;                          // Has the help engine been used.
    bool            ContextHelp;                        // SHIFT-F1 state (context sensitive HELP)
    HCURSOR         HelpCursor;                         // Context sensitive help cursor

    void SetupSpeedBar (TDecoratedFrame *frame);

public:
    mastermiApp ();
    virtual ~mastermiApp ();

//{{mastermiAppVIRTUAL_BEGIN}}
public:
    virtual void InitMainWindow();
    virtual bool CanClose ();
    virtual bool ProcessAppMsg (MSG& msg);
//{{mastermiAppVIRTUAL_END}}

//{{mastermiAppRSP_TBL_BEGIN}}
protected:
    void CmHelpContents ();
    void CmHelpUsing ();
    void CmHelpAbout ();
//{{mastermiAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(mastermiApp);
};    //{{mastermiApp}}


#endif                                      // __mstrmapp_h sentry.
