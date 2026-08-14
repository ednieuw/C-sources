#if !defined(__mstrmabd_h)              // Sentry, use file only if it's not already included.
#define __mstrmabd_h

/*  Project mastermi
    EDRON
    Copyright © 1995. All Rights Reserved.

    SUBSYSTEM:    mastermi.exe Application
    FILE:         mstrmabd.h
    AUTHOR:       Ed N & Ron L


    OVERVIEW
    ========
    Class definition for mastermiAboutDlg (TDialog).      
*/


#include <owl\owlpch.h>
#pragma hdrstop

#include "mstrmapp.rh"                  // Definition of all resources.


//{{TDialog = mastermiAboutDlg}}
class mastermiAboutDlg : public TDialog {
public:
    mastermiAboutDlg (TWindow *parent, TResId resId = IDD_ABOUT, TModule *module = 0);
    virtual ~mastermiAboutDlg ();

//{{mastermiAboutDlgVIRTUAL_BEGIN}}
public:
    void SetupWindow ();
//{{mastermiAboutDlgVIRTUAL_END}}
};    //{{mastermiAboutDlg}}


// Reading the VERSIONINFO resource.
class ProjectRCVersion {
public:
    ProjectRCVersion (TModule *module);
    virtual ~ProjectRCVersion ();

    bool GetProductName (LPSTR &prodName);
    bool GetProductVersion (LPSTR &prodVersion);
    bool GetCopyright (LPSTR &copyright);
    bool GetDebug (LPSTR &debug);

protected:
    LPBYTE      TransBlock;
    void FAR    *FVData;

private:
    // Don't allow this object to be copied.
    ProjectRCVersion (const ProjectRCVersion &);
    ProjectRCVersion & operator =(const ProjectRCVersion &);
};


#endif                                      // __mstrmabd_h sentry.
